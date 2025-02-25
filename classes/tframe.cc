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
#define Uses_string
#include <stdlib.h>
#include <stdio.h>

#define Uses_TFrame
#define Uses_TDrawBuffer
#define Uses_TWindow
#define Uses_TRect
#define Uses_TPoint
#define Uses_TEvent
#define Uses_TStreamableClass
#define Uses_TPalette
#include <tv.h>

#define cpFrame "\x01\x01\x02\x02\x03"

TFrame::TFrame( const TRect& bounds ) : TView( bounds )
{
    growMode = gfGrowHiX + gfGrowHiY;
    eventMask |= evBroadcast | evMouseUp;
}

void TFrame::draw()
{
    ushort cFrame, cTitle;
    short  f, i, l, width;
    TDrawBuffer b;

    if( (state & sfActive) == 0 )
        {
        cFrame = 0x0101;
        cTitle = 0x0002;
        f = 0;
        }
    else
        if( (state & sfDragging) != 0 )
            {
            cFrame = 0x0505;
            cTitle = 0x0005;
            f = 0;
            }
        else
            {
            cFrame = 0x0503;
            cTitle = 0x0004;
            f = 9;
            }

    cFrame = getColor(cFrame);
    cTitle = getColor(cTitle);

    width = size.x;
    l = width - 10;

    if( ( ((TWindow *)owner)->flags & (wfClose | wfZoom) ) != 0 )
        l -= 6;
    frameLine( b, 0, f, uchar(cFrame) );
    if( ((TWindow *)owner)->number != wnNoNumber )
        {
        l -= 4;
        if( ( ((TWindow *)owner)->flags & wfZoom ) != 0 )
            i = 7;
        else
            i = 3;
        int number = ((TWindow *)owner)->number;
        if (number > 10) i++;
        if (number > 100) i++;
        if (number > 1000) i++;
        char Number[10];
        sprintf(Number,"%d",number);
        int j=0;
        while (Number[j])
          {
            b.putChar( width-i+j, Number[j]);
            j++;
          }
        }

    if( owner != 0 )
        {
        const char *title = ((TWindow *)owner)->getTitle(l);
        if( title != 0 )
            {
            int ls;
            l = max(width-10,0);
            ls = strlen(title);
            if (ls>l)
              {
               i = (width - l) >> 1;
               b.moveBuf( i-1, " ..", cTitle, 3 );
               b.moveBuf( i+2, &title[ls-l+2], cTitle, l );
               b.putChar( i+l, ' ' );
               b.putChar( i+l+1, ' ' );
              }
            else
              {
               l=ls;
               i = (width - l) >> 1;
               b.putChar( i-1, ' ' );
               b.moveBuf( i, title, cTitle, l );
               b.putChar( i+l, ' ' );
              }
            }
        }

    if( (state & sfActive) != 0 )
        {
        if( ( ((TWindow *)owner)->flags & wfClose ) != 0 )
            b.moveCStr( 2, closeIcon, cFrame );
        if( ( ((TWindow *)owner)->flags & wfZoom ) != 0 )
            {
            TPoint minSize, maxSize;
            owner->sizeLimits( minSize, maxSize );
            if( owner->size == maxSize )
                b.moveCStr( width-5, unZoomIcon, cFrame );
            else
                b.moveCStr( width-5, zoomIcon, cFrame );
            }
        }

    writeLine( 0, 0, size.x, 1, b );
    for( i = 1; i <=  size.y - 2; i++ )
        {
        frameLine( b, i, f +  3, cFrame );
        writeLine( 0, i, size.x, 1, b );
        }
    frameLine( b, size.y - 1, f +  6, cFrame );
    if( (state & sfActive) != 0 )
        if( ( ((TWindow *)owner)->flags & wfGrow ) != 0 )
            b.moveCStr( width-2, dragIcon, cFrame );
    writeLine( 0, size.y - 1, size.x, 1, b );
}

TPalette& TFrame::getPalette() const
{
    static TPalette palette( cpFrame, sizeof( cpFrame )-1 );
    return palette;
}

void TFrame::dragWindow( TEvent& event, uchar mode )
{
    TRect  limits;
    TPoint min, max;

    limits = owner->owner->getExtent();
    owner->sizeLimits( min, max );
    owner->dragView( event, owner->dragMode | mode, limits, min, max );
    clearEvent( event );
}

