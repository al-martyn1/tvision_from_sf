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
// SET: Moved the standard headers here because according to DJ
// they can inconditionally declare symbols like NULL
#define Uses_ctype
#define Uses_string
#define Uses_TKeys
#define Uses_TInputLine
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#define Uses_TStreamableClass
#define Uses_TValidator
#define Uses_TPalette
#include <tv.h>

const int CONTROL_Y = 25;

char hotKey( const char *s )
{
    const char *p;

    if( (p = strchr( s, '~' )) != 0 )
        return uctoupper(p[1]);
    else
        return 0;
}

#define cpInputLine "\x13\x13\x14\x15"

TInputLine::TInputLine( const TRect& bounds, int aMaxLen ) :
    TView(bounds),
    data( new char[aMaxLen] ),
    maxLen( aMaxLen-1 ),
    curPos( 0 ),
    firstPos( 0 ),
    selStart( 0 ),
    selEnd( 0 ),
    validator(NULL)
{
    state |= sfCursorVis;
    options |= ofSelectable | ofFirstClick;
    *data = EOS;
}

void TInputLine::SetValidator(TValidator * aValidator)
{
  destroy(validator);
  validator = aValidator;
  if (validator)
    validator->SetOwner(this);
}

TInputLine::~TInputLine()
{
    delete[] data;
    destroy(validator);
}

Boolean TInputLine::canScroll( int delta )
{
    if( delta < 0 )
        return Boolean( firstPos > 0 );
    else
        if( delta > 0 )
            return Boolean( (int32)strlen(data) - firstPos + 2 > size.x );
        else
            return False;
}

uint32 TInputLine::dataSize()
{
    return maxLen+1;
}

void TInputLine::draw()
{
    int l, r;
    TDrawBuffer b;

    uchar color = (state & sfFocused) ? getColor( 2 ) : getColor( 1 );

    b.moveChar( 0, ' ', color, size.x );
    char buf[256];
    strncpy( buf, data+firstPos, size.x - 2 );
    buf[size.x - 2 ] = EOS;
    b.moveStr( 1, buf, color );

    if( canScroll(1) )
        b.moveChar( size.x-1, rightArrow, getColor(4), 1 );
    if( canScroll(-1) )
        b.moveChar( 0, leftArrow, getColor(4), 1 );
    if( state & sfSelected )
        {
        l = selStart - firstPos;
        r = selEnd - firstPos;
        l = max( 0, l );
        r = min( size.x - 2, r );
        if (l <  r)
            b.moveChar( l+1, 0, getColor(3), r - l );
        }
    writeLine( 0, 0, size.x, size.y, b );
    setCursor( curPos-firstPos+1, 0);
}

void TInputLine::getData( void *rec )
{
    memcpy( rec, data, dataSize() );
}

TPalette& TInputLine::getPalette() const
{
    static TPalette palette( cpInputLine, sizeof( cpInputLine )-1 );
    return palette;
}

int TInputLine::mouseDelta( TEvent& event )
{
    TPoint mouse = makeLocal( event.mouse.where );

    if( mouse.x <= 0 )
        return -1;
    else
        if( mouse.x >= size.x - 1 )
            return 1;
        else
            return 0;
}

int TInputLine::mousePos( TEvent& event )
{
    TPoint mouse = makeLocal( event.mouse.where );
    mouse.x = max( mouse.x, 1 );
    int pos = mouse.x + firstPos - 1;
    pos = max( pos, 0 );
    pos = min( pos, strlen(data) );
    return pos;
}

void  TInputLine::deleteSelect()
{
    if( selStart < selEnd )
        {
        strcpy( data+selStart, data+selEnd );
        curPos = selStart;
        }
}

