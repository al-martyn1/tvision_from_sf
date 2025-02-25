/* Copyright (C) 1996-1998 Robert H�hne, see COPYING.RH for details */
/* Copyright (C) 1999-2000 Salvador Eduardo Tropea */
#include <stdlib.h>
#define Uses_string
#include <time.h>
#include <stdio.h>

#define Uses_TEvent
#define Uses_TEventQueue
#define Uses_TScreen
#include <tv.h>

//----------------- Common stuff ---------------

//#define DEBUG
#ifdef DEBUG
#ifdef __linux__
extern char *program_invocation_short_name;
#define LOG(s) cerr << program_invocation_short_name << ": " << s << "\n"
#else
#define LOG(s) cerr << __FILE__": " << s << "\n"
#endif
#else
#define LOG(s)
#endif

uchar THWMouse::buttonCount = 0;
Boolean THWMouse::handlerInstalled = False;
Boolean THWMouse::noMouse = False;

int use_mouse_handler = 1;

void THWMouse::registerHandler( unsigned , void (*)() )
{
}

THWMouse::THWMouse()
{
  resume(); // SET: to avoid duplicating code
}

THWMouse::~THWMouse()
{
  suspend();
}

void THWMouse::setRange( ushort , ushort )
{
}

//---------------- GPM version -----------------
#ifdef HAVE_GPM

#include <linux/keyboard.h>

extern "C" {
#include <gpm.h>
}

static int last_x=0,last_y=0,visible=0;
static unsigned short mouse_char;
static char XTermDetected=0;

extern int TScreen_suspended;

static
void get_mouse_char()
{
  mouse_char = TScreen::getCharacter(last_x+last_y*TScreen::screenWidth);
}

static
void set_mouse_char()
{
  unsigned short c = mouse_char ^ 0x7F00;
  TScreen::setCharacter(last_x+last_y*TScreen::screenWidth,c);
}

static
void reset_mouse_char()
{
  TScreen::setCharacter(last_x+last_y*TScreen::screenWidth,mouse_char);
}

static volatile
void show_mouse_char()
{
  if (!visible) return;
  if (last_x > TScreen::screenWidth)
    last_x = 0;
  if (last_y > TScreen::screenHeight)
    last_y = 0;
  get_mouse_char();
  set_mouse_char();
}

int draw_mouse(int x,int y)
{
  if (TScreen::suspended) return 0;
  if (x != last_x || y != last_y)
  {
    if (visible) reset_mouse_char();
    last_x = x;
    last_y = y;
    show_mouse_char();
    return 1;
  }
  return 0;
}   

static
int SetUpGPMConnection()
{
 Gpm_Connect conn;
 
 conn.eventMask = ~0;	/* I want all events */
 conn.defaultMask = 0;	/* no default treatment */
 /* It doesn't report events from GPM when ALT or ALTGR are pressed.
    I saw it in mc and my friends from BALUG, Ivan and Laszlo bugged me
    about adding it. Was hyper-easy, no? (SET) */
 conn.maxMod = ~((1<<KG_ALT) | (1<<KG_ALTGR));
 conn.minMod = 0;
 gpm_zerobased = 1;	/* coordinates start from zero */
 
 return Gpm_Open(&conn, 0);
}

void THWMouse::resume()
{
 if (handlerInstalled)
    return;
 fflush(stderr);
 // SET: Are we in a xterm* ?
 // Note: we can't try gpm first, it mess the xterm mouse, it took me a lot
 // of time to figure out why it worked in a simple example but not in a real
 // Turbo Vision program.
 char *term=getenv("TERM");
 if (term && strncmp(term,"xterm",5)==0)
   {
    handlerInstalled=True;
    XTermDetected=1;
    buttonCount=3; // X uses 3 buttons
    // Save old hilit tracking and enable mouse tracking (GPM_XTERM_ON)
    TScreen::SendToTerminal("\x1B[?1001s\x1B[?1000h");
    LOG("using xterm* mouse");
   }
 else
   { // Try gpm
    if (gpm_fd!=-1)
      { // It can happend only if a bug is there
       handlerInstalled=True;
       return;
      }
    if (SetUpGPMConnection()<0)
       LOG("no gpm, running without mouse");
    else
      {
       LOG("gpm server version " << Gpm_GetServerVersion(NULL));
       handlerInstalled=True;
       buttonCount=3;
       /*Gpm_Event mev; Just draw the cursor, no way to move it, a really
                        huge limitation.
       Gpm_GetEvent(&mev);
       mev.x=mev.y=0;
       GPM_DRAWPOINTER (&mev);*/
      }
   }
  show();
}

