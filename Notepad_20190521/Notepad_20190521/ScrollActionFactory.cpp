#include "ScrollActionFactory.h"
#include "NotepadForm.h"
#include "ScrollActions.h"

ScrollActionFactory::ScrollActionFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

ScrollActionFactory::ScrollActionFactory(const ScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;
}

ScrollActionFactory::~ScrollActionFactory() {

}

ScrollAction* ScrollActionFactory::MakeHScrollAction(UINT nSBCode) {
	ScrollAction *scrollAction = 0;

	switch (nSBCode) {
	case SB_LINELEFT:
		scrollAction = new LineLeftAction(this->notepadForm);
		break;
	case SB_LINERIGHT:
		scrollAction = new LineRightAction(this->notepadForm);
		break;
	case SB_PAGELEFT:
		scrollAction = new PageLeftAction(this->notepadForm);
		break;
	case SB_PAGERIGHT:
		scrollAction = new PageRightAction(this->notepadForm);
		break;
	case SB_THUMBTRACK:
		scrollAction = new ThumbTrackHScrollAction(this->notepadForm);
		break;

	default:
		break;
	}

	return scrollAction;
}

ScrollAction* ScrollActionFactory::MakeVScrollAction(UINT nSBCode) {
	ScrollAction *scrollAction = 0;

	switch (nSBCode) {
	case SB_LINEUP:
		scrollAction = new LineUpAction(this->notepadForm);
		break;
	case SB_LINEDOWN:
		scrollAction = new LineDownAction(this->notepadForm);
		break;
	case SB_PAGEUP:
		scrollAction = new PageUpAction(this->notepadForm);
		break;
	case SB_PAGEDOWN:
		scrollAction = new PageDownAction(this->notepadForm);
		break;
	case SB_THUMBTRACK:
		scrollAction = new ThumbTrackVScrollAction(this->notepadForm);
		break;
	default:
		break;
	}

	return scrollAction;
}


ScrollActionFactory& ScrollActionFactory::operator=(const ScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}