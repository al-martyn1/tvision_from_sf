/**[txh]********************************************************************

  Description:
  This header works as a distribution point. It parses the Uses_*
definitions and includes (or defines) the needed stuff.@p
  Important guidelines:@*

  Never include string.h use Use_string instead.
  strlwr and strupr are defined.@*
  stricmp and strnicmp are deprecated, use strcasecmp and strncasecmp
instead.@*
  Never use ios::bin (CLY_IOSBin).
  Never use filebuf::openprot (CLY_FBOpenProtDef).
  The O_TEXT and O_BINARY from fcntl needs Uses_fcntl.
  S_IS* from sys/stat needs Uses_sys_stat.
  Never use s/random, use s/rand which are ANSI/POSIX.
  Streambuf members: setbuf, seekoff and sync have wrappers requested with
Uses_PubStreamBuf.
  Request alloca with Uses_alloca.
  Never use movmem, use memmove instead (ANSI/POSIX).
  Even when supported the old calls to fexpand, isDir, pathValid,
validFileName, getCurDir, isWild, __file_exists, relativePath and driveValid
should be replaced by a CLY_* function.
  When possible POSIX compliant replacements are supplied for: glob, fnmatch
and regex.
  Added itoa for faster and safe integer to string conversion.

  Copyright (c) 2000-2001 by Salvador E. Tropea
  Covered by the GPL license.

***************************************************************************/

#ifndef CLY_CompatLayerIncluded
//#define CLY_CompatLayerIncluded

#include <tv/configtv.h>

/* MSS memory leak debugger */
#ifdef MSS
 #include <mss.h>
#endif

#ifdef __cplusplus
#define CFunc extern "C"
#else
#define CFunc extern
#endif

#ifndef CLY_DoNotDefineSizedTypes
/* The following types should be platform independent */
typedef signed char int8;
typedef short       int16;
typedef int         int32;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
#if defined(TVComp_GCC)
typedef unsigned long long uint64;
typedef          long long int64;
#elif defined(TVComp_BCPP) || defined(TVComp_MSC)
typedef unsigned __int64 uint64;
typedef          __int64 int64;
#else
 #error Can not define uint64 type: unknown compiler.
#endif
#endif // CLY_DoNotDefineSizedTypes

#if defined(CLY_DefineUTypes) || defined(__cplusplus)
/* The following are just aliases and the size is platform dependant */
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;
#endif // CLY_DefineUTypes


#ifdef Uses_stdio
  #define Include_stdio
#endif

#ifdef Uses_CLYFileAttrs
 // For mode_t
 #define Uses_sys_stat
#endif

#define FSTREAM_HEADER  <fstream.h>
#define IOMANIP_HEADER  <iomanip.h>
#define IOSTREAM_HEADER <iostream.h>

#ifdef Uses_time
 #define Include_time
#endif

