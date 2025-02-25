/*----------------------------------------------------------*/
/*                                                          */
/*   Turbo Vision TVDEMO source file                        */
/*                                                          */
/*----------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */
/*
 * Modified by Sergio Sigala <ssigala@globalnet.it>
 */
 
// SET: moved the standard headers before tv.h
#include <stdio.h>
#define Uses_string

#define Uses_TView
#define Uses_TRect
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TKeys
#define Uses_MsgBox
#define Uses_fpstream
#define Uses_TEvent
#define Uses_TDeskTop
#define Uses_TApplication
#define Uses_TWindow
#define Uses_TDeskTop
#define Uses_TScreen
#define Uses_IOS_BIN
#include <tv.h>

#include "tvdemo.h"
#include "gadgets.h"
#include "fileview.h"
#include "puzzle.h"
#include "demohelp.h"
#include <help.h>

/* SS: changed */

//#ifdef __DPMI32__
#define HELP_FILENAME "demohelp.h32"
//#else
//#define HELP_FILENAME "DEMOHELP.H16"
//#endif

__link(RCalculator);
__link(RCalcDisplay);
//
// main: create an application object.  Constructor takes care of all
//   initialization.  Calling run() from TProgram makes it tick and
//   the destructor will destroy the world.
//
//   File names can be specified on the command line for automatic
//   opening.
//

int main(int argc, char **argv)
{
    TVDemo *demoProgram = new TVDemo(argc, argv);

    TScreen::SetWindowTitle("Turbo Vision Demo Program");
    demoProgram->run();

    TObject::destroy( demoProgram );
    return 0;
}


//
// Constructor for the application.  Command line parameters are interpreted
//   as file names and opened.  Wildcards are accepted and put up a dialog
//   box with the appropriate search path.
//

TVDemo::TVDemo( int argc, char **argv ) :
    TProgInit( &TVDemo::initStatusLine,
               &TVDemo::initMenuBar,
               &TVDemo::initDeskTop )
{
    TView *w;
    char fileSpec[128];
    int len;

    TRect r = getExtent();                      // Create the clock view.
    r.a.x = r.b.x - 9;      r.b.y = r.a.y + 1;
    clock = new TClockView( r );
    insert(clock);

    r = getExtent();                            // Create the heap view.
    r.a.x = r.b.x - 13;     r.a.y = r.b.y - 1;
    heap = new THeapView( r );
    insert(heap);

    while (--argc > 0)                              // Display files specified
        {                                           //  on command line.
        strcpy( fileSpec, *++argv );
        len = strlen( fileSpec );

	/* SS: changed */

        if( fileSpec[len-1] == '/' )
            strcat( fileSpec, "*.*" );
        if( strchr( fileSpec, '*' ) || strchr( fileSpec, '?' ) )
            openFile( fileSpec );
        else
            {
            w = validView( new TFileWindow( fileSpec ) );
            if( w != 0 )
                deskTop->insert(w);
            }
        }

}


//
// DemoApp::getEvent()
//  Event loop to check for context help request
//

void TVDemo::getEvent(TEvent &event)
{
    TWindow *w;
    THelpFile *hFile;
    fpstream *helpStrm;
    static Boolean helpInUse = False;

    TApplication::getEvent(event);
    switch (event.what)
        {
        case evCommand:
            if ((event.message.command == cmHelp) && ( helpInUse == False)) 
                {
                helpInUse = True;
                helpStrm = new fpstream(HELP_FILENAME, ios::in|IOS_BIN);
                hFile = new THelpFile(*helpStrm);
                if (!helpStrm)
                    {
                    messageBox("Could not open help file", mfError | mfOKButton);
                    delete hFile;
                    }
                else
                    {
                    w = new THelpWindow(hFile, getHelpCtx());
                    if (validView(w) != 0)
                        {
                        execView(w);
                        destroy( w );
                        }
                    clearEvent(event);
                    }
                helpInUse = False;
                }
            break;
        case evMouseDown:
            if (event.mouse.buttons != 1)
                event.what = evNothing;
            break;
        }

}  

//
// Create statusline.
//

TStatusLine *TVDemo::initStatusLine( TRect r )
{
    r.a.y = r.b.y - 1;

    return (new TStatusLine( r,
      *new TStatusDef( 0, 50 ) +
        *new TStatusItem( "~F1~ Help", kbF1, cmHelp ) +
        *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit ) +
        *new TStatusItem( 0, kbAltF3, cmClose ) +
        *new TStatusItem( 0, kbF10, cmMenu ) +
        *new TStatusItem( 0, kbF5, cmZoom ) +
        *new TStatusItem( 0, kbCtrlF5, cmResize ) +
      *new TStatusDef( 50, 0xffff ) +
        *new TStatusItem( "Howdy", kbF1, cmHelp )
        )
    );
}


//
// Puzzle function
//

void TVDemo::puzzle()
{
    TPuzzleWindow *puzz = (TPuzzleWindow *) validView(new TPuzzleWindow);

    if(puzz != 0)
        {
        puzz->helpCtx = hcPuzzle;
        deskTop->insert(puzz);
	}
}


//
// retrieveDesktop() function ( restores the previously stored Desktop )
//

void TVDemo::retrieveDesktop()
{
	/* SS: changed */

//    struct ffblk ffblk;

//    if (findfirst("TVDEMO.DST", &ffblk, 0))
	FILE *fp;
	if ((fp = fopen("TVDEMO.DST", "r")) == NULL)
        messageBox("Could not find desktop file", mfOKButton | mfError);
        else
        {
	fclose(fp);
        fpstream *f = new fpstream("TVDEMO.DST", ios::in|IOS_BIN);
        if( !f )
            messageBox("Could not open desktop file", mfOKButton | mfError);
        else
           {
           TVDemo::loadDesktop(*f);
           if( !f )
               messageBox("Error reading desktop file", mfOKButton | mfError);
           }
        delete f;
        }
}

//
// saveDesktop() function ( saves the DeskTop by calling storeDesktop function )
//

void TVDemo::saveDesktop()
{
    fpstream *f = new fpstream("TVDEMO.DST", ios::out|IOS_BIN);

    if( f )
        {
        TVDemo::storeDesktop(*f);
        if( !f )
            {
            messageBox("Could not create TVDEMO.DST.", mfOKButton | mfError);
            delete f;
            ::remove("TVDEMO.DST");
            return;
            }
        }
    delete f;
}

//
// writeView() function ( writes a view object to a resource file )
//

static void writeView(TView *p, void *strm)
{
   fpstream *s = (fpstream *) strm;
   if (p != TProgram::deskTop->last)
      *s << p;
}

//
// storeDesktop() function ( stores the Desktop in a resource file )
//

void TVDemo::storeDesktop(fpstream& s)
{
  deskTop->forEach(::writeView, &s);
  s << 0;
}

//
// Tile function
//

void TVDemo::tile()
{
    deskTop->tile( deskTop->getExtent() );
}
