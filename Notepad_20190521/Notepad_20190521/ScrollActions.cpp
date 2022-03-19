#include "ScrollActions.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Scroll.h"

ScrollAction::ScrollAction(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

ScrollAction::ScrollAction(const ScrollAction& source) {
	this->notepadForm = source.notepadForm;
}

ScrollAction::~ScrollAction() {

}

ScrollAction& ScrollAction::operator=(const ScrollAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}


// LineUpAction
LineUpAction::LineUpAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

LineUpAction::LineUpAction(const LineUpAction& source)
	: ScrollAction(source) {

}

LineUpAction::~LineUpAction() {

}

void LineUpAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Up();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

LineUpAction& LineUpAction::operator=(const LineUpAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// LineDownAction
LineDownAction::LineDownAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

LineDownAction::LineDownAction(const LineDownAction& source)
	: ScrollAction(source) {

}

LineDownAction::~LineDownAction() {

}

void LineDownAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Down();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

LineDownAction& LineDownAction::operator=(const LineDownAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// PageUpAction

PageUpAction::PageUpAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

PageUpAction::PageUpAction(const PageUpAction& source)
	: ScrollAction(source) {

}

PageUpAction::~PageUpAction() {

}

void PageUpAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageUp();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

PageUpAction& PageUpAction::operator=(const PageUpAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// PageDownAction

PageDownAction::PageDownAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

PageDownAction::PageDownAction(const PageDownAction& source)
	: ScrollAction(source) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageDown();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

PageDownAction& PageDownAction::operator=(const PageDownAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// ThumbTrackVScrollAction

ThumbTrackVScrollAction::ThumbTrackVScrollAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

ThumbTrackVScrollAction::ThumbTrackVScrollAction(const ThumbTrackVScrollAction& source)
	: ScrollAction(source) {

}

ThumbTrackVScrollAction::~ThumbTrackVScrollAction() {

}

void ThumbTrackVScrollAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->MoveVerticalScroll(nPos);
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

ThumbTrackVScrollAction& ThumbTrackVScrollAction::operator=(const ThumbTrackVScrollAction& source) {
	ScrollAction::operator=(source);

	return *this;
}


// LineLeftAction
LineLeftAction::LineLeftAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

LineLeftAction::LineLeftAction(const LineLeftAction& source)
	: ScrollAction(source) {

}

LineLeftAction::~LineLeftAction() {

}

void LineLeftAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Left();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);//
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

LineLeftAction& LineLeftAction::operator=(const LineLeftAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// LineRightAction

LineRightAction::LineRightAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

LineRightAction::LineRightAction(const LineRightAction& source)
	: ScrollAction(source) {

}

LineRightAction::~LineRightAction() {

}

void LineRightAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Right();
	Long previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);

}

LineRightAction& LineRightAction::operator=(const LineRightAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// PageLeftAction

PageLeftAction::PageLeftAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

PageLeftAction::PageLeftAction(const PageLeftAction& source)
	: ScrollAction(source) {

}

PageLeftAction::~PageLeftAction() {

}

void PageLeftAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageLeft();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

PageLeftAction& PageLeftAction::operator=(const PageLeftAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// PageRightAction

PageRightAction::PageRightAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

PageRightAction::PageRightAction(const PageRightAction& source)
	: ScrollAction(source) {

}

PageRightAction::~PageRightAction() {

}

void PageRightAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageRight();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

PageRightAction& PageRightAction::operator=(const PageRightAction& source) {
	ScrollAction::operator=(source);

	return *this;
}

// ThumbTrackHScrollAction

ThumbTrackHScrollAction::ThumbTrackHScrollAction(NotepadForm *notepadForm)
	:ScrollAction(notepadForm) {

}

ThumbTrackHScrollAction::ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source)
	: ScrollAction(source) {

}

ThumbTrackHScrollAction::~ThumbTrackHScrollAction() {

}

void ThumbTrackHScrollAction::OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->MoveHorizontalScroll(nPos);
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

ThumbTrackHScrollAction& ThumbTrackHScrollAction::operator=(const ThumbTrackHScrollAction& source) {
	ScrollAction::operator=(source);

	return *this;
}
