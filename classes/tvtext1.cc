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

#define Uses_TScreen
#define Uses_TRadioButtons
#define Uses_TMenuBox
#define Uses_TFrame
#define Uses_TIndicator
#define Uses_THistory
#define Uses_TColorSelector
#define Uses_TMonoSelector
#define Uses_TColorDialog
#define Uses_TInputLine
#define Uses_TStatusLine
#define Uses_TCheckBoxes
#define Uses_TScrollBar
#define Uses_TButton
#define Uses_TDirListBox
#define Uses_TFileEditor
#define Uses_TFileInfoPane
#define Uses_TDeskTop
#define Uses_TListViewer
#include <tv.h>

// All the strings are arrays ASCIIZ
// All duplicated to restore the original

uchar specialChars[] =
{
    175, 174, 26, 27, ' ', ' ', 0
};
uchar o_specialChars[] =
{
    175, 174, 26, 27, ' ', ' ', 0
};

char TRadioButtons::button[] = " ( ) ";
char TRadioButtons::obutton[] = " ( ) ";
char TRadioButtons::check = 7; // 
char TRadioButtons::ocheck = 7;

//" �Ŀ  ���  � �  �Ĵ " 8
char TMenuBox::frameChars[] = " \332\304\277  \300\304\331  \263 \263  \303\304\264 ";
char TMenuBox::oframeChars[] = " \332\304\277  \300\304\331  \263 \263  \303\304\264 ";
char TMenuBox::rightArrow = 16; // 
char TMenuBox::orightArrow = 16; // 

const char TFrame::initFrame[19] =
  "\x06\x0A\x0C\x05\x00\x05\x03\x0A\x09\x16\x1A\x1C\x15\x00\x15\x13\x1A\x19";

char TFrame::frameChars[33] =
    "   � ��� �������   � ��� ��ϻ�� "; // for UnitedStates code page
char TFrame::oframeChars[33] =
    "   � ��� �������   � ��� ��ϻ�� "; // for UnitedStates code page

// �  
char TFrame::closeIcon[] = "[~\xFE~]";
char TFrame::ocloseIcon[] = "[~\xFE~]";
char TFrame::zoomIcon[] = "[~\x18~]";
char TFrame::ozoomIcon[] = "[~\x18~]";
char TFrame::unZoomIcon[] = "[~\x12~]";
char TFrame::ounZoomIcon[] = "[~\x12~]";
char TFrame::dragIcon[] = "~��~";
char TFrame::odragIcon[] = "~��~";

char TIndicator::dragFrame = '\xCD';  // �
char TIndicator::odragFrame = '\xCD';  // �
char TIndicator::normalFrame = '\xC4'; // �
char TIndicator::onormalFrame = '\xC4'; // �
char TIndicator::modifiedStar = 15; // 
char TIndicator::omodifiedStar = 15; // 

char THistory::icon[] = "\xDE~\x19~\xDD"; // ��
char THistory::oicon[] = "\xDE~\x19~\xDD"; // ��

char TColorSelector::icon = '\xDB'; // �
char TColorSelector::oicon = '\xDB'; // �

char TMonoSelector::button[] = " ( ) ";
char TMonoSelector::obutton[] = " ( ) ";

char TInputLine::rightArrow = '\x10';
char TInputLine::orightArrow = '\x10';
char TInputLine::leftArrow = '\x11';
char TInputLine::oleftArrow = '\x11';

char TStatusLine::hintSeparator[] = "\xB3 "; // �
char TStatusLine::ohintSeparator[] = "\xB3 "; // �

char TCheckBoxes::button[] = " [ ] ";
char TCheckBoxes::obutton[] = " [ ] ";

TScrollChars TScrollBar::vChars = {30, 31, 177, 254, 178}; // ���
TScrollChars TScrollBar::ovChars = {30, 31, 177, 254, 178}; // ���
TScrollChars TScrollBar::hChars = {17, 16, 177, 254, 178}; // ���
TScrollChars TScrollBar::ohChars = {17, 16, 177, 254, 178}; // ���

char TButton::shadows[] = "\xDC\xDB\xDF"; // ���
char TButton::oshadows[] = "\xDC\xDB\xDF"; // ���
char TButton::markers[] = "[]";
char TButton::omarkers[] = "[]";

char TDirListBox::pathDir[]   = "���";
char TDirListBox::opathDir[]   = "���";
char TDirListBox::firstDir[]  =   "���";
char TDirListBox::ofirstDir[]  =   "���";
char TDirListBox::middleDir[] =   " ��";
char TDirListBox::omiddleDir[] =   " ��";
char TDirListBox::lastDir[]   =   " ��";
char TDirListBox::olastDir[]   =   " ��";
char TDirListBox::graphics[]  = "���";
char TDirListBox::ographics[]  = "���";

const char * TFileInfoPane::pmText = "p";
const char * TFileInfoPane::amText = "a";

const char TDeskTop::defaultBkgrnd = '\xB0';

uchar TListViewer::columnSeparator = 179; // �
uchar TListViewer::ocolumnSeparator = 179; // �

