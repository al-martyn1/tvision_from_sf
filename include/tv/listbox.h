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

#if defined( Uses_TListBox ) && !defined( __TListBox )
#define __TListBox

class TRect;
class TScrollBar;
class TCollection;

struct TListBoxRec
{
    TCollection *items;
    ccIndex selection;
};

class TListBox : public TListViewer
{

public:

    TListBox( const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar );
    TListBox( const TRect& bounds, ushort aNumCols, TScrollBar *aHScrollBar,
              TScrollBar *aVScrollBar );
    ~TListBox();

    virtual uint32 dataSize();
    virtual void getData( void *rec );
    virtual void getText( char *dest, ccIndex item, short maxLen );
    virtual void newList( TCollection *aList );
    virtual void setData( void *rec );

    TCollection *list();

protected:
    TCollection *items;

private:

#if !defined( NO_STREAM )
    virtual const char *streamableName() const
	{ return name; }

protected:

    TListBox( StreamableInit );
    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();
#endif // NO_STREAM
};

#if !defined( NO_STREAM )
inline ipstream& operator >> ( ipstream& is, TListBox& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TListBox*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TListBox& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TListBox* cl )
    { return os << (TStreamable *)cl; }
#endif // NO_STREAM

inline TCollection *TListBox::list()
{
    return items;
}

#endif  // Uses_TListBox

