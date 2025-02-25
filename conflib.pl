#!/usr/bin/perl
# Copyright (C) 1999,2000 by Salvador E. Tropea (SET),
# see copyrigh file for details
#
# Common configuration routines.
#

$ErrorLog='errormsg.txt';
$MakeDefsRHIDE={};
$ExtraModifyMakefiles={};
$OSflavor='';

sub GetCache
{
 my $val;

 if (open(FIL,'<configure.cache'))
   {
    while ($val=<FIL>)
      {
       if ($val=~/(.*) => (.*)\n/)
         {
          @conf{$1}=$2;
          #print "$1 => $2\n";
         }
       else
         {
          #print "No toma: ($val)";
         }
      }
    close(FIL);
   }
}


sub CreateCache
{
 my ($i,$ff);

 $ff=1;
 if (open(FIL,'>configure.cache'))
   {
    foreach $i (%conf)
      {
       if ($ff)
         {
          print FIL ("$i => @conf{$i}\n");
          $ff=0;
         }
       else
         {
          $ff=1;
         }
      }
    close(FIL);
   }
}


sub CompareVersion
{
 my ($actual,$needed)=@_;
 my ($vact,$vneed);

 $actual=~/(\d+)\.(\d+)(\.(\d+))?/;
 $vact=$1*1000000+$2*1000+$4;
 $needed=~/(\d+)\.(\d+)(\.(\d+))?/;
 $vneed=$1*1000000+$2*1000+$4;
 #print "$vact v.s. $vneed\n";
 $vact>=$vneed;
}


sub LookForFile
{
 my ($file,@path)=@_;
 my ($i,@a,$f);

 foreach $i (@path)
   {
    #print "Testing for ".$i.'/'.$file."\n";
    @a=glob($i.'/'.$file);
    foreach $f (@a)
      {
       if (-e $f)
         {
          return $i;
         }
      }
   }
 '';
}

###[txh]####################################################################
#
# Prototype: RunGCCTest($gcc,$extension,$prog,$flags)
# Description:
#  Compiles a test program with gcc and runs it callecting the output.@*
#  You must pass the name of the compiler as argument, also the extension of
# the file (.c, .cc, etc.), the code to compile and any etra switch options
# needed to compile.@*
#  Additionally the following global variables are used: $CFLAGS: standard
# flags for the C compiler (used when extension=='c'), $CXXFLAGS: standard
# flags for the C++ compiler (used when extension=='cc'), $ErrorLog: file
# to log the actions and errors and the $OS guess.@*
#
# Return: The value returned from the program (from stdout).
#
####################################################################[txi]###

sub RunGCCTest
{
 my ($cc,$ext,$test,$switchs)=@_;
 my ($command,$label);
 my ($file,$flags)=('test.'.$ext,'');

 replace($file,$test);
 $flags=$CFLAGS if ($ext eq 'c');
 $flags=$CXXFLAGS if ($ext eq 'cc');
 $command="$cc -o test.exe $flags $file $switchs";
 #print "Running: $command\n";
 $label=$command.":\n";
 `echo $label >> $ErrorLog`;

 if ($OS eq 'DOS')
   {
    `redir -ea $ErrorLog $command`;
   }
 elsif ($OS eq 'Win32')
   {
    `sh -c "$command 2>> $ErrorLog"`;
   }
 else
   {
    `$command 2>> $ErrorLog`;
   }
 $test=`./test.exe`;
 unlink('test.o',$file,'test.exe');
 $test;
}

###[txh]####################################################################
#
# Prototype: LookForPrefix()
# Description:
#   Tries to find the right installation prefix. The following mechanisms
# are used:@*
# 1) Look for cached prefix key.@*
# 2) Look for prefix environment variable.@*
# 3) Look for PREFIX environment variable.@*
# UNIX:@*
# 4) Find where make is installed and guess from it.@*
# 5) If not found default to /usr@*
# DOS:@*
# 6) Use the DJDIR environment variable.@*
#   The result is stored in the prefix configuration key.
#
####################################################################[txi]###