#ifdef TVComp_GCC
// GNU C is supported for various OSs:

 #ifdef Uses_string
  #define Include_string
 #endif
 #ifdef Uses_limits
  #define Include_limits
 #endif
 #ifdef Uses_fcntl
  #define Include_fcntl
 #endif
 #ifdef Uses_sys_stat
  #define Include_sys_stat
 #endif
 #ifdef Uses_ctype
  #define Include_ctype
 #endif
 #ifdef Uses_unistd
  #define Include_unistd
 #endif
 #ifdef Uses_access
  #define Include_unistd
 #endif
 #ifdef Uses_alloca
  #define Include_stdlib
 #endif
 #ifdef Uses_free
  #define Include_stdlib
 #endif
 #ifdef Uses_stdlib
  #define Include_stdlib
 #endif
 #ifdef Uses_dirent
  #define Include_dirent
 #endif
 #ifdef Uses_abort
  #define Include_stdlib
 #endif
 #ifdef Uses_AllocLocal
  #define AllocLocalStr(s,l) char s[l]
  #define AllocLocalUShort(s,l) ushort s[l]
 #endif
 #define NEVER_RETURNS __attribute__((noreturn))
 #define RETURN_WHEN_NEVER_RETURNS
 #define CLY_Packed __attribute__((packed))
 // SET: Anything allocated with new[] should be deleted with [].
 // Pointed out by Laurynas Biveinis.
 #define DeleteArray(a) delete[] a
 #ifdef Uses_getcurdir
  #define getcurdir CLY_getcurdir
 #endif
 // Not yet supported for Win9x with Ming
 // SET: I guess they can be defined for Win9x, but Anatoli's code seems to
 // be using NETAPI32.DLL functions that only exists under NT.
 #define CLY_IsUNCShare(a) (0)
 #define CLY_IsUNC(a)      (0)
 #ifdef Uses_ftell
  #define Include_stdio
 #endif
 #ifdef Uses_strstream
  #define Include_strstream
 #endif
 // ifstream::getline behaves strangely in BC++
 // I take the gcc implementation.
 #define IfStreamGetLine(istream,buffer,size) \
         istream.getline(buffer,size)

 // ISO C++ 1998 standard changed a lot of details in the standard C++
 // library. GCC implemented it in version 3.0. BC++ implemented some
 // stuff in versions like BC++ 5.5. So that's a real mess.
 #if __GNUC__>=3
  #define CLY_ISOCpp98 1
  #define CLY_OpenModeT      std::ios::openmode
  #define CLY_StreamPosT     std::streampos
  #define CLY_StreamOffT     std::streamoff
  #define CLY_IOSSeekDir     std::ios::seekdir
  #define CLY_FBOpenProtDef  0
  #define CLY_NewFBFromFD(f) new filebuf(fdopen(f,"rb+"),ios::in|ios::out|ios::binary)
  #define CLY_PubSetBuf(a,b) pubsetbuf(a,b)
  #undef  CLY_HaveFBAttach
  #define CLY_FBOpen(a,b,c)  open(a,b)
  #define CLY_IOSBin         std::ios::binary
  #define CLY_PubSeekOff     pubseekoff
  #define CLY_PubSync        pubsync
  #define CLY_std(a)         std::a
  #define UsingNamespaceStd  using namespace std;
  #define CreateStrStream(os,buf,size) std::string buf; std::ostringstream os(buf)
  #define GetStrStream(buf) buf.c_str()
  #ifdef Uses_StrStream
   #define Include_sstream
  #endif
  #undef  FSTREAM_HEADER
  #define FSTREAM_HEADER  <fstream>
  #undef  IOMANIP_HEADER
  #define IOMANIP_HEADER  <iomanip>
  #undef  IOSTREAM_HEADER
  #define IOSTREAM_HEADER <iostream>
 #else
  #define CLY_OpenModeT      int
  #define CLY_StreamPosT     streampos
  #define CLY_StreamOffT     streamoff
  #define CLY_IOSSeekDir     ios::seek_dir
  #define CLY_FBOpenProtDef  filebuf::openprot
  #define CLY_NewFBFromFD(f) new filebuf(f)
  #define CLY_PubSetBuf(a,b) setbuf(a,b)
  #define CLY_HaveFBAttach   1
  #define CLY_FBOpen(a,b,c)  open(a,b,c)
  #define CLY_IOSBin         ios::bin
  #define CLY_PubSeekOff     seekoff
  #define CLY_PubSync        sync
  #define CLY_std(a)         a
  #define UsingNamespaceStd
  #define CreateStrStream(os,buf,size) char buf[size]; \
                                       ostrstream os(buf,sizeof(buf))
  #define GetStrStream(buf) buf
  #ifdef Uses_StrStream
   #define Include_strstream
  #endif
 #endif
 
 /* Use the internal bool type for Boolean */
 #undef Boolean
 #undef False
 #undef True
 #define Boolean bool
 #define True true
 #define False false
 
 #ifdef TVCompf_MinGW
  #define CLY_UseCrLf 1
  #define CLY_HaveDriveLetters 1
  #define FA_ARCH   0x01
  #define FA_DIREC  0x02
  #define FA_RDONLY 0x04
  #ifdef Uses_filelength
   #define filelength _filelength
   #define Include_io
  #endif
  #define PATHSEPARATOR ';'
  #define PATHSEPARATOR_ ";"
  // SET: Not sure why Vadim wanted it. Perhaps to make the port more usable
  // for users accustomed to back slashes, but then the library isn't uniform
  // across platforms
  #define DIRSEPARATOR '\\'
  #define DIRSEPARATOR_ "\\"
  #define CLY_IsValidDirSep(a) (a=='/' || a=='\\')
  #ifdef Uses_fixpath
   CFunc void _fixpath(const char *in, char *out);
  #endif
  #ifdef Uses_HaveLFNs
   #define OS_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_cl_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_cl_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_cl_regex
  #endif
  #ifdef Uses_getopt
   #define Include_cl_getopt
  #endif
  #ifdef Uses_io
   #define Include_io
  #endif
  #ifdef Uses_utime
   // I can't beleive they forgot the header when the function is there!
   #define Include_cl_utime
   #define Include_time
  #endif
  #ifdef Uses_mkstemp
   CFunc int mkstemp(char *_template);
  #endif
  #ifdef Uses_getcwd
   #define Include_io
  #endif
  #ifdef Uses_itoa
   #define Include_stdlib
  #endif
  #ifdef Uses_direct
   #define Include_direct
  #endif
  #ifdef Uses_dir
   #define Include_dir
  #endif
  #ifdef Uses_nl_langinfo
   #define Uses_CLY_nl_langinfo
  #endif
 #endif

 // Win32/Cygwin -- OH!
 #ifdef TVCompf_Cygwin
  #define CLY_Have_UGID 1
  #define FA_ARCH   0x01
  #define FA_DIREC  0x02
  #define FA_RDONLY 0x04
  #ifdef Uses_filelength
   #define filelength CLY_filelength
  #endif
  #define PATHSEPARATOR ':'
  #define PATHSEPARATOR_ ":"
  #define DIRSEPARATOR '/'
  #define DIRSEPARATOR_ "/"
  #define CLY_IsValidDirSep(a) (a=='/')
  #ifdef Uses_fixpath
   CFunc void _fixpath(const char *in, char *out);
  #endif
  #ifdef Uses_HaveLFNs
   #define OS_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_cl_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_sys_types
   #define Include_regex
  #endif
  #ifdef Uses_getopt
   #define Include_getopt
  #endif
  #ifdef Uses_utime
   #define Include_utime
  #endif
  #ifdef Uses_mkstemp
   #define Include_stdio
  #endif
  #ifdef Uses_getcwd
   #define Include_unistd
  #endif
  #ifdef Uses_itoa
   CFunc char *itoa(int value, char *string, int radix);
  #endif
  #ifdef Uses_dir
   #define Include_dir
   #define Include_direct
  #endif
  #ifdef Uses_nl_langinfo
   #define Uses_CLY_nl_langinfo
  #endif
 #endif
 
 // Under DOS djgpp defines it
 #ifdef TVCompf_djgpp
  #define CLY_UseCrLf 1
  #define CLY_HaveDriveLetters 1
  #ifdef Uses_filelength
   #define Include_io
  #endif
  #define PATHSEPARATOR ';'
  #define PATHSEPARATOR_ ";"
  #define DIRSEPARATOR '/'
  #define DIRSEPARATOR_ "/"
  #define CLY_IsValidDirSep(a) (a=='/' || a=='\\')
  #ifdef Uses_fixpath
   #define Include_sys_stat
  #endif
  #ifdef Uses_HaveLFNs
   #define Include_fcntl
   #define DJGPP_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_sys_types
   #define Include_regex
  #endif
  #ifdef Uses_getopt
   #define Include_cl_getopt
  #endif
  #ifdef Uses_io
   #define Include_io
  #endif
  #ifdef Uses_utime
   #define Include_utime
  #endif
  #ifdef Uses_mkstemp
   #define Include_stdio
  #endif
  #ifdef Uses_getcwd
   #define Include_unistd
  #endif
  #ifdef Uses_itoa
   #define Include_stdlib
  #endif
  #ifdef Uses_direct
   #define Include_direct
  #endif
  #ifdef Uses_dir
   #define Include_dir
  #endif
  #ifdef Uses_nl_langinfo
   #define Uses_CLY_nl_langinfo
  #endif
 #endif

 // Common to all UNIX systems
 #ifdef TVOS_UNIX
  #define CLY_Have_UGID 1
  #define FA_ARCH   0x01
  #define FA_DIREC  0x02
  #define FA_RDONLY 0x04
  #define PATHSEPARATOR ':'
  #define PATHSEPARATOR_ ":"
  #define DIRSEPARATOR '/'
  #define DIRSEPARATOR_ "/"
  #define CLY_IsValidDirSep(a) (a=='/')
 #endif

 // Linux (glibc)
 #ifdef TVOSf_Linux
  #ifdef Uses_string
   CFunc char *strlwr(char *s);
   CFunc char *strupr(char *s);
  #endif
  #ifdef Uses_filelength
   #define filelength CLY_filelength
  #endif
  #ifdef Uses_fixpath
   CFunc void _fixpath(const char *in, char *out);
  #endif
  #ifdef Uses_HaveLFNs
   #define OS_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_sys_types
   #define Include_regex
  #endif
  #ifdef Uses_getopt
   #define Include_getopt
  #endif
  #ifdef Uses_utime
   #define Include_utime
  #endif
  #ifdef Uses_mkstemp
   #define Include_stdio
  #endif
  #ifdef Uses_CLYFileAttrs
   // Is that a bug? shouldn't be sys/stat.h enough?
   #define Include_sys_types
  #endif
  #ifdef Uses_getcwd
   #define Include_unistd
  #endif
  #ifdef Uses_itoa
   CFunc char *itoa(int value, char *string, int radix);
  #endif
  #ifdef Uses_direct
   #define Include_direct
  #endif
  #ifdef Uses_dir
   #define Include_dir
  #endif
  #ifdef Uses_nl_langinfo
   #define Include_langinfo
  #endif
 #endif

 // Solaris using gcc but not glibc
 #ifdef TVOSf_Solaris
  #ifdef Uses_string
   CFunc char *strlwr(char *s);
   CFunc char *strupr(char *s);
  #endif
  #ifdef Uses_filelength
   #define filelength CLY_filelength
  #endif
  #ifdef Uses_fixpath
   CFunc void _fixpath(const char *in, char *out);
  #endif
  #ifdef Uses_HaveLFNs
   #define OS_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_sys_types
   #define Include_regex
  #endif
  #ifdef Uses_getopt
   #define Include_cl_getopt
  #endif
  #ifdef Uses_utime
   #define Include_utime
  #endif
  #ifdef Uses_mkstemp
   #define Include_stdio
  #endif
  #ifdef Uses_getcwd
   #define Include_unistd
  #endif
  #ifdef Uses_itoa
   CFunc char *itoa(int value, char *string, int radix);
  #endif
  #ifdef Uses_direct
   #define Include_direct
  #endif
  #ifdef Uses_dir
   #define Include_dir
  #endif
  #ifdef Uses_nl_langinfo
   #define Uses_CLY_nl_langinfo
  #endif
 #endif

 // Generic UNIX system
 #if defined(TVOS_UNIX) && !defined(TVOSf_Linux) && !defined(TVOSf_Solaris)
  #ifdef Uses_string
   CFunc char *strlwr(char *s);
   CFunc char *strupr(char *s);
  #endif
  #ifdef Uses_filelength
   #define filelength CLY_filelength
  #endif
  #ifdef Uses_fixpath
   CFunc void _fixpath(const char *in, char *out);
  #endif
  #ifdef Uses_HaveLFNs
   #define OS_HaveLFNs
  #endif
  #ifdef Uses_glob
   #define Include_glob
  #endif
  #ifdef Uses_fnmatch
   #define Include_fnmatch
  #endif
  #ifdef Uses_regex
   #define Include_sys_types
   #define Include_regex
  #endif
  #ifdef Uses_getopt
   #define Include_getopt
  #endif
  #ifdef Uses_utime
   #define Include_utime
  #endif
  #ifdef Uses_mkstemp
   #define Include_stdio
  #endif
  #ifdef Uses_getcwd
   #define Include_unistd
  #endif
  #ifdef Uses_itoa
   CFunc char *itoa(int value, char *string, int radix);
  #endif
  #ifdef Uses_direct
   #define Include_direct
  #endif
  #ifdef Uses_dir
   #define Include_dir
  #endif
  #ifdef Uses_nl_langinfo
   #define Uses_CLY_nl_langinfo
  #endif
 #endif
