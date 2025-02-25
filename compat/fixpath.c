/**[txh]********************************************************************

  Description:
  This file contains a replacement for djgpp's _fixpath function. Currently
I have two versions. Both are based in djgpp's code. One is for Linux
(adapted by Robert) and the other is for NT adapted by Anatoli Soltan.

***************************************************************************/

#include <cl/needs.h>



//#ifdef TVOSf_NT
#ifdef TVOS_Win32
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/* Modified for Win32 by Anatoli Soltan */

#define Uses_string
#define Uses_getcurdir
#include <compatlayer.h>

#ifdef __BORLANDC__

#include <dir.h>

static int __fp_getdisk(void)
{
  return getdisk();
}

#elif defined(_MSC_VER) || defined(__MINGW32__)

#include <direct.h>

static int __fp_getdisk(void)
{
  return _getdrive() - 1;
}

#else
#error "No support for this compiler"
#endif

// getcurdir is provided by the compatibility layer ;-)
static char* __fp_getcurdir(char* out, int drive)
{
  if (!getcurdir(drive + 1, out))
    out += strlen(out);
  return out;
}

#define is_slash(c) CLY_IsValidDirSep(c)
#define is_term(c)  (CLY_IsValidDirSep(c) || c=='\0')

/* Takes as input an arbitrary path.  Fixes up the path by:
   1. Removing consecutive slashes
   2. Removing trailing slashes
   3. Making the path absolute if it wasn't already
   4. Removing "." in the path
   5. Removing ".." entries in the path (and the directory above them)
   6. Adding a drive specification if one wasn't there
   7. Converting all slashes to '/'
 */
void
_fixpath(const char *in, char *out)
{
  int           drive_number = 0;
  const char    *ip = in;
  char          *op = out;
  int           unc = CLY_IsUNC(in); /* is a UNC pathname */

  /* Add drive specification to output string */
  if (((*ip >= 'a' && *ip <= 'z') ||
       (*ip >= 'A' && *ip <= 'Z'))
      && (*(ip + 1) == ':'))
  {
    if (*ip >= 'a' && *ip <= 'z')
    {
      drive_number = *ip - 'a';
      *op++ = *ip++;
    }
    else
    {
      drive_number = *ip - 'A';
      *op++ = (char)(drive_number + 'a');
      ++ip;
    }
    *op++ = *ip++;
  }
  else if (!unc)
  {
    drive_number = __fp_getdisk();
    *op++ = (char)(drive_number + 'a');
    *op++ = ':';
  }

  /* Convert relative path to absolute */
  if (!is_slash(*ip))
  {
    *op++ = '/';
    op = __fp_getcurdir(op, drive_number);
  }

  /* Handle UNC path */
  if (unc)
  {
    *op++ = *ip++;
  }

  /* Step through the input path */
  while (*ip)
  {
    /* Skip input slashes */
    if (is_slash(*ip))
    {
      ip++;
      continue;
    }

    /* Skip "." and output nothing */
    if (*ip == '.' && is_term(*(ip + 1)))
    {
      ip++;
      continue;
    }

    /* Skip ".." and remove previous output directory */
    if (*ip == '.' && *(ip + 1) == '.' && is_term(*(ip + 2)))
    {
      ip += 2;
      /* Don't back up over drive spec */
      if (op > out + 2)
      {
        /* This requires "/" to follow drive spec */
        --op;
        while (!is_slash(*op)) --op;
      }
      continue;
    }

    /* Copy path component from in to out */
    *op++ = '/';
    while (!is_term(*ip)) *op++ = *ip++;
  }

  /* If root directory, insert trailing slash */
  if (op == out + 2) *op++ = '/';

  /* Null terminate the output */
  *op = '\0';

  /* Convert backslashes to slashes */
  for (op = out; *op; op++)
    if (*op == '\\') *op = '/';
}
/* vi: set ts=8 sw=2 : */
#define FIXPATH_DEFINED
#endif // TVOSf_NT -- Anatoli's version







#if defined(NEEDS_FIXPATH) && !defined(FIXPATH_DEFINED)
/*
   Robert:
   Here now the very usefull function _fixpath() from DJGPP's
   libc 'fixpath.c'
   I have modified it to be used on unix systems (like linux).
*/

/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */

#define Uses_unistd
#define Uses_string
#define Uses_limits
#define Uses_getcurdir
#include <compatlayer.h>

inline static int
is_slash(int c)
{
  return c == DIRSEPARATOR;
}

inline static int
is_term(int c)
{
  return c == DIRSEPARATOR || c == '\0';
}

/* Takes as input an arbitrary path.  Fixes up the path by:
   1. Removing consecutive slashes
   2. Removing trailing slashes
   3. Making the path absolute if it wasn't already
   4. Removing "." in the path
   5. Removing ".." entries in the path (and the directory above them)
 */
void _fixpath(const char *in, char *out)
{
  const char    *ip = in;
  char          *op = out;

  /* Convert relative path to absolute */
  if (!is_slash(*ip))
  {
    getcurdir(0,op);
    op += strlen(op);
  }

  /* Step through the input path */
  while (*ip)
  {
    /* Skip input slashes */
    if (is_slash(*ip))
    {
      ip++;
      continue;
    }

    /* Skip "." and output nothing */
    if (*ip == '.' && is_term(*(ip + 1)))
    {
      ip++;
      continue;
    }

    /* Skip ".." and remove previous output directory */
    if (*ip == '.' && *(ip + 1) == '.' && is_term(*(ip + 2)))
    {
      ip += 2;
      /* Don't back up over root '/' */
      if (op > out )
      /* This requires "/" to follow drive spec */
        while (!is_slash(*--op));
      continue;
    }

    /* Copy path component from in to out */
    *op++ = '/';
    while (!is_term(*ip)) *op++ = *ip++;
  }

  /* If root directory, insert trailing slash */
  if (op == out) *op++ = '/';

  /* Null terminate the output */
  *op = '\0';

}

/*
  End of modified code from DJGPP's libc 'fixpath.c'
*/
#endif // Default version

