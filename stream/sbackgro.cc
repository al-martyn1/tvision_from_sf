#ifndef NO_STREAM

#define Uses_TStreamableClass
#define Uses_TBackground
#include <tv.h>
__link( RView )

TStreamableClass RBackground( TBackground::name,
                              TBackground::build,
                              __DELTA(TBackground)
                            );

#endif