#endif // TVComp_GCC



// BC++ 5.5 for Win32 is supported
#ifdef TVComp_BCPP
 #define CLY_UseCrLf 1
 #define CLY_HaveDriveLetters 1
 #define CLY_Packed
 /* Simple Boolean type */
 enum Boolean { False, True };
 #ifdef Uses_string
  #define Include_string
  #define strncasecmp strnicmp
  #define strcasecmp  stricmp
 #endif
 #ifdef Uses_abort
  #define Include_stdlib
 #endif
 #ifdef Uses_limits
  #define Include_limits
 #endif
 #ifdef Uses_fcntl
  #define Include_fcntl
 #endif
 #ifdef Uses_sys_stat
  #define Include_sys_stat
 #endif
 #ifdef Uses_unistd
  #define Include_cl_unistd
  // Most unistd equivalents are here:
  #define Include_io
 #endif
 #ifdef Uses_access
  #define Include_io
  #define R_OK 4
 #endif
 #ifdef Uses_ctype
  #define Include_ctype
 #endif
 #ifdef Uses_filelength
  #define Include_io
 #endif
 #ifdef Uses_getcurdir
  #define Include_dir
 #endif
 #ifdef Uses_AllocLocal
  #define AllocLocalStr(s,l) char* s = (char*)alloca(l)
  #define AllocLocalUShort(s,l) ushort *s = (ushort*)alloca(sizeof(ushort) * (l))
  #define Uses_alloca
 #endif
 #ifdef Uses_alloca
  #define Include_malloc
 #endif
 #ifdef Uses_free
  #define Include_malloc
 #endif
 #define NEVER_RETURNS
 #define RETURN_WHEN_NEVER_RETURNS return 0
 #define __attribute__( value )
 #define DeleteArray(a) delete[] a
 #define PATHSEPARATOR ';'
 #define PATHSEPARATOR_ ";"
 #define DIRSEPARATOR '/'
 #define DIRSEPARATOR_ "/"
 #define CLY_IsValidDirSep(a) (a=='/' || a=='\\')
 #ifdef Uses_fixpath
  CFunc void _fixpath(const char *in, char *out);
 #endif
 // Checks for UNCs under NT provided by Anantoli Soltan
 CFunc int CLY_IsUNC(const char* path);
 CFunc int CLY_IsUNCShare(const char* path);
 #ifdef Uses_HaveLFNs
  #define OS_HaveLFNs
 #endif
 #ifdef Uses_glob
  #define Include_cl_glob
 #endif
 #ifdef Uses_fnmatch
  #define Include_cl_fnmatch
 #endif
 #ifdef Uses_regex
  #define Include_cl_regex
 #endif
 #ifdef Uses_getopt
  #define Include_cl_getopt
 #endif
 #ifdef Uses_io
  #define Include_io
 #endif
 #ifdef Uses_dirent
  #define Include_cl_dirent
 #endif
 #ifdef Uses_ftell
  #define Include_io
  #define Include_stdio
 #endif
 #ifdef Uses_stdlib
  #define Include_stdlib
 #endif
 #ifdef Uses_utime
  #define Include_utime
 #endif
 #ifdef Uses_mkstemp
  CFunc int mkstemp(char *_template);
 #endif
 #ifdef Uses_getcwd
  #define Include_dir
 #endif
 #ifdef Uses_itoa
  #define Include_stdlib
 #endif
 #ifdef Uses_direct
  #define Include_direct
 #endif
 #ifdef Uses_dir
  #define Include_dir
 #endif
 #ifdef Uses_strstream
  #define Include_strstream
 #endif
 #ifdef Uses_nl_langinfo
  #define Uses_CLY_nl_langinfo
 #endif

 // ifstream::getline behaves strangely in BC++
 // I take the gcc implementation, here is a replacement.
 #define IfStreamGetLine(istream,buffer,size) \
         CLY_IfStreamGetLine(istream,buffer,size)
 #ifdef Uses_IfStreamGetLine
  #define Uses_fstream
  #define Uses_CLY_IfStreamGetLine
 #endif

 #define CLY_OpenModeT      int
 #define CLY_StreamPosT     streampos
 #define CLY_StreamOffT     streamoff
 #define CLY_IOSSeekDir     ios::seek_dir
 #define CLY_FBOpenProtDef  0666
 #define CLY_NewFBFromFD(f) new filebuf(f)
 #define CLY_PubSetBuf(a,b) pubsetbuf(a,b)
 #undef  CLY_HaveFBAttach
 #define CLY_FBOpen(a,b,c)  open(a,b,c)
 #define CLY_IOSBin         ios::binary
 #define CLY_PubSeekOff     pubseekoff
 #define CLY_PubSync        pubsync
 #define CLY_std(a)         a
 #define CreateStrStream(os,buf,size) char buf[size]; \
                                      ostrstream os(buf,sizeof(buf))
 #define GetStrStream(buf) buf
 #ifdef Uses_StrStream
  #define Include_strstream
 #endif
 #define UsingNamespaceStd
