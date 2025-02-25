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

#if defined( Uses_TView ) && !defined( __TView )
#define __TView

struct write_args
{
    void *self;
    void *target;
    void *buf;
    ushort offset;
};

class TRect;
struct TEvent;
class TGroup;
class TPalette;
class TCommandSet;

class TView : public TObject
#if !defined ( NO_STREAM )
                             , public TStreamable
#endif // NO_STREAM
{

public:

    friend void genRefs();

    enum phaseType { phFocused, phPreProcess, phPostProcess };
    enum selectMode{ normalSelect, enterSelect, leaveSelect };

    TView( const TRect& bounds );
    ~TView();

    virtual void sizeLimits( TPoint& min, TPoint& max );
    TRect getBounds();
    TRect getExtent();
    TRect getClipRect();
    Boolean mouseInView( TPoint mouse );
    Boolean containsMouse( TEvent& event );

    void locate( TRect& bounds );
    virtual void dragView( TEvent& event, uchar mode,   //  temporary fix
      TRect& limits, TPoint minSize, TPoint maxSize ); //  for Miller's stuff
    virtual void calcBounds( TRect& bounds, TPoint delta );
    virtual void changeBounds( const TRect& bounds );
    void growTo( int x, int y );
    void moveTo( int x, int y );
    void setBounds( const TRect& bounds );

    virtual ushort getHelpCtx();

    virtual Boolean valid( ushort command );

    void hide();
    void show();
    virtual void draw();
    void drawView();
    Boolean exposed();
    void hideCursor();
    void drawHide( TView *lastView );
    void drawShow( TView *lastView );
    void drawUnderRect( TRect& r, TView *lastView );
    void drawUnderView( Boolean doShadow, TView *lastView );

    virtual uint32 dataSize();
    virtual void getData( void *rec );
    virtual void setData( void *rec );

    void blockCursor();
    void normalCursor();
    virtual void resetCursor();
    void setCursor( int x, int y );
    void showCursor();
    void drawCursor();

    void clearEvent( TEvent& event );
    Boolean eventAvail();
    virtual void getEvent( TEvent& event );
    virtual void handleEvent( TEvent& event );
    virtual void putEvent( TEvent& event );

    static Boolean commandEnabled( ushort command );
    static void disableCommands( TCommandSet& commands );
    static void enableCommands( TCommandSet& commands );
    static void disableCommand( ushort command );
    static void enableCommand( ushort command );
    static void getCommands( TCommandSet& commands );
    static void setCommands( TCommandSet& commands );

    virtual void endModal( ushort command );
    virtual ushort execute();

    ushort getColor( ushort color );
    virtual TPalette& getPalette() const;
    uchar mapColor( uchar );

    Boolean getState( ushort aState );
    void select();
    virtual void setState( ushort aState, Boolean enable );

    void keyEvent( TEvent& event );
    Boolean mouseEvent( TEvent& event, ushort mask );


    TPoint makeGlobal( TPoint source );
    TPoint makeLocal( TPoint source );

    TView *nextView();
    TView *prevView();
    TView *prev();
    TView *next;

    void makeFirst();
    void putInFrontOf( TView *Target );
    TView *TopView();

    void writeBuf(  short x, short y, short w, short h, const void *b );
    void writeBuf(  short x, short y, short w, short h, const TDrawBuffer& b );
    void writeChar( short x, short y, char c, uchar color, short count );
    void writeLine( short x, short y, short w, short h, const TDrawBuffer& b );
    void writeLine( short x, short y, short w, short h, const void *b );
    void writeStr( short x, short y, const char *str, uchar color );

    TPoint size;
    ushort options;
    ushort eventMask;
    ushort state;
    TPoint origin;
    TPoint cursor;
    uchar growMode;
    uchar dragMode;
    ushort helpCtx;
    static Boolean commandSetChanged;
    static TCommandSet curCommandSet;
    TGroup *owner;

    static Boolean showMarkers;
    static uchar errorAttr;

    virtual void shutDown();

private:

    void moveGrow( TPoint p,
                   TPoint s,
                   TRect& limits,
                   TPoint minSize,
                   TPoint maxSize,
                   uchar mode
                 );
    void change( uchar, TPoint delta, TPoint& p, TPoint& s, int grow=0 );
    static void writeView( write_args );

#if !defined( NO_STREAM )
    virtual const char *streamableName() const
        { return name; }

protected:

    TView( StreamableInit );

public:

    static const char * const name;
    static TStreamable *build();

protected:

    virtual void write( opstream& );
    virtual void *read( ipstream& );
#endif // NO_STREAM
};

inline ipstream& operator >> ( ipstream& is, TView& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TView*& cl )
    { return is >> (void *&)cl; }

inline opstream& operator << ( opstream& os, TView& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TView* cl )
    { return os << (TStreamable *)cl; }

inline void TView::writeBuf( short x, short y, short w, short h,
                             const TDrawBuffer& b )
{
    writeBuf( x, y, w, h, b.data );
}

inline void TView::writeLine( short x, short y, short w, short h,
                              const TDrawBuffer& b )
{
    writeLine( x, y, w, h, b.data );
}

#endif  // Uses_TView

