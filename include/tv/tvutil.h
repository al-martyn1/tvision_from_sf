/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *

Modified by Robert H�hne to be used for RHIDE.

 *
 *
 */

#if !defined( __UTIL_H )
#define __UTIL_H
#undef __CM
#undef __HC
#undef __BASE
#define __BASE 128
#define __CM(x,y) const unsigned short cm##x = __BASE+y;
#define __HC(x,y) const unsigned short hc##x = __BASE+y;

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

inline int min( int a, int b )
{
    return (a>b) ? b : a;
}

inline int max( int a, int b )
{
    return (a<b) ? b : a;
}

char hotKey( const char *s );
unsigned short ctrlToArrow( unsigned short );

unsigned short historyCount( unsigned char id );
const char *historyStr( unsigned char id, int index );
void historyAdd( unsigned char id, const char * );

int cstrlen( const char * );

class TView;
void *message( TView *receiver, unsigned short what, unsigned short command,
               void *infoPtr );
Boolean lowMemory();

char *newStr( const char * );

/* SET: Added a conditional for it because n is too common. I remmember I had
   problems with it and Jerzy Witkowski <jwi@pl.ibm.com> complained about
   conflicts between a class member in your code and this macro */
#ifdef Uses_n
/* The following macro is a little hack to decrease the compile time
   for the TV lib when compiling all the n*.cc files */

#define n(CLASS)                          \
class CLASS                               \
{                                         \
public:                                   \
  static const char * const name;         \
};                                        \
                                          \
const char * const CLASS::name = #CLASS;
#endif

#endif  // __UTIL_H