#endif



// MSVC will be supported if volunteers tests it or Microsoft decides to
// give it for free ;-). After all Borland released BC++ 5.5.
#if (defined(_MSVC) || defined(__MSC_VER)) && !defined(_MSC_VER)
#define _MSC_VER
#endif

#ifdef TVComp_MSC
 #define CLY_UseCrLf 1
 #define CLY_HaveDriveLetters 1
 #define CLY_Packed
 /* Simple Boolean type */
 enum Boolean { False, True };
 #ifdef Uses_string
  #define Include_string
  #define strncasecmp strnicmp
  #define strcasecmp  stricmp
 #endif
 #ifdef Uses_abort
  #define Include_process
 #endif
 #ifdef Uses_limits
  #define Include_limits
 #endif
 #ifdef Uses_fcntl
  #define Include_fcntl
 #endif
 #ifdef Uses_sys_stat
  #define Include_sys_stat
  #define Fake_S_IS
 #endif
 #ifdef Uses_unistd
  #define Include_unistd
 #endif
 #ifdef Uses_access
  #error Where is access defined?
 #endif
 #ifdef Uses_ctype
  #define Include_ctype
 #endif
 #ifdef Uses_filelength
  #define filelength _filelength
  #define Include_io
 #endif
 #ifdef Uses_AllocLocal
  #define AllocLocalStr(s,l) char* s = (char*)alloca(l)
  #define AllocLocalUShort(s,l) ushort *s = (ushort*)alloca(sizeof(ushort) * (l))
  #define Uses_alloca
 #endif
 #ifdef Uses_alloca
  #define Include_malloc
 #endif
 #ifdef Uses_free
  #define Include_malloc
 #endif
 #ifdef Uses_getcurdir
  #define getcurdir CLY_getcurdir
 #endif
 #define NEVER_RETURNS
 #define RETURN_WHEN_NEVER_RETURNS return 0
 #define __attribute__( value )
 #pragma warning( disable : 4250 )
 // SET: MSVC have a non-standard delete[] support. It doesn't follow last
 // standard. And which is worst doesn't understand it.
 // Vadim Beloborodov pointed out this missfeature.
 #define DeleteArray(a) delete (void *)a
 #define FA_ARCH   0x01
 #define FA_DIREC  0x02
 #define FA_RDONLY 0x04
 #define PATHSEPARATOR ';'
 #define PATHSEPARATOR_ ";"
 // SET: Not sure why Vadim wanted it. Perhaps to make the port more usable
 // for users accustomed to back slashes, but then the library isn't uniform
 // across platforms
 #define DIRSEPARATOR '\\'
 #define DIRSEPARATOR_ "\\"
 #define CLY_IsValidDirSep(a) (a=='/' || a=='\\')
 #ifdef Uses_fixpath
  CFunc void _fixpath(const char *in, char *out);
 #endif
 // Not yet supported for Win9x with MSVC
 #define CLY_IsUNCShare(a) (0)
 #define CLY_IsUNC(a)      (0)
 #ifdef Uses_HaveLFNs
  #define OS_HaveLFNs
 #endif
 #ifdef Uses_glob
  #define Include_cl_glob
 #endif
 #ifdef Uses_fnmatch
  #define Include_cl_fnmatch
 #endif
 #ifdef Uses_regex
  #define Include_cl_regex
 #endif
 #ifdef Uses_getopt
  #define Include_cl_getopt
 #endif
 #ifdef Uses_io
  #define Include_io
 #endif
 #ifdef Uses_dirent
  #define Include_dirent
 #endif
 #ifdef Uses_ftell
  #define Include_stdio
 #endif
 #ifdef Uses_utime
  #define Include_utime
 #endif
 #ifdef Uses_mkstemp
  CFunc int mkstemp(char *_template);
 #endif
 #ifdef Uses_getcwd
  #define Include_io
 #endif
 #ifdef Uses_itoa
  #define Include_stdlib
 #endif
 #ifdef Uses_direct
  #define Include_direct
 #endif
 #ifdef Uses_dir
  #define Include_dir
 #endif
 #ifdef Uses_strstream
  #define Include_strstrea
 #endif
 #ifdef Uses_nl_langinfo
  #define Uses_CLY_nl_langinfo
 #endif

 // ifstream::getline behaves strangely in BC++
 // I take the gcc implementation. I hope MSVC is like it.
 #define IfStreamGetLine(istream,buffer,size) \
         istream.getline(buffer,size)
 #define CLY_OpenModeT      int
 #define CLY_StreamPosT     streampos
 #define CLY_StreamOffT     streamoff
 #define CLY_IOSSeekDir     ios::seek_dir
 #define CLY_FBOpenProtDef  filebuf::openprot
 #define CLY_NewFBFromFD(f) new filebuf(f)
 #define CLY_PubSetBuf(a,b) setbuf(a,b)
 #define CLY_HaveFBAttach
 #define CLY_FBOpen(a,b,c)  open(a,b,c)
 #define CLY_IOSBin         ios::binary
 #define CLY_PubSeekOff     seekoff
 #define CLY_PubSync        sync
 #define CLY_std(a)         a
 #define CreateStrStream(os,buf,size) char buf[size]; \
                                      ostrstream os(buf,sizeof(buf))
 #define GetStrStream(buf) buf
 #ifdef Uses_StrStream
  #define Include_strstrea
 #endif
 #define UsingNamespaceStd