sub LookForPrefix
{
 my ($test,$prefix);

 print 'Looking for prefix: ';
 $prefix=@conf{'prefix'};
 if ($prefix)
   {
    print "$prefix (cached/specified)\n";
    LookIfFHS();
    return;
   }
 $prefix=@ENV{'prefix'};
 if (!$prefix)
   {
    $prefix=@ENV{'PREFIX'};
   }
 if ($prefix)
   {
    print "$prefix (environment)\n";
    @conf{'prefix'}=$prefix;
    LookIfFHS();
    return;
   }
 if ($OS eq 'UNIX')
   {
    if (`which make`=~/(.*)\/bin\/make/)
      {
       $prefix=$1;
      }
    else
     {
      $prefix='/usr';
     }
   }
 elsif ($OS eq 'DOS')
   {
    $prefix=@ENV{'DJDIR'};
   }
 else
   {
    $test=`rm --help`;
    if ($test=~/(.*)\/BIN\/RM.EXE/)
      {
       $prefix=$1;
       if ($prefix=~/\/\/(\w)\/(.*)/)
         {
          $prefix="$1:/$2";
         }
       else
         {
          $prefix="c:/$prefix";
         }
      }
    else
      {
       if (!$test)
         {
          die "Some critical tools aren't installed please read the reame files";
         }
       else
         {
          print "Unable to determine installation prefix, please use --prefix option.\n";
          print "You'll most probably need to quote the --prefix option (\"--prefix=path\").\n" if @ENV{'OS'} eq 'Windows_NT';
          die;
         }
      }
   }
 @conf{'prefix'}=$prefix;
 print "$prefix\n";
 LookIfFHS();
}

sub LookIfFHS
{
 if ($OS eq 'UNIX')
   {
    print 'Checking if this system follows the FHS: ';
    if ($conf{'fhs'})
      {
       print "$conf{'fhs'} (cached/specified)\n";
       return;
      }
    if (-d '/usr/share/doc')
      {
       $conf{'fhs'}='yes';
      }
    else
      {
       $conf{'fhs'}='no';
      }
    print "$conf{'fhs'}\n";
   }
}

###[txh]####################################################################
#
# Prototype: CheckGCC()
# Description:
#   Tries to find a working C compiler. The following tests are done:@*
# 1) Look for cached GCC key.@*
# 2) Look for CC environment variable.@*
# 3) Default to gcc.@*
#   Then a small test program is compiled. If it fails the program dies.@*
#   The result is stored in the GCC configuration key.
#
# Return: The name of the working compiler.
#
####################################################################[txi]###

sub CheckGCC
{
 my ($cc,$test);

 print 'Looking for a working gcc: ';
 $cc=@conf{'GCC'};
 if ($cc)
   {
    print "$cc (cached) OK\n";
    return $cc;
   }
 $cc=$ENV{'CC'};
 if (!length($cc))
   {
    $cc='gcc';
   }
 print "$cc ";
 $test='#include <stdio.h>
int main(void)
{
 printf("OK\n");
 return 0;
}
';
 $test=RunGCCTest($cc,'c',$test,'');
 if ($test ne "OK\n")
   {
    CreateCache();
    die 'Not working gcc found';
   }
 print "OK\n";
 $conf{'GCC'}=$cc;
 $cc;
}

###[txh]####################################################################
#
# Prototype: CheckGCCcanXX($gcc)
# Description:
#   Checks if the C compiler can be used to compile C++ code.
#
# Return: 'yes' or 'no' according to a compilation test.
#
####################################################################[txi]###

sub CheckGCCcanXX
{
 my $cc=$_[0];
 my ($ret,$test);

 print "$cc can compile C++ code: ";
 $test='#include <iostream.h>
int main(void)
{
 cout << "OK" << endl;
 return 0;
}';
 $test=RunGCCTest($cc,'cc',$test,$stdcxx);
 if ($test eq "OK\n")
   {
    print "yes\n";
    $ret=1;
   }
 else
   {
    print "no\n";
    $ret=0;
   }
 $ret;
}


