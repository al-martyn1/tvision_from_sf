#ifndef NO_STREAM

#define Uses_TStatusLine
#define Uses_TStreamableClass
#include <tv.h>
__link( RView )

TStreamableClass RStatusLine( TStatusLine::name,
                              TStatusLine::build,
                              __DELTA(TStatusLine)
                            );

#endif