#endif

#ifdef Uses_IOS_BIN
 #define IOS_BIN CLY_IOSBin
#endif

CFunc void CLY_YieldProcessor(int micros);
CFunc void CLY_ReleaseCPU();
/* Return the number of ticks (on MSDOS 1 tick is 1/18 sec),
   this is used to compute the double click */
CFunc unsigned short CLY_Ticks(void);
/* An utility to split directory and file components of a path:
   Extracts from path the directory part and filename part.
   if 'dir' and/or 'file' == NULL, it is not filled.
   The directory will have a trailing slash. */
CFunc void CLY_ExpandPath(const char *path, char *dir, char *file);
/* An utility function. It makes the path an absolute one and replaces the
   original value passed. Should use fixpath. */
CFunc void CLY_fexpand(char *rpath);
#define fexpand(a) CLY_fexpand(a)
/* Utility function to know if a drive letter is valid. */
CFunc int  CLY_DriveValid(char drive);
#ifdef __cplusplus
#define driveValid(a) (CLY_DriveValid(a) ? True : False)
#endif
/* Utility function to know is a file is in fact a directory */
CFunc int CLY_IsDir(const char *str);
#ifdef __cplusplus
#define isDir(a) (CLY_DriveValid(a) ? True : False)
#endif
/* Utility function to know if a path is valid (existing directory or file) */
CFunc int CLY_PathValid(const char *path);
#ifdef __cplusplus
#define pathValid(a) (CLY_PathValid(a) ? True : False)
#endif
/* Utility function to know if a file name is valid to create a new file */
CFunc int CLY_ValidFileName(const char *fileName);
#ifdef __cplusplus
#define validFileName(a) (CLY_ValidFileName(a) ? True : False)
#endif
/* Utility function to know the current directory including a trailing
   slash */
