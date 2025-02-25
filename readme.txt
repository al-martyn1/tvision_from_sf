Hi!

The small sections are:

0.  Ultra quick instructions
1.  Introduction
2.  Notes about the package
3.  Compilation and use
4.  Linux dynamic libraries
5.  Libraries and tools needed
6.  Examples
7.  Targets supported, limitations
8.  CPU usage
9.  How to submit a patch
10. Special thanks
11. Contact information

If you are looking for information about the Win32 target please look in the
win32 (MingW) directory and winnt directory (BC++).

****** Important! for old users: the headers now have two levels. In ******
****** the first level tv.h is located. The rest of the headers are  ******
****** located in a subdirectory called tv. It shouldn't affect your ******
****** program.                                                      ******

****** Important! for old users: the library generated is now called ******
****** librhtv.a this change in the name is to avoid confusion with  ******
****** other ports of the library.                                   ******

0.  Ultra quick instructions
----------------------------

  If you can't wait even a second and you are running Linux or have a very
well equiped djgpp installation (Perl, shell utils, file utils, bash,
make, etc.) you can just follow these steps:

A) Run the configure script (./configure or configure).

Examples:
   linux:/usr/src/tvision# ./configure
   c:\djgpp\contrib\tvision>configure

B) If all is OK run "make".
C) If all is OK just run "make install".

  With it you'll get the headers and library installed.
  If the configure script makes a wrong guess about something and you want to
run it again delete the configure.cache file to force a new fresh
configuration.
  If the installation prefix directory is wrongly guessed call the configure
script passing --prefix=dir as argument.
Example:
  linux:/usr/src/tvision# ./configure --prefix=/usr/local
  
  Note: The configuration mechanism can be used for Win32+MingW but you must
install some GNU tools.




1. Introduction:
---------------

  That's the Turbo Vision distribution (TV for short) v1.1.0.
  The sources are copyrighted by Borland (currently Inprise) and are freely
available in internet from:

ftp://ftp.inprise.com/pub/borlandcpp/devsupport/archive/turbovision/tv.zip

  Look in the borland.txt file for more information. Additionally here is
what Inprise says about this package:

URL: http://www.inprise.com/devsupport/bcppbuilder/faq/QNA906.html
------------------------------------------------------------------------------
Question and Answer Database

QNA        :Turbo Vision
Category   :C/C++ Language Issues
Platform   :Windows95
Product   :C++Builder  ALL

Question:
Where can I find the public domain version of Turbo Vision?

Answer:
It can be found at
ftp.inprise.com/pub/borlandcpp/devsupport/archive/turbovision/

the turbo vision libraries are in the tv.zip archve, the rest of the 
files are examples.

7/2/1998 10:32:32 AM
------------------------------------------------------------------------------
  So according to Inprise web tv.zip is a Public Domain version.
<Clarification>
  I added this small text because some people have doubts about copyrights
issues.
</Clarification>

  They were heavily patched to work with gcc. Some patches fixes bugs, others
changes the behavior of things limited to 16 bits values, the resulting
TV isn't 100% compatible with the original. See the examples/porting.txt
file for more information, I explain which differences I found while porting
the examples found there.
  Up to release 1.0.1 most of the patches were made by Robert Hoehne. I
contributed with various patches and modules mainly to extend the original
functionality. For information about what changed before it just read the
change.log file.
  The library is distributed under the GPL license, it seems to be OK because
according to a FAQ in the Inprise web site the base library is Public
Domain. The file copying contains the GPL license and the file copying.rh
contains the RHIDE copyright.

  The purpose of this distribution is avoid downloading the whole RHIDE to
use TV. I'm doing the distribution because Robert doesn't have enough time
to do it. Additionally this distribution will make RHIDE's compilation much
more simple for people don't familiarized with it.

  Recent news: Richard M. Stallman contacted me and put some doubts about the
distribution of Borland's sources, he contacted somebody at Borland asking if
TV was freesoftware and he got no as an answer. I think this reply was mainly
because RMS asked without clarifying your point. To clarify it I contacted
the person in charge of the Linux web section of Borland and he told me that
he is redirecting these doubts to the right people inside Borland. Until now
I didn't get a final answer.




2. Notes about the package:
--------------------------

  As you are reading it you already unpacked it but here are some notes:
(I usually read the readmes without unpacking)
  The files that must be in DOS format are listed in lista.dos, the rest are
binary or can be UNIX files because djgpp tools handles these UNIX files very
well.
  If you take a look at the tree you'll see I included both, the DOS and
Linux, makefiles and .gpr files. That's because the source distribution is
basically the same for DOS and Linux. If you have the Linux distribution and
you want to use it under DOS just unpack the files and run the fromunix.pl
Perl script. The reverse is valid: the DOS package can be used under Linux
running the fromdos.pl Perl script.
  Starting with 1.0.5 I include the Debian package files. As these files