void  TInputLine::handleEvent( TEvent& event )
{
    TView::handleEvent(event);

    int delta, anchor, i;
    if( (state & sfSelected) != 0 )
        switch( event.what )
            {
            case  evMouseDown:
                if( canScroll(delta = mouseDelta(event)) )
                    do  {
                        if( canScroll(delta) )
                            {
                            firstPos += delta;
                            drawView();
                            }
                        } while( mouseEvent( event, evMouseAuto ) );
                else if (event.mouse.doubleClick)
                        selectAll(True);
                else
                    {
                    anchor =  mousePos(event);
                    do  {
                        if( event.what == evMouseAuto &&
                            canScroll( delta = mouseDelta(event) )
                          )
                            firstPos += delta;
                        curPos = mousePos(event);
                        if( curPos < anchor )
                            {
                            selStart = curPos;
                            selEnd = anchor;
                            }
                        else
                            {
                            selStart = anchor;
                            selEnd = curPos;
                            }
                        drawView();
                        } while (mouseEvent(event, evMouseMove | evMouseAuto));
                    }
                clearEvent(event);
                break;
            case  evKeyDown:
                switch( ctrlToArrow(event.keyDown.keyCode) )
                    {
                    case kbLeft:
                        if( curPos > 0 )
                            curPos--;
                        break;
                    case kbRight:
                        if( curPos < (int32)strlen(data) )
                            curPos++;
                        break;
                    case kbHome:
                        curPos =  0;
                        break;
                    case kbEnd:
                        curPos = strlen(data);
                        break;
                    case kbBack:
                        if( curPos > 0 )
                            {
                            strcpy( data+curPos-1, data+curPos );
                            curPos--;
                            if( firstPos > 0 )
                                firstPos--;
                            }
                        break;
                    case kbDel:
                        if( selStart == selEnd )
                            if( curPos < (int32)strlen(data) )
                                {
                                selStart = curPos;
                                selEnd = curPos + 1;
                                }
                        deleteSelect();
                        break;
                    case kbIns:
                        setState(sfCursorIns, Boolean(!(state & sfCursorIns)));
                        break;
                    default:
                        if( event.keyDown.charScan.charCode >= ' ' )
                            {
                            if (validator)
                            {
                              char tmp[2];
                              tmp[0] = event.keyDown.charScan.charCode;
                              tmp[1] = 0;
                              if (validator->IsValidInput(tmp,False) == False)
                              {
                                clearEvent(event);
                                break;
                              }
                            }
                            if( (state & sfCursorIns) == 0 )
                            {
                                deleteSelect();
                            }
                            int32 l = strlen(data);
                            if (((l == maxLen) && ((state & sfCursorIns) == 0)) ||
                                ((state & sfCursorIns) && curPos == maxLen))
                              resizeData();
                            {
                              if( (state & sfCursorIns) == 0 )
                              {
                                if (l < maxLen)
                                  memmove( data + curPos + 1, data + curPos,
                                           strlen(data+curPos)+1 );
                              }
                              else if (l == curPos)
                              {
                                data[curPos+1] = 0;
                              }
                              if ((((state & sfCursorIns) == 0) && (l < maxLen)) ||
                                  ((state & sfCursorIns) && (curPos < maxLen)))
                              {
                                if( firstPos > curPos )
                                    firstPos = curPos;
                                data[curPos++] =
                                    event.keyDown.charScan.charCode;
                              }
                            }
                            }
                        else if( event.keyDown.charScan.charCode == CONTROL_Y)
                            {
                            *data = EOS;
                            curPos = 0;
                            }
                        else
                            return;
                    }
                    selStart = 0;
                    selEnd = 0;
                    if( firstPos > curPos )
                        firstPos = curPos;
                    i = curPos - size.x + 2;
                    if( firstPos < i )
                        firstPos = i;
                    drawView();
                    clearEvent( event );
                    break;
            }
}

void TInputLine::selectAll( Boolean enable )
{
    selStart = 0;
    if( enable )
        curPos = selEnd = strlen(data);
    else
        curPos = selEnd = 0;
    firstPos = max( 0, curPos-size.x+2 );
    drawView();
}

void TInputLine::setData( void *rec )
{
    memcpy( data, rec, dataSize()-1 );
    data[dataSize()-1] = EOS;
    selectAll( True );
}

void TInputLine::setState( ushort aState, Boolean enable )
{
    TView::setState( aState, enable );
    if( aState == sfSelected ||
        ( aState == sfActive && (state & sfSelected) != 0 )
      )
        selectAll( enable );
}

#if !defined( NO_STREAM )
void TInputLine::write( opstream& os )
{
    TView::write( os );
    os << maxLen << curPos << firstPos
       << selStart << selEnd;
    os.writeString( data);
    os << validator;
}

void *TInputLine::read( ipstream& is )
{
    TView::read( is );
    is >> maxLen >> curPos >> firstPos
       >> selStart >> selEnd;
    data = new char[maxLen + 1];
    is.readString(data, maxLen+1);
    state |= sfCursorVis;
    is >> validator;
    return this;
}

TStreamable *TInputLine::build()
{
    return new TInputLine( streamableInit );
}

TInputLine::TInputLine( StreamableInit ) : TView( streamableInit ),
  validator(NULL)
{
}
#endif // NO_STREAM

Boolean TInputLine::valid(ushort )
{
  Boolean ret = True;
  if (validator)
  {
    ret = validator->Valid(data);
    if (ret == True)
    {
      validator->Format(data);
      drawView();
    }
  }
  return ret;
}