CFunc void CLY_GetCurDirSlash(char *dir);
#define getCurDir(a) CLY_GetCurDirSlash(a)
/* Utility function to know if a path contains a supported wildcard */
CFunc int CLY_IsWild(const char *f);
#ifdef __cplusplus
#define isWild(a) (CLY_IsWild(a) ? True : False)
#endif
/* Utility function to know if a file exist and we can read from it */
CFunc int CLY_FileExists(const char *fname);
#ifndef TVCompf_djgpp
#define __file_exists(a) CLY_FileExists(a)
#endif
/* Utility function to know if a path is relative */
CFunc int CLY_IsRelativePath(const char *path);
#ifdef __cplusplus
#define relativePath(a) (CLY_IsRelativePath(a) ? True : False)
#endif
/* Utility function to generated a beep */
CFunc void CLY_Beep(void);
/* Used internally, just call filelength prior request */
CFunc long CLY_filelength(int);
/* Used internally, just call getcurdir prior request */
CFunc int  CLY_getcurdir(int drive, char *buffer);

/* cl/unistd.h includes dir.h */
#ifdef Include_cl_unistd
 #define Include_dir
#endif

#ifdef Uses_ifsFileLength
 #define Uses_fstream
#endif

#ifdef Include_sys_types
 #include <sys/types.h>
