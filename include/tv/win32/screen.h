/* Win32 screen routines header.
   Copyright (c) 2002 by Salvador E. Tropea (SET)
   Covered by the GPL license. */
// This headers needs windows header
#ifndef WIN32SCR_HEADER_INCLUDED
#define WIN32SCR_HEADER_INCLUDED

// virtual to avoid problems with multiple inheritance
class TDisplayWin32 : virtual public TDisplay
{
protected:
 TDisplayWin32() {};
 // We will use casts to base classes, destructors must be pointers
 virtual ~TDisplayWin32();
 // This sets the pointers of TDisplay to point to this class
 static void   Init();

 // Behaviors to replace TDisplay
 static void   SetCursorPos(unsigned x, unsigned y);
 static void   GetCursorPos(unsigned &x, unsigned &y);
 static void   GetCursorShape(unsigned &start, unsigned &end);
 static void   SetCursorShape(unsigned start, unsigned end);
 static ushort GetRows();
 static ushort GetCols();
 // Not available static void setCrtMode(ushort mode);
 // Not available static ushort getCrtMode();
 // Not available static void   SetCrtModeExt(char *mode);
 static int    CheckForWindowSize(void);
 static const char *GetWindowTitle(void);
 static int SetWindowTitle(const char *name);
 static void SetCrtMode(ushort);
 static void SetCrtModeExt(char *);

 // Variables for this driver
 // Input/output handles
 static HANDLE hOut,hIn;
 // Console information
 static CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
 // Cursor information
 static CONSOLE_CURSOR_INFO ConsoleCursorInfo;
 // Lock for the WindowSizeChanged variable
 static CRITICAL_SECTION lockWindowSizeChanged;
 // Used to indicate a window size change
 static int WindowSizeChanged;
};

// With this order the destructor will be called first for TScreenWin32,
// TScreen, TDisplayWin32 and finally TDisplay.
class TScreenWin32 : public TDisplayWin32, public TScreen
{
public:
 TScreenWin32();
 // We will use casts to base classes, destructors must be pointers
 virtual ~TScreenWin32();

 friend class TGKeyWin32;
 friend class THWMouseWin32;
 friend class TDisplayWin32;

protected:
 static void   Resume();
 static void   Suspend();
 // Default: ushort fixCrtMode( ushort mode )
 // Default: static void   setCrtData();
 static void   clearScreen();
 // Default: static void   setVideoMode(ushort mode);
 // Default: static void   setVideoModeExt(char *mode);
 // Default: static void   getCharacters(unsigned offset,ushort *buf,unsigned count);
 // Default: static ushort getCharacter(unsigned dst);
 static void   setCharacter(unsigned offset, ushort value);
 static void   setCharacters(unsigned dst, ushort *src, unsigned len);
 static int    System(const char *command, pid_t *pidChild);

 // Support functions
 static void   SaveScreen();
 static void   SaveScreenReleaseMemory();
 static void   RestoreScreen();
 static void   InitConsole();
 static void   DoneConsole();
 static DWORD WINAPI HandleEvents(void* p);

 // Support variables
 static CHAR_INFO *buffer;
 static CONSOLE_SCREEN_BUFFER_INFO info;
 // Flag to stop the events thread
 static int ExitEventThread;
 // Handler for the events thread
 static HANDLE EventThreadHandle;

 friend class THWMouseWin32;
 friend class TGKeyWin32;
};

#endif // WIN32SCR_HEADER_INCLUDED