aren't 8+3 clean if you uncompress the files in plain DOS you'll get some
warnings about files with the same name, so I removed these files from the
DOS distribution in 1.0.6, now they are only in the Linux distribution. So
if you want to try generating the .deb packages you *must* have the Linux
distribution of the library.
  The compress and compress.bat files are used to create the distribution
packages and are included.
  Starting with 1.1.0 Win32 platforms are also supported using MingW and BC++
5.5 compilers.




3. Compilation and use:
----------------------

  You can try the GNU's traditional mechanism for compilation: configure,
make and finally make install, as explained in point 0. Start running
configure script. If you want to do changes to the library you should read
the rest of the chapter, but if you just want to compile and install the
library the configure/make mechanism should be enough. If you are using DOS
and have the last release of RHIDE and don't have all the GNU tools installed
you could also need to do some stuff by hand, so continue reading. To use the
configure script you'll need (at least): make, shell utils, file utils and
perl; and perhaps even more tools.

  Compilation 'by hand':
  To compile the library go to the directory for your system (djgpp or linux)
and run rhide or make in this directory.
  All the .o files are stored in the obj directory it helps to delete or
compress these files if you want to save disk space and you won't be
modifying the sources all the time.

  To use the library add the include directory in your includes path and the
directory where the library was created to your libraries path (or just move
the library to the lib directory). I recommend using an environment variable
pointing to the base directory where the files are located and add the rest
to it. For example:
TVISION=e:/dj/contrib/tvision
Then $(TVISION)/include is the include directory and $(TVISION)/linux or
$(TVISION)/djgpp is the library path.
  *Note* In this way you don't need to install the library and you can make
changes in the library without needing to reinstall all the time.

  I use gcc 2.8.0 under DOS so the library is by default compiled with
-fno-exceptions and -fno-rtti so you should use these settings. If you
don't like these settings change it before compiling or rebuild the library
with your favorite settings.
  In Linux I use gcc 2.7.2.3 and hence -fno-exceptions isn't needed and is
rejected by gcc too. So if you compile the library for Linux using 2.8.x you
could want to change it.

<Clarification>
  *** Important *** the above sentences seems to be not enough so here I
clarify it: code compiled with -fno-exceptions and -fno-rtti doesn't have the
hooks needed for C++ exceptions in the classes so you can't link this code
with code compiled with gcc 2.8.x if this code wasn't compiled with
-fno-exceptions and -fno-rtti. Why we use it? (1) We don't use C++ exceptions
mechanism (2) It adds unnecesary code and data (3) libraries compiled with
these hooks using 2.8.x can't be linked with objects from 2.7.x.
  So, if you use gcc 2.8.x or newer compiler and want to use the precompiled
library you must compile your code with -fno-exceptions and -fno-rtti.
</Clarification>

  The library is compiled with -O2 as optimization, that's good enough for
this kind of code and any processor.
  The library contains debug information, the default settings use -gstabs3,
it works very well and isn't much greater than -g. If you want to delete
the debug information you can do it running the sacadeb batch or script. It
will rename the original library to libtvdb.a and create a new one without
debug information.




4. Linux dynamic libraries:
--------------------------

  The configure/make process generates it, but if you must do it by hand:

From sources:
  To create the .so library go to the linuxso directory, run makemak.pl
(yes, you'll need Perl for it, that's standard in Debian) and then edit and
run the instlib installation script.

From binary distribution:
  Edit and run the instlib script.

  That's very experimental so be careful I don't know if the version numbers
are OK and what will happend if you try to execute something created with
the 1.0.1 library using the 1.0.2 library. If you have hints about this
topic please contact me. Note: as a precaution I'm changing the soname of the
library with each release.
  My tests shows that a small TV example (examples/dyntxt) is only 14Kb
long when linked dynamically and without debug information. But you should
know that dynamically linked executables are slower than static ones,
specially when you call a lot of small and fast library functions, that's
because the address resolution overhead.




5. Libraries needed:
-------------------

Linux:
  In Linux you'll need ncurses (I use 3.4 and 4.2) and gpm (I use 1.3 and
1.17.8). If you are using libc5 you'll need gettext too (libintl) that isn't
needed for glibc (I use 2.0.7 and 2.1.2).
  Note that the configure script will check if you have the right libraries
installed. I don't know if the script is too strict, let me know if you think
it.

DOS:
  You must install gettext, you can find it in the v2gnu directory in
Simtelnet. Is called (for example) gtxt032b.zip. There are a bug in the
headers of gtxt032b.zip, in the file libintl.h there are a line that says
@INCLUDE_LOCALE_H@, you can safetly remove it. An alternative is comment the
line that says HAVE_INTL_SUPPORT in the configtv.h file. The configure script
will detect if you have a working gettext installed and modify the configtv.h
accordingly.