void THWMouse::suspend()
{
 if (!handlerInstalled)
    return;
 handlerInstalled=False;
 hide();
 buttonCount=0;

 if (XTermDetected)
   {
    XTermDetected=0;
    // Disable mouse tracking and restore old hilittracking (GPM_XTERM_OFF)
    TScreen::SendToTerminal("\x1B[?1000l\x1B[?1001r");
    LOG("xterm mouse disabled");
   }
 else
   {
    if (gpm_fd==-1)
       return;
    Gpm_Close();
    LOG("gpm connection closed")
   }
}

void THWMouse::show()
{
  if (!present()) return;
  if (visible) return;
  visible = 1;
  show_mouse_char();
}

void THWMouse::hide()
{
  if (!present()) return;
  if (!visible) return;
  reset_mouse_char();
  visible = 0;
}

inline
int range(int test, int min, int max)
{
	return test < min ? min : test > max ? max : test;
}

static int m_x=0,m_y=0,m_b=0,m_bOld=0;
static int forced=0;

/**[txh]********************************************************************

  Description:
  It forces the state of the mouse externally, the next call to getEvent
will return this values instead of values from the mouse driver. That's
used to simuate the mouse with other events like keyboard.@p
  One interesting problem is that TV expects one event for the position
change and another for the button change, both can't be provided in one
step so we must emulate it too. (SET)

***************************************************************************/

void THWMouse::forceEvent(int x, int y, int buttons)
{
 forced=0;
 if (m_x!=x || m_y!=y)
    forced++;
 if (m_b!=buttons)
    forced++;
 m_bOld=m_b;
 m_x=x; m_y=y; m_b=buttons;
}

void THWMouse::getEvent( MouseEventType &me )
{
  if (!handlerInstalled)
     return;
  Gpm_Event mev;
  me.where.x = m_x;
  me.where.y = m_y;
  me.doubleClick = False;
  if (forced)
    { // SET: 2 steps, one the movement, other the buttons
     if (forced==1)
       {
        me.buttons=m_b;
        //draw_mouse(m_x,m_y);
       }
     else
       {
        me.buttons=m_bOld;
       }
     draw_mouse(m_x,m_y);
     forced--;
    }
  else
    {
     me.buttons = m_b;
     if (!XTermDetected && !Gpm_Repeat(1) && (Gpm_GetEvent(&mev) == 1))
       {
        int b = mev.buttons;
        if ((b & GPM_B_LEFT) && !(mev.type & GPM_UP))
          me.buttons |= mbLeftButton;
        else
          me.buttons &= ~mbLeftButton;
        if ((b & GPM_B_RIGHT) && !(mev.type & GPM_UP))
          me.buttons |= mbRightButton;
        else
          me.buttons &= ~mbRightButton;
        m_b = me.buttons;
        m_x = me.where.x = range(mev.x, 0, TScreen::screenWidth - 1);
        m_y = me.where.y = range(mev.y, 0, TScreen::screenHeight - 1);
        draw_mouse(m_x,m_y);
       }
    }
  // curMouse must be set, because it is used by other functions
  TEventQueue::curMouse = me;
}

#else // HAVE_GPM

// Just some dummies in case we don't have/want mouse

void THWMouse::resume()
{
}

void THWMouse::suspend()
{
}

void THWMouse::show()
{
}

void THWMouse::hide()
{
}

void THWMouse::forceEvent(int , int , int )
{
}

void THWMouse::getEvent( MouseEventType & )
{
}

#endif