###[txh]####################################################################
#
# Prototype: CheckGXX()
# Description:
#   Finds the C++ compiler. Mechanism:@*
# 1) Cached GXX key.@*
# 2) Compiler declared in global $GCC variable.@*
# 3) Return value from CheckGXXReal().@*
#   The result is stored in the GXX configuration key and the $GXX global
# variable.
#
# Return: The $GXX value.
#
####################################################################[txi]###

sub CheckGXX
{
 if (@conf{'GXX'})
   {
    print "C++ compiler: @conf{'GXX'} (cached) OK\n";
    return @conf{'GXX'};
   }
 if (CheckGCCcanXX($GCC))
   {
    $GXX=$GCC;
   }
 else
   {
    # Test for a working g++
    $GXX=CheckGXXReal();
   }
 $conf{'GXX'}=$GXX;
}

###[txh]####################################################################
#
# Prototype: CheckGXXReal()
# Description:
#   Tries to find if $defaultCXX can compile C++. This global variable is
# determined during the OS detection. If it fails the program dies.
#
# Return: The name of the compiler.
#
####################################################################[txi]###

sub CheckGXXReal
{
 my $test;

 print 'Looking for the C++ compiler: ';
 $test='#include <iostream.h>
int main(void)
{
 cout << "OK" << endl;
 return 0;
}';
 $test=RunGCCTest($defaultCXX,'cc',$test,$stdcxx);
 if ($test ne "OK\n")
   {
    CreateCache();
    die('can not find it');
   }
 print "$defaultCXX\n";
 $defaultCXX;
}

###[txh]####################################################################
#
# Prototype: LookForDJGPP($versionNeeded)
# Description:
#   Finds if the installed djgpp version is the needed. Note that the
# version should be provided in the following way: djgpp v2.01 => 2.0.1.
# I did it to accomodate to the version comparisson test. Mechanism:@*
# 1) Cached djgpp key.@*
# 2) Compilation of a small program printing DJGPP.0.DJGPP_MINOR.@*
#   If the version is incorrect the program dies.@*
#   The result is stored in the 'djgpp' configuration key.
#
####################################################################[txi]###

sub LookForDJGPP
{
 my $vNeed=$_[0];
 my $test;

 print 'Checking DJGPP version: ';
 $test=@conf{'djgpp'};
 if ($test)
   {
    print "$test (cached) OK\n";
    return;
   }
 $test='
#include <stdio.h>
int main(void)
{
 printf("%d.0.%d",DJGPP,DJGPP_MINOR);
 return 0;
}';
 $test=RunGCCTest($GCC,'c',$test,'');
 if (!CompareVersion($test,$vNeed))
   {
    print "\nWrong DJGPP version, please use $vNeed or newer\n";
    print "Look in $ErrorLog for potential compile errors of the test\n";
    CreateCache();
    die "Wrong version\n";
   }
 print "$test OK\n";
 $conf{'djgpp'}=$test;
}

###[txh]####################################################################
#
# Prototype: FindCFLAGS()
# Description:
#   Determines the flags to be used for compilation. Mechanism:@*
# 1) Cached CFLAGS key.@*
# 2) Environment variable CFLAGS.@*
# 3) -O2 -gstabs+3 (-pipe if UNIX) (_WIN32 if Win32).@*
#   The result is stored in the 'CFLAGS' configuration key.
#
# Return: The value determined.
#
####################################################################[txi]###

sub FindCFLAGS
{
 my $ret;

 print 'C flags: ';
 $ret=@conf{'CFLAGS'};
 if ($ret)
   {
    print "$ret (cached)\n";
    return $ret;
   }
 $ret=@ENV{'CFLAGS'};
 if (!$ret)
   {
    $ret='-O2 -gstabs+3';
    # In UNIX pipes are in memory and allows multithreading so they are
    # usually faster. In Linux that's faster.
    $ret.=' -pipe' if ($OS eq 'UNIX');
    $ret.=' -D_WIN32' if ($OS eq 'Win32');
    # Looks like that's common and some sysadmins doesn't configure gcc to
    # look there:
    $ret.=' -I/usr/local/include -L/usr/local/include' if ($OSflavor eq 'FreeBSD');
   }
 print "$ret\n";
 $conf{'CFLAGS'}=$ret;
 $ret;
}