Tools:
Compiler: I tested with gcc 2.7.x, 2.8.x, egcs 1.1.x and gcc 2.95.x.
Binutils: I use binutils 2.8.1, newers should work.
RHIDE: if you plan to use the project (.gpr) files you'll need the last
version of RHIDE available. Any older version will crash when loading the
projects.
make: (only if you plan to use the makefiles) I use make 3.78.1 under DOS and
3.76.1 under Linux.
Perl: (only if you need to run some special scripts, like the configure) Any
perl 5 should work, I use 5.004 currently.
File utils: (I'm not sure if that's needed if you don't use the configure
script but just in case) you need to have it installed if you are in DOS,
Linux of course have it by default.




6. Examples:
-----------

  I included a collection of 13 examples I got from the net and ported to
this TV. The examples are shipped only with the sources distribution.




7. Targets supported, limitations:
---------------------------------

  The library compiles and runs in DOS and Linux. Only i386 Linux is fully
supported, we are working to support Alpha too, currently I think you can
get it working if you compile the library without warnings enabled, that's
needed because there are errors in system.h file for Alpha (an invalid
pointer convertion) that are reported only by C++ and not C, looks like
nobody includes these headers in C++ programs. Another problem I know about
Alpha is that egcs 2.91.66 generates some silly warnings, I think they are
due to some bug in egcs, removing the -Werror switch you'll get a working
library. Dim Zegebart <zager@post.comstar.ru> is the one testing the Alpha
stuff.
  There are some important things to note, specially what is supported in
each case:

[Sorted alphabetically]