#endif

#ifdef Include_string
#include <string.h>
#endif

#ifdef Include_limits
 #include <limits.h>
 #ifndef PATH_MAX // BC++ and MSVC
  #define PATH_MAX 512
 #endif
#endif

#ifdef Include_fcntl
 #include <fcntl.h>
 #ifndef O_TEXT // UNIX
  #define O_TEXT (0)
 #endif
 #ifndef O_BINARY // UNIX
  #define O_BINARY (0)
 #endif
#endif

#ifdef Include_sys_stat
 #include <sys/stat.h>
 #ifdef Fake_S_IS
  #define S_ISDIR(m)  ((m) & S_IFDIR)
  #define S_ISCHR(m)  ((m) & S_IFCHR)
  #define S_ISBLK(m)  ((m) & S_IFBLK)
  #define S_ISREG(m)  ((m) & S_IFREG)
  #define S_ISFIFO(m) ((m) & S_IFIFO)
 #endif
#endif

#ifdef Include_stdlib
 #include <stdlib.h>
#endif

#ifdef Include_malloc
 // BC++ and MSVC defines alloca here
 #include <malloc.h>
#endif

#ifdef Include_io
 #include <io.h>
#endif

#ifdef Include_direct
 #include <direct.h>
#endif

#ifdef Include_dir
 #include <dir.h>
#endif

#ifdef Include_langinfo
 #include <langinfo.h>
 // Workaround a bug in glibc
 #if !defined(RADIXCHAR) && defined(DECIMAL_POINT)
  #define RADIXCHAR DECIMAL_POINT
 #endif
#endif


#ifdef Include_ctype
 #include <ctype.h>
 /* The following macros are defined to avoid passing negative values to
    the ctype functions in the common case: isxxxx(char).
    They are suitable for x86 DOS and Linux, perhaps not for others */
 #define ucisalnum(a)  isalnum((unsigned char)a)
 #define ucisalpha(a)  isalpha((unsigned char)a)
 #define ucisascii(a)  isascii((unsigned char)a)
 #define uciscntrl(a)  iscntrl((unsigned char)a)
 #define ucisdigit(a)  isdigit((unsigned char)a)
 #define ucisgraph(a)  isgraph((unsigned char)a)
 #define ucislower(a)  islower((unsigned char)a)
 #define ucisprint(a)  isprint((unsigned char)a)
 #define ucispunct(a)  ispunct((unsigned char)a)
 #define ucisspace(a)  isspace((unsigned char)a)
 #define ucisupper(a)  isupper((unsigned char)a)
 #define ucisxdigit(a) isxdigit((unsigned char)a)
 #define uctoascii(a)  toascii((unsigned char)a)
 #define uctolower(a)  tolower((unsigned char)a)
 #define uctoupper(a)  toupper((unsigned char)a)
#endif

#ifdef Include_unistd
 #include <unistd.h>
 #ifndef R_OK
  #define R_OK 4
 #endif
#endif

#ifdef Include_process
 #include <process.h>
#endif

#ifdef Include_cl_unistd
 #include <cl/unistd.h>
#endif

#ifdef Include_glob
 // POSIX
 #include <glob.h>
#endif

#ifdef Include_cl_glob
 // Replacement
 #include <cl/glob.h>
#endif

#ifdef Include_fnmatch
 // POSIX
 #include <fnmatch.h>
#endif

#ifdef Include_cl_fnmatch
 // Replacement
 #include <cl/fnmatch.h>
#endif

#ifdef Include_sys_types
 #include <sys/types.h>
#endif

#ifdef Include_regex
 // POSIX
 #include <regex.h>
#endif

#ifdef Include_cl_regex
 // Replacement
 #include <cl/regex.h>
#endif

#ifdef Include_getopt
 #include <getopt.h>
#endif

#ifdef Include_cl_getopt
 #include <cl/getopt.h>
#endif

#ifdef Include_io
 #include <io.h>
#endif

#ifdef Include_stdio
 #include <stdio.h>
#endif

#ifdef Include_dirent
 #include <dirent.h>
#endif

#ifdef Include_cl_dirent
 #include <cl/dirent.h>
#endif

#ifdef Include_time
 #include <time.h>
#endif

#ifdef Include_utime
 #include <utime.h>
#endif

#ifdef Include_cl_utime
 #include <cl/utime.h>
#endif

#ifdef Include_strstream
 #include <strstream.h>
#endif

#ifdef Include_strstrea
 #include <strstrea.h>
#endif

#ifdef Include_sstream
 #include <sstream>
#endif

#ifdef Uses_fstream
 #include FSTREAM_HEADER
#endif

#ifdef Uses_iomanip
 #include IOMANIP_HEADER
#endif