###[txh]####################################################################
#
# Prototype: FindCXXFLAGS()
# Description:
#   Determines the flags to be used for C++ compilation. Mechanism:@*
# 1) Cached CXXFLAGS key.@*
# 2) Environment variable CXXFLAGS.@*
# 3) Cached CFLAGS key.@*
# 4) Environment variable CFLAGS.@*
# 5) -O2 -gstabs+3 (-pipe if UNIX) (_WIN32 if Win32).@*
#   The result is stored in the 'CXXFLAGS' configuration key.
#
# Return: The value determined.
#
####################################################################[txi]###

sub FindCXXFLAGS
{
 my $ret;

 print 'C++ flags: ';
 $ret=@conf{'CXXFLAGS'};
 if ($ret)
   {
    print "$ret (cached)\n";
    return $ret;
   }
 $ret=@ENV{'CXXFLAGS'};
 $ret=@conf{'CFLAGS'} unless $ret;
 $ret=@ENV{'CFLAGS'} unless $ret;
 if (!$ret)
   {
    $ret='-O2 -gstabs+3';
    $ret.=' -pipe' if ($OS eq 'UNIX');
    $ret.=' -D_WIN32' if ($OS eq 'Win32');
    $ret.=' -I/usr/local/include -L/usr/local/include' if ($OSflavor eq 'FreeBSD');
   }
 print "$ret\n";
 $conf{'CXXFLAGS'}=$ret;
 $ret;
}


###[txh]####################################################################
#
# Prototype: FindXCFLAGS()
# Description:
#   Determines the flags to be used for special compilation. Mechanism:@*
# 1) Cached XCFLAGS key.@*
# 2) -O3 -fomit-frame-pointer -ffast-math (-pipe if not DOS).@*
#   The result is stored in the 'XCFLAGS' configuration key.
#
# Return: The value determined.
#
####################################################################[txi]###

sub FindXCFLAGS
{
 my $ret;

 print 'Special C flags: ';
 $ret=@conf{'XCFLAGS'};
 if ($ret)
   {
    print "$ret (cached/cmdline)\n";
    return $ret;
   }
 if (!$ret)
   {
    $ret='-O3 -fomit-frame-pointer -ffast-math';
    $ret.=' -pipe' unless ($OS eq 'dos');
   }
 print "$ret\n";
 $conf{'XCFLAGS'}=$ret;
 $ret;
}


###[txh]####################################################################
#
# Prototype: FindXCXXFLAGS()
# Description:
#   Determines the flags to be used for C++ compilation. Mechanism:@*
# 1) Cached XCXXFLAGS key.@*
# 2) -O3 -fomit-frame-pointer -ffast-math (-pipe if not DOS).@*
#   The result is stored in the 'XCXXFLAGS' configuration key.
#
# Return: The value determined.
#
####################################################################[txi]###

sub FindXCXXFLAGS
{
 my $ret;

 print 'Special C++ flags: ';
 $ret=@conf{'XCXXFLAGS'};
 if ($ret)
   {
    print "$ret (cached/cmdline)\n";
    return $ret;
   }
 if (!$ret)
   {
    $ret='-O3 -fomit-frame-pointer -ffast-math';
    $ret.=' -pipe' unless ($OS eq 'dos');
   }
 print "$ret\n";
 $conf{'XCXXFLAGS'}=$ret;
 $ret;
}


###[txh]####################################################################
#
# Prototype: DetectOS()
# Description:
#   Determines the OS using the uname command. If it returns MS-DOS then dos
# is assumed, if Linux or linux is returned the linux is assumed. If none
# of these is returned the program dies. The following global variables are
# filled according to the OS:@*
# $OS: dos or linux.@*
# $stdcxx: C++ library (stdcxx or stdc++).@*
# $defaultCXX: C++ compiler (gxx or g++).@*
# $supportDir: Directory to look for OS specific support (djgpp or linux).@*
#
# Return: The determined OS.
#
####################################################################[txi]###

