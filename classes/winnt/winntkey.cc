/*****************************************************************************

 This code is GPL, see copying file for more details.
 Keyboard handler for Win32 Copyright by Anatoli Soltan (2000)
 Based on the handler for Linux Copyright by Salvador E. Tropea (SET) (1998,1999)

*****************************************************************************/
#include <tv/configtv.h>
#define Uses_TEvent
#define Uses_TGKey
#define Uses_FullSingleKeySymbols
#include <tv.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern HANDLE __tvWin32ConInp;
static INPUT_RECORD inpRec;
static ushort shiftState;

extern ushort vk2kk[];

unsigned short getshiftstate(void)
{
  return shiftState;
}


void TGKey::SetKbdMapping(int /*version*/)
{
}

int TGKey::CompareASCII(uchar val, uchar code)
{
  return val==code;
}

unsigned short TGKey::gkey(void)
{
  GetRaw();
  return 0;
}

void TGKey::GetRaw(void)
{
  DWORD cEvents;
  ReadConsoleInput(__tvWin32ConInp, &inpRec, 1, &cEvents);
}

int TGKey::kbhit(void)
{
  DWORD cEvents;
  while (PeekConsoleInput(__tvWin32ConInp, &inpRec, 1, &cEvents) && cEvents == 1)
    {
     if (inpRec.EventType != KEY_EVENT)
       {
        // The following two lines are commented because if someone waits for
        // a key press by calling kbhit in a loop and a mouse event is in the
        // queue, the loop will never end.
        //if (inpRec.EventType == MOUSE_EVENT)
        //  return 0;
        ReadConsoleInput(__tvWin32ConInp, &inpRec, 1, &cEvents);
        continue;
       }
     if (!inpRec.Event.KeyEvent.bKeyDown) // skeek keyup events
       ReadConsoleInput(__tvWin32ConInp, &inpRec, 1, &cEvents);
     else
       return 1;
    }
  return 0;
}

void TGKey::clear(void)
{
  FlushConsoleInputBuffer(__tvWin32ConInp);
}

void TGKey::fillTEvent(TEvent &e)
{
  TGKey::gkey();

  DWORD dwCtlState = inpRec.Event.KeyEvent.dwControlKeyState;
  shiftState = 0;
  if (dwCtlState & SHIFT_PRESSED)
    shiftState |= kbShiftCode;
  else if (dwCtlState & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED))
    shiftState |= kbCtrlCode;
  else if (dwCtlState & LEFT_ALT_PRESSED)
    shiftState |= kbAltLCode;
  else if (dwCtlState & RIGHT_ALT_PRESSED)
    shiftState |= kbAltRCode;
  //else if (dwCtlState & SCROLLLOCK_ON)
  //  shiftState |= kbScrollState;
  //else if (dwCtlState & NUMLOCK_ON)
  //  shiftState |= kbNumState;
  //else if (dwCtlState & CAPSLOCK_ON)
  //  shiftState |= kbCapsState;

  e.keyDown.charScan.scanCode = inpRec.Event.KeyEvent.wVirtualScanCode;
  e.keyDown.charScan.charCode = inpRec.Event.KeyEvent.uChar.AsciiChar;
  e.keyDown.shiftState = shiftState;
  e.keyDown.keyCode = (inpRec.Event.KeyEvent.wVirtualKeyCode < 256)
    ? vk2kk[inpRec.Event.KeyEvent.wVirtualKeyCode] : (ushort)0;
  if (e.keyDown.keyCode == 0)
    return;
  e.what = evKeyDown;
  e.keyDown.keyCode |= shiftState;
}

ushort TGKey::AltSet=0;  // Default: Left and right key are different ones

static ushort vk2kk[256] = {
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x002a, 0x002b, 0x0000, 0x0000, 0x0000, 0x002c, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x001f, 0x0000, 0x0000, 0x0000, 0x0000,
0x0034, 0x0047, 0x004c, 0x004a, 0x0045, 0x0048, 0x0046, 0x0049,
0x004b, 0x0000, 0x0000, 0x0000, 0x0000, 0x004d, 0x004e, 0x0000,
0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
0x0028, 0x0029, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
0x0018, 0x0019, 0x001a, 0x0063, 0x0064, 0x0065, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0036, 0x0000, 0x0035, 0x0000, 0x0000,
0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040,
0x0041, 0x0042, 0x0043, 0x0044, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x002d, 0x0038, 0x0059, 0x005e, 0x005a, 0x005b,
0x0061, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0056, 0x001c, 0x0057, 0x002e, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

