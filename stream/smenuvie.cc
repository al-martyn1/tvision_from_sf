#ifndef NO_STREAM

#define Uses_TMenuView
#define Uses_TStreamableClass
#include <tv.h>
__link( RView )

TStreamableClass RMenuView( TMenuView::name,
                            TMenuView::build,
                            __DELTA(TMenuView)
                          );

#endif