sub DetectOS
{
 my ($os,$OS);
 $os=`uname`;
 print 'Determining OS: ';
 
 if ($os=~/MS\-DOS/)
   {
    $OS='DOS';
    $OSflavor='djgpp';
    $stdcxx='-lstdcxx';
    $defaultCXX='gxx';
    $supportDir='djgpp';
   }
 elsif ($os=~/[Ll]inux/)
   {
    $OS='UNIX';
    $OSflavor='Linux';
    $stdcxx='-lstdc++';
    $defaultCXX='g++';
    $supportDir='linux';
   }
 elsif ($os=~/FreeBSD/)
   {
    $OS='UNIX';
    $OSflavor='FreeBSD';
    $stdcxx='-lstdc++';
    $defaultCXX='g++';
    $supportDir='linux';
   }
 elsif ($os=~/CYGWIN/)
   {
    $OS='Win32';
    $OSflavor='Mingw';
    $stdcxx='-lstdc++';
    $defaultCXX='g++';
    $supportDir='win32';
   }
 else
   {
    die('Unknown OS, you must do things by yourself');
   }
 print "$OS [$OSflavor]\n";
 $OS;
}


###[txh]####################################################################
#
# Prototype: ModifyMakefiles(@list)
# Description:
#   Patches the listed RHIDE makefiles to use the detected C compiler, C++
# compiler and compilation flags.@*
#
####################################################################[txi]###

sub ModifyMakefiles
{
 my ($a,$text,$rep,$repv);

 print 'Configuring makefiles: ';
 foreach $a (@_)
   {
    print "$a ";
    $text=cat($a);
    if ($text)
      {
       $text=~s/RHIDE_GCC=(.*)\n/RHIDE_GCC=$GCC\n/;
       $text=~s/RHIDE_GXX=(.*)\n/RHIDE_GXX=$GXX\n/;
       $text=~s/RHIDE_LD=(.*)\n/RHIDE_LD=$GXX\n/;
       $text=~s/RHIDE_OS_CFLAGS=(.*)\n/RHIDE_OS_CFLAGS=$CFLAGS\n/;
       $text=~s/RHIDE_OS_CXXFLAGS=(.*)\n/RHIDE_OS_CXXFLAGS=$CXXFLAGS\n/;
       foreach $rep (%ExtraModifyMakefiles)
         {
          $repv="$rep=@ExtraModifyMakefiles{$rep}\n";
          $text=~s/$rep=(.*)\n/$repv/;
         }
       replace($a,$text);
      }
   }
 print "\n";
}

###[txh]####################################################################
#
# Prototype: ModifySimpleMakefiles(@list)
# Description:
#   Patches the listed makefiles to use the detected C compiler, C++
# compiler and compilation flags.@*
#
####################################################################[txi]###

sub ModifySimpleMakefiles
{
 my ($a,$text,$rep);

 print 'Configuring simple makefiles: ';
 foreach $a (@_)
   {
    print "$a ";
    $text=cat($a);
    if ($text)
      {
       $text=~s/GCC=(.*)\n/GCC=$GCC\n/;
       $text=~s/GXX=(.*)\n/GXX=$GXX\n/;
       $text=~s/LD=(.*)\n/LD=$GXX\n/;
       $text=~s/CFLAGS=(.*)\n/CFLAGS=$CFLAGS\n/;
       $text=~s/CXXFLAGS=(.*)\n/CXXFLAGS=$CXXFLAGS\n/;
       replace($a,$text);
      }
   }
 print "\n";
}

###[txh]####################################################################
#
# Prototype: CreateRHIDEenvs(@list)
# Description:
#   Generates all the needed rhide.env files to configure RHIDE with the
# detected C compiler, C++ compiler and compilation flags.@*
#   That's needed to use RHIDE after configuring.
#   Projects that needs faster C/C++ options are indicated with a +.
#
####################################################################[txi]###