void TFrame::handleEvent( TEvent& event )
{
  TView::handleEvent(event);
  if( (event.what & (evMouseDown | evMouseUp)) && (state & sfActive) != 0 )
  {
    TPoint mouse = makeLocal( event.mouse.where );
    if( mouse.y == 0 )
    {
      if( ( ((TWindow *)owner)->flags & wfClose ) != 0 &&
              mouse.x >= 2 &&
              mouse.x <= 4 )
      {
        if (event.what == evMouseUp)
        {
          event.what= evCommand;
          event.message.command = cmClose;
          event.message.infoPtr = owner;
          putEvent( event );
        }
        clearEvent( event );
      }
      else
      {
        if (  event.mouse.doubleClick ||
             ((((TWindow *)owner)->flags & wfZoom) != 0 &&
              mouse.x >= size.x - 5 &&
              mouse.x <= size.x - 3))
        {
          if (event.mouse.doubleClick || event.what == evMouseUp)
          {
            event.what= evCommand;
            event.message.command = cmZoom;
            event.message.infoPtr = owner;
            putEvent( event );
          }
          clearEvent( event );
        }
        else
        {
          if( ( ((TWindow *)owner)->flags & wfMove ) != 0 &&
                    event.what == evMouseDown)
          {
            dragWindow( event, dmDragMove );
          }
        }
      }
    }
    else
    {
      if( mouse.x >= size.x - 2 && mouse.y >= size.y - 1 &&
          event.what == evMouseDown )
      {
        if( ( ((TWindow *)owner)->flags & wfGrow ) != 0 )
        {
          dragWindow( event, dmDragGrow );
        }
      }
    }
  }
}

void TFrame::setState( ushort aState, Boolean enable )
{
    TView::setState( aState, enable );
    if( (aState & (sfActive | sfDragging)) != 0 )
	drawView();
}

#if !defined( NO_STREAM )
TStreamable *TFrame::build()
{
    return new TFrame( streamableInit );
}

TFrame::TFrame( StreamableInit ) : TView( streamableInit )
{
}
#endif // NO_STREAM

char FrameMask[maxViewWidth];

void TFrame::frameLine( TDrawBuffer& frameBuf, short y, short n, uchar color )
{
  ushort si,ax,cx,dx,di;
  int i=1;
  TView *view;
  cx = dx = size.x;
  cx -= 2;
  FrameMask[0] = initFrame[n];
  while (cx--) FrameMask[i++] = initFrame[n+1];
  FrameMask[i] = initFrame[n+2];
  view = owner->last;
  dx--;
lab1:
  view = view->next;
  if (view == this) goto lab10;
  if (!(view->options & ofFramed)) goto lab1;
  if (!(view->state & sfVisible)) goto lab1;
  ax = y - view->origin.y;
  if ((short)(ax)<0) goto lab3;
  if (ax>view->size.y) goto lab1;
  if (ax<view->size.y) ax = 5;
    else ax = 0x0a03;
  goto lab4;
lab3:
  ax++;
  if (ax) goto lab1;
  ax = 0x0a06;
lab4:
  si = view->origin.x;
  di = si + view->size.x;
  if (si>1) goto lab5;
  si = 1;
lab5:
  if (di<dx) goto lab6;
  di = dx;
lab6:
  if (si>=di) goto lab1;
  FrameMask[si-1] |= (ax & 0x00ff);
  ax ^= (((ax & 0xff00) >> 8) & 0x00ff);
  FrameMask[di] |= (ax & 0x00ff);
  if (!(ax & 0xff00)) goto lab1;
  cx = di-si;
  while (cx--) FrameMask[si++] |= (((ax & 0xff00) >> 8) & 0x00ff);
  goto lab1;
lab10:
  dx++;
  {
    uchar * framechars = (uchar *)malloc(dx);
    for (i=0;i<dx;i++)
        framechars[i] = (uchar)frameChars[FrameMask[i]];
    frameBuf.moveBuf(0,framechars,color,dx);
    free(framechars);
  }
#if 0
  ax = color << 8;
  cx = dx;
  si = 0;
  i = 0;
  while (cx--)
  {
    ((ushort *)(frameBuf.data))[i] = ((uchar)frameChars[FrameMask[i]]) | (ax & 0xff00);
    i++;
  }
#endif
}

