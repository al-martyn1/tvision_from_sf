#ifndef NO_STREAM

#define Uses_TGroup
#define Uses_TStreamableClass
#include <tv.h>
__link( RView )

TStreamableClass RGroup( TGroup::name,
                         TGroup::build,
                         __DELTA(TGroup)
                       );

#endif