sub CreateRHIDEenvs
{
 my ($a,$text,$rep,$useXtreme,$b);

 print 'Configuring RHIDE: ';
 foreach $a (@_)
   {
    if (substr($a,0,1) eq "+")
      {
       $b=substr($a,1);
       $useXtreme=1;
      }
    else
      {
       $b=$a;
       $useXtreme=0;
      }
    print "$b ";
    $text ="#\n# Automatically generated by configure script\n#\n\n";
    $text.="RHIDE_GCC=$GCC\n" unless ($GCC eq 'gcc');
    $text.="RHIDE_GXX=$GXX\n" unless ($GXX eq 'gcc');
    $text.="RHIDE_LD=$GXX\n"  unless ($GXX eq 'gcc');
    if ($useXtreme)
      {
       $text.="RHIDE_OS_CFLAGS=@conf{'XCFLAGS'}\n";
       $text.="RHIDE_OS_CXXFLAGS=@conf{'XCXXFLAGS'}\n";
      }
    else
      {
       $text.="RHIDE_OS_CFLAGS=$CFLAGS\n";
       $text.="RHIDE_OS_CXXFLAGS=$CXXFLAGS\n";
      }
    foreach $rep (@MakeDefsRHIDE)
      {
       $text.="$rep\n";
      }

    if ($text)
      {
       replace($b,$text);
      }
    else
      {
       unlink $b;
      }
   }
 print "\n";
}


###[txh]####################################################################
#
# Prototype: ConfigIncDef($defVar,$defComment)
# Description:
#   That's used to generate the config.h file and generates a line with
# /* $defComment */\n#define $defVar or not according to the existence of
# the $defVar configure key.
#
# Return: The string generated.
#
####################################################################[txi]###

sub ConfigIncDef
{
 my $text;

 $text ="\n\n/* $_[1] */\n";
 $text.='/*' unless (@conf{$_[0]});
 $text.="#define $_[0] 1";
 $text.='*/' unless (@conf{$_[0]});
 $text;
}

###[txh]####################################################################
#
# Prototype: ConfigIncDefYes($defVar,$defComment)
# Description:
#   That's similar to ConfigIncDef, but here the key must be 'yes'.
#
# Return: The string generated.
#
####################################################################[txi]###

sub ConfigIncDefYes
{
 my $text;

 $text ="\n\n/* $_[1] */\n";
 $text.='/*' unless (@conf{$_[0]} eq 'yes');
 $text.="#define $_[0] 1";
 $text.='*/' unless (@conf{$_[0]} eq 'yes');
 $text;
}

sub ParentDir
{
 my ($cur,$parent);
 $cur=`pwd`;
 chop $cur;
 chdir('..');
 $parent=`pwd`;
 chop $parent;
 chdir($cur);
 $parent;
}

sub ReplaceText
{
 my ($Text,$Dest,$i,$se,$re);

 $Dest=$_[1];
 print "Processing $_[0] => $_[1]\n";
 $Text=cat($_[0]);
 foreach $i (%ReplaceTags)
   {
    $se='@'.$i.'@';
    $re=@ReplaceTags{$i};
    $Text =~ s/$se/$re/g;
   }
 replace($Dest,$Text);
}

###[txh]####################################################################
#
# Prototype: ExtractItemsMak($makefile,\$column)
# Description:
#   Extracts the list of project items from a .mak. Is recursive.
#
# Return: The list of items.
#
####################################################################[txi]###

sub ExtractItemsMak
{
 my ($makefile,$col)=($_[0],\$_[1]);
 my ($mak,@items,$file,$result,$path);

 $result='';
 $makefile=~/((.+)\/+)/;
 $path=$1;
 print "Extracting from $makefile\n";
 $mak=cat($makefile);
 if ($mak=~ /PROJECT_ITEMS=(((.+)\\\n)+(.+)\n)/)
   {
    @items=split(/[\\\s]+/,$1);
    foreach $file (@items)
      {
       if ($file=~/\.gpr/)
         {
          $file=~s/\.gpr/\.mak/;
          $result.=ExtractItemsMak($path.$file,$$col);
          #print "**** Back ($path)\n";
         }
       else
         {
          #print "$file\n";
          if ($$col!=0)
            {
             if ($$col+length($file)>76)
               {
                $result.="\\\n\t";
                $$col=8;
               }
             else
               {
                $result.=' ';
                $$col++;
               }
            }
          $$col+=length($file);
          $result.="$file";
         }
      }
   }
 $result;
}

1;