#ifdef Uses_iostream
 #include IOSTREAM_HEADER
#endif

#if defined(Uses_CLYFileAttrs) && !defined(Uses_CLYFileAttrsDef)
#define Uses_CLYFileAttrsDef
/* Equivalent to mode_t */
#ifdef CLY_Have_UGID
// In systems with User and Group ID the mode is an structure
typedef struct
{
 mode_t mode,writemask;
 uid_t  user;
 gid_t  group;
} CLY_mode_t;
#else
typedef unsigned int CLY_mode_t;
#endif
/* Utility function to find the attributes of a file. You must call stat
   first and pass the st_mode member of stat's struct in statVal. */
CFunc void CLY_GetFileAttributes(CLY_mode_t *mode, struct stat *statVal, const char *fileName);
/* The reverse. The file must be closed! */
CFunc int CLY_SetFileAttributes(CLY_mode_t *newmode, const char *fileName);
/* This function alters mode content so the attribute indicates that the
   owner of the file can't read from it */
CFunc void CLY_FileAttrReadOnly(CLY_mode_t *mode);
/* This function alters mode content so the attribute indicates that the
   owner of the file can read from it */
CFunc void CLY_FileAttrReadWrite(CLY_mode_t *mode);
/* Returns !=0 if the file is read-only */
CFunc int  CLY_FileAttrIsRO(CLY_mode_t *mode);
/* Sets the attribute that indicates the file was modified */
CFunc void CLY_FileAttrModified(CLY_mode_t *mode);
/* It returns a mode that can be used for a newly created file */
CFunc void CLY_GetDefaultFileAttr(CLY_mode_t *mode);
#endif

/* Returns the name of the shell command */
CFunc char *CLY_GetShellName(void);

#ifdef Uses_ifsFileLength
extern long CLY_ifsFileLength(CLY_std(ifstream) &f);
#endif

#ifdef Uses_CLY_IfStreamGetLine
extern int CLY_IfStreamGetLine(CLY_std(ifstream) &is, char *buffer, unsigned len);
#endif

/* Internal definition of nl_langinfo */
#ifdef Uses_CLY_nl_langinfo
typedef int nl_item;
#define CURRENCY_SYMBOL   0 /*currency_symbol*/
#define CRNCYSTR          0 /*currency_symbol*/    /*deprecated*/
#define DECIMAL_POINT     1 /*decimal_point*/
#define RADIXCHAR         1 /*decimal_point*/      /*deprecated*/
#define GROUPING          2 /*grouping*/
#define INT_CURR_SYMBOL   3 /*int_curr_symbol*/
#define MON_DECIMAL_POINT 4 /*mon_decimal_point*/
#define MON_GROUPING      5 /*mon_grouping*/
#define NEGATIVE_SIGN     6 /*negative_sign*/
#define POSITIVE_SIGN     7 /*positive_sign*/
#define THOUSANDS_SEP     8 /*thousands_sep*/
#define THOUSEP           8 /*thousands_sep*/      /*deprecated*/
/*char*/
#define FRAC_DIGITS       9 /*frac_digits*/
#define INT_FRAC_DIGITS  10 /*int_frac_digits*/
#define N_CS_PRECEDES    11 /*n_cs_precedes*/
#define N_SEP_BY_SPACE   12 /*n_sep_by_space*/
#define P_SIGN_POSN      13 /*p_sign_posn*/
#define N_SIGN_POSN      14 /*n_sign_posn*/
#define P_CS_PRECEDES    15 /*p_cs_precedes*/
#define P_SEP_BY_SPACE   16 /*p_sep_by_space*/

#define nl_langinfo CLY_nl_langinfo
CFunc char *CLY_nl_langinfo(nl_item item);
#endif

#ifdef TV_BIG_ENDIAN
// Most RISC machines
#define CLY_High16(a)  ((a) & 0xFF)
#define CLY_Low16(a)   ((a) >> 8)
#else
// Intel machines
#define CLY_High16(a)  ((a) >> 8)
#define CLY_Low16(a)   ((a) & 0xFF)
#endif

#ifdef DJGPP_HaveLFNs
inline
int CLY_HaveLFNs()
{
 return _use_lfn(0);
}
#endif
#ifdef OS_HaveLFNs
inline
int CLY_HaveLFNs()
{
 return 1;
}
#endif

// Is that an EOL char?
// Ask for no
#define CLY_IsntEOL(a) (a!='\r' && a!='\n')
// Ask for yes
#define CLY_IsEOL(a)   (a=='\r' || a=='\n')

#ifdef CLY_UseCrLf
 #define CLY_LenEOL     2
 // This asks if the EOL is really usable for the OS
 #define CLY_IsTrueEOL(a) (a=='\r' || a=='\n')
 // String containing EOL
 #define CLY_crlf "\r\n"
#else
 #define CLY_LenEOL     1
 #define CLY_crlf "\n"
 #define CLY_IsTrueEOL(a) (a=='\n')
#endif

#endif // CLY_CompatLayerIncluded

