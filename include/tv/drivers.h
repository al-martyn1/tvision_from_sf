typedef TScreen *(*drvChecker)();

extern TScreen *TV_DOSDriverCheck();
extern TScreen *TV_LinuxDriverCheck();
extern TScreen *TV_QNXRtPDriverCheck();
extern TScreen *TV_UNIXDriverCheck();
extern TScreen *TV_Win32DriverCheck();
extern TScreen *TV_XDriverCheck();

