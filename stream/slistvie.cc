#ifndef NO_STREAM

#define Uses_TListViewer
#define Uses_TStreamableClass
#include <tv.h>
__link( RView )
__link( RScrollBar )

TStreamableClass RListViewer( TListViewer::name,
                              TListViewer::build,
                              __DELTA(TListViewer)
                            );

#endif
