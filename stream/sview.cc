#ifndef NO_STREAM

#define Uses_TView
#define Uses_TStreamableClass
#include <tv.h>

TStreamableClass RView( TView::name,
                        TView::build,
                        __DELTA(TView)
                      );

#endif