DOS:
 Keyboard:
   Almost all the keyboard combinations are supported. Some keyboard
  combinations aren't reported by BIOS, they are:
  Control + `1457890=;,./
  Macro key (I have it in one of my keyboards)
   To workaround it I created a small TSR (hacking another TSR and rewriting
  the code inside the ISR ;-). This TSR is included with the sources
  distribution in the extra/doskeys directory, you can distribute it with
  your program if you want, just don't remove my copyright.
 Screen:
   Any text mode fully supported by BIOS and that is compatible with mode 3
  (VGA color) is supported, VESA modes are supported too, if you have
  problems please report it. If you use a program that sets the video mode
  by tweaking the VGA registers it will also set the BIOS data area to
  reflect the new screen size or it won't work.
   Additionally the library supports some build in modes:
  82x25, 80x28, 80x30, 90x30, 94x30, 80x34, 90x34, 94x34, 80x35, 80x40,
  80x43 and 80x50.
   Note: if you wonder why 82x25 that's a mode where the characters have 8
  pixels of width and is very good for fonts with strange layouts like
  ISO-Latin-1.
   The hercules monochrome mode should be supported but I didn't test it for
  a long time so perhaps something is broken, please report your experience
  if you try it.
 Mouse
   Full support for drivers compatible with MSMouse v6.0.

Linux console for users with access to /dev/vcs* devices:
  Keyboard:
    If you are running in the console (ttyN, not ttypN for example) the
   support is excellent. The library uses TIOCLINUX function #6 to get the
   Alt, Ctrl, Shift, etc. and ncurses for the keys. Combining this
   information almost anything is supported. As some keys are used by Linux
   kernel the library patchs the keyboard tables to fool kernel, so things
   like Alt+Fn won't change console, or Shift+PgUp won't scroll up. To
   switch consoles use Ctrl+Alt+Fn wich, by the way, is the combination used
   by X.
    An interesting limitation is that Ctrl+I is Tab and you can't avoid it.
  Screen:
    You need at least write access to the /dev/vcs* devices, if not you'll
   lose performance and some characters. The best is having read and write
   access, if you only have write access TScreen::suspend/resume doesn't
   work properly.
    The current video mode is used, no functions to change the video mode are
   provided. Currently the library is limited to the standard fonts (VGA)
   I'm working to fix it. The library supports 16 foreground and 16
   background colors.
  Mouse:
    Full support for gpm 1.x, pressing Alt+mouse releases the mouse to gpm.
   It alows copying from a Turbo Vision application to other application, but
   not the reverse, why? because gpm will simply type the keys and as you are
   holding Alt all will be received as Alt+key. But there are a solution ;-)
   if you configure the library to use the left alt key for the menues
   (see TGKey::SetAltSettings(value)) you can use the right alt for the
   mouse paste.

Linux using telnet:
  Keyboard:
    It means when you are running in a remote mode (ttypN for example), note
   that midnight commander runs the applications in this way!
    In this case you are at the mercy of ncurses wich uses some arcaic
   standards. You'll lose: Shift+Special keys and Ctrl+Special keys (and
   more). Special keys are arrows, home, end, insert, etc. This limitation is
   in the protocols and standards used and I can't do anything to workaround
   it, the system is broken.
  Screen:
    The support is slow (1/3.5 compared with vcsN) and if you are using a
   network the speed is of course limited by the transmision, wich could
   make the application too slow. As not all the characters are supported
   some things like the arrows of the scroll bars will be replaced by ASCII
   values. 16 foreground and 8 background colors.
  Mouse:
    Not supported, don't know if possible, in fact kmous is not defined in
   the terminfo file for Linux.

Linux using xterm: (see also Eterm section!!)
  Keyboard:
    The same limitations that "Linux using telnet" because xterms and ncurses
   can't do more.
  Screen:
    Good support, the library supports SIGWINCH so changing the size of the
   window will resize the application. 8 foreground and 8 background colors
   are supported, but using bold/normal the library gets 16 foreground
   colors.
  Mouse:
    Supported, lamentably isn't very interactive because xterm only reports
   when you press or release a button, but not mouse movement. You must get
   acustomed, but works.

Linux using Eterm: (an excellent replacement for xterm)
    In addition to the things supported for xterm all the keyboard
   combinations are detected correctly. To get it working you must setup some
   things see in the extra/eterm directory.

Windows 3.1: Currently I don't test under this platform, tell me if you have
             problems, I tested in the past and worked OK. Most of the W9x
             restrictions applies.

Windows 9x:
  * Important * A native Win32 port is available, see the win32 and winnt
                directories. When using any of these two ports the platform
                is fully supported.
  Keyboard:
    Almost the same as DOS so read the DOS section first. The TSR works at
   least in Windows '95. Some keys are used by the GUI so not all are
   available I include a .pif example showing how to avoid W9x take control
   of Alt+Space and other keys. The example is in extra/pifexamp.
  Screen:
    The support is similar to the one found in DOS, but I know some video
   drivers have bugs in the VESA handling so perhaps it could make problems.
   If you face such a problem please contact me, in the past I fixed VESA
   problems with S3 boards thanks to the help of a user.
    Usually isn't a good idea to run applications that change the video mode
   inside a window, so if your program does it recommend not running in a
   window to your users. One way to avoid it is using a .pif file, it will
   prevent 98% of the people from "windowizing" the program. See the
   extra/pifexamp example for it, it instructs W95 to run the program in full
   screen and disable Alt+Enter, the example also gives up to 64Mb to the
   application (the maximun W95 will give to a DOS task).
  Mouse:
    Full support.
  Note: I think all of it applies to Windows 98, if you see any difference
        please tell me.

Windows NT:
  * Important * A native Win32 port is available, see the win32 and winnt
                directories. When using any of these two ports the platform
                is fully supported.
  This platform isn't fully supported by the DOS target. I think NT's support
  for DOS applications is completly broken so I don't care about it. If you
  have patches I'll receive it.




8.  CPU usage:
-------------

  Since v1.0.8 the TProgram::idle() member releases the CPU to the OS. If for
some reason you want to eat 100% of the CPU or you want to use a methode
different than the used by this function just set TProgram::doNotReleaseCPU
to 1 and the class won't release the CPU.
  For Linux I just do a usleep(1000), that's sleep for 1 ms. For djgpp I call
to the __dpmi_yield() function. Note that you could use usleep for both
(DOS and Linux) but in this case you should sleep for more than 18.2 ms to
let djgpp's sleep really release the CPU. I think is easier to just call
__dpmi_yield() and explicitly release the CPU, in this way you sleep the
minimal amount of time.




9. How to submit a patch:
------------------------

  The simplest way is running a diff between your current directory and a
copy of the library as you downloaded it. Don't forget to specify the exact
version of the distribution and the purpose of the patchs. Use the unified
mode (-u), this mode is the best for humans ;-)




10. Special thanks:
------------------

They goes to (no particular sorting):

Robert Hoehne:
He started this port and invested a lot of time on it. Lamentably he doesn't
have time for it anymore. I respected all the Robert's copyright labels, even
when a lot of them doesn't apply anymore.

Vadim Beloborodov:
He contributed the first win32 port supporting MingW and MSVC.

Anatoli Soltan:
He contributed the winnt port supporting BC++. Now it can be used for win32
too.

Laurynas Biveinis:
He contributed a lot of patches.




11. Contact information:
-----------------------

Salvador E. Tropea (SET)
Curapaligue 2124
3 de Febrero, Caseros
(1678) Bs.As. Argentina

Phone: (+5411) 4759-0013

e-mail: salvador@inti.gov.ar, set@computer.org
http:   www.geocities.com/SiliconValley/Vista/6552/
        www.geocities.com/set-soft/ (alias of the above mentioned)



Enjoy, SET
