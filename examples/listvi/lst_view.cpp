// ***********************************************************************
//
//  LST_VIEW.CPP
//  List Viewer(s) that display String array(s)
//  revised November 27,1993
//  C.Porter
// ***********************************************************************
#include <stdlib.h>
#include <stdio.h>
#define Uses_string
#define Uses_TListViewer
#define Uses_TListBox
#define Uses_TEvent
#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TKeys
#define Uses_TWindow
#define Uses_TView
#define Uses_MsgBox
#include <tv.h>
#include "lst_view.h"

const int cmListKeyEnter		   = 59;
//************************************************************
TListViewBox::TListViewBox( const TRect& bounds,
						  ushort aNumCols,
						  TScrollBar *hScrollBar,
						  TScrollBar *vScrollBar,
						  char *aList[],
						  int aListSize):
	 TListViewer(bounds, aNumCols, hScrollBar, vScrollBar)
{
	strlist = aList; // strlist is a pointer to a pointer				  //	to keep from overflowing the stack
	setRange(aListSize);
	vScrollBar->maxVal = aListSize-1;
}
//************************************************************
TListViewBox::~TListViewBox()
{ }
//************************************************************
void TListViewBox::getText( char *dest, ccIndex item, short maxLen )
{
 strncpy( dest,strlist[item],maxLen ); // strlist is a pointer to a pointer
}
//************************************************************
// List View Box uses evBroadCast to send message back to dialog
// 'this' pointer is used in evBroadcast to let dialog know who sent the
// message. This pointer is used to get text for input line and  later to
// store input line data back into List View Box object.

void TListViewBox::handleEvent(TEvent& event) {

  if ((event.what == evMouseDown) && (event.mouse.doubleClick))
	{ message(owner, evBroadcast, cmListItemSelected, this);
		clearEvent(event);}
  if ((event.what == evKeyDown) && (event.keyDown.keyCode == kbEnter ))
	{ message(owner, evBroadcast, cmListKeyEnter, this);
		clearEvent(event);}

  TListViewer::handleEvent(event);
} // end of TMyListBox::handleEvent()
//************************************************************
// writes data to list
void TListViewBox::putData(void *rec)
{
	strcpy(strlist[focused],((char *)rec));
	draw();
}
//************************************************************
//************************************************************

TListViewDialog::TListViewDialog(const TRect &trect, char *title,
					char *aList[],char *aList2[],int aListSize)
					:TDialog(trect, title),
					 TWindowInit(TDialog::initFrame) {
  options |= ofCentered;
  int lbwidth =( trect.b.x -trect.a.x)-3;
  int lbhite = (trect.b.y -trect.a.y)-2;
  insert (new TStaticText(TRect(3,1,12,2)," City "));
  insert (new TStaticText(TRect(15,1,25,2)," Zip Code"));
  // create a scroll bar for the list box
  listScroller = new TScrollBar(TRect(trect.a.x+lbwidth,trect.a.y+2,
						   trect.a.x+lbwidth+1,trect.a.y+lbhite));

  listBox = new TListViewBox(TRect(trect.a.x+3, trect.a.y+2,
		trect.a.x+lbwidth/2+2, trect.a.y+lbhite),1,0,listScroller,
		aList,aListSize);
  listBox2 = new TListViewBox(TRect(trect.a.x+15, trect.a.y+2,
		trect.a.x+lbwidth, trect.a.y+lbhite),1,0,listScroller,
		aList2,aListSize);
  // insert the list box with its scroller into the dialog box
  inputLine = new TInputLine(TRect(listBox->origin.x,3,listBox->size.x+5, 4),listBox->size.x);
  inputLine->hide(); //hide input line
  itemNumber = new TInputLine(TRect(3,lbhite,8,lbhite+1),5);
  itemNumber->options &= (!ofSelectable); // used to hold current line number
  insert(listBox);
  insert(listBox2);
  insert(listScroller);
  insert(inputLine);
  insert(itemNumber);
} // end of MyDialogBox::MyDialogBox()
//*****************************************************************
// a single mouse click is not cleared by List View Box, Dialog uses it
// to erase inputline if user clicks off of it .
void TListViewDialog::handleEvent(TEvent &event) {
	switch(event.what)
	{
	  case evMouseDown:  // clears input line
			if (inputLine->state&sfSelected) {
				inputLine->hide();
			}
			break;
	  case evKeyDown:
		  switch(event.keyDown.keyCode)
		  {
		  case kbEsc:  // clears input line
			if (inputLine->state&sfSelected) {
				inputLine->hide();
				clearEvent(event);
			}
			break;
		  case kbEnter: // saves input line to list box using listBoxPtr
			if (inputLine->state&sfSelected) {
				listBoxPtr->putData( inputLine->data );
				inputLine->hide();
				clearEvent(event);
			}
			break;
	  }
	 case evBroadcast: // from List Boxes, infoPtr points to orginator
	   switch (event.message.command)
	   {
		case cmListItemSelected: // if input line is already showing, hide it
			if (inputLine->state&sfSelected) {
				inputLine->hide();
				clearEvent(event);
			}
			else {  // show empty input line
				int mouseLocY =((TListViewBox *)event.message.infoPtr)->cursor.y ;
				int mouseLocX =((TListViewBox *)event.message.infoPtr)->origin.x ;
				listBoxPtr =(TListViewBox *)event.message.infoPtr;
				inputLine->moveTo(mouseLocX, mouseLocY+2);
				strcpy(inputLine->data,"        ");
				inputLine->show();
				clearEvent(event);
			}
			break;
		case cmListKeyEnter: // enter key pressed in list box, copy data to inputline
			int mouseLocY =((TListViewBox *)event.message.infoPtr)->cursor.y ;
			int mouseLocX =((TListViewBox *)event.message.infoPtr)->origin.x ;
			listBoxPtr =(TListViewBox *)event.message.infoPtr;
			inputLine->moveTo(mouseLocX, mouseLocY+2);
			listBoxPtr->getText( inputLine->data,listBoxPtr->focused,inputLine->maxLen );
			inputLine->show();
			clearEvent(event);
			break;
	   }
	}
	// Let TDialog handler do it's thing with any remaining events.
	TDialog::handleEvent(event);
	//itoa(listBox->focused, itemNumber->data, 10);
   sprintf(itemNumber->data,"%d",listBox->focused);
	itemNumber->draw();
} // end of MyDialogBox::eventHandler()

