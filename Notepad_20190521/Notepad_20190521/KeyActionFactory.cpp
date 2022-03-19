#include "KeyActionFactory.h"
#include "KeyActions.h"
#include <WinUser.h>
#include "NotepadForm.h"
#include "NoteStates.h"

KeyActionFactory::KeyActionFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

KeyActionFactory::~KeyActionFactory() {

}

KeyAction* KeyActionFactory::Make(UINT nChar) {
	KeyAction* keyAction = 0;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (isCtrl && isShift && nChar == VK_HOME) {
		keyAction = new CtrlShiftHomeKeyAction(this->notepadForm);
	}
	else if (isCtrl && isShift && nChar == VK_END) {
		keyAction = new CtrlShiftEndKeyAction(this->notepadForm);
	}
	else if (isCtrl && isShift && nChar == VK_LEFT) {
		keyAction = new CtrlShiftLeftKeyAction(this->notepadForm);
	}
	else if (isCtrl && isShift && nChar == VK_RIGHT) {
		keyAction = new CtrlShiftRightKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_LEFT) {
		keyAction = new ShiftLeftKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_RIGHT) {
		keyAction = new ShiftRightKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_UP) {
		keyAction = new ShiftUpKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_DOWN) {
		keyAction = new ShiftDownKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_HOME) {
		keyAction = new ShiftHomeKeyAction(this->notepadForm);
	}
	else if (isShift && nChar == VK_END) {
		keyAction = new ShiftEndKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_LEFT) {
		keyAction = new CtrlLeftKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_RIGHT) {
		keyAction = new CtrlRightKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_HOME) {
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_END) {
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == 0x5A) {
		keyAction = new CtrlZ(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x41 || nChar == 0x61)) {
		keyAction = new CtrlAKeyAction(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x43 || nChar == 0x63)) {
		keyAction = new CtrlCKeyAction(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x58 || nChar == 0x78)) {
		keyAction = new CtrlXKeyAction(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x56 || nChar == 0x76)) {
		keyAction = new CtrlVKeyAction(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x46 || nChar == 0x66)) {
		keyAction = new CtrlFKeyAction(this->notepadForm);
	}
	else if (isCtrl && (nChar == 0x48 || nChar == 0x68)) {
		keyAction = new CtrlHKeyAction(this->notepadForm);
	}
	else if (nChar == VK_LEFT) {
		keyAction = new LeftKeyAction(this->notepadForm);
	}
	else if (nChar == VK_RIGHT) {
		keyAction = new RightKeyAction(this->notepadForm);
	}
	else if (nChar == VK_UP) {
		keyAction = new UpKeyAction(this->notepadForm);
	}
	else if (nChar == VK_DOWN) {
		keyAction = new DownKeyAction(this->notepadForm);
	}
	else if (nChar == VK_HOME) {
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	else if (nChar == VK_END) {
		keyAction = new EndKeyAction(this->notepadForm);
	}
	else if ((nChar == VK_DELETE || nChar == VK_BACK) && dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())){
		keyAction = new DelAnBSKeyActionInSelState(this->notepadForm);
	}
	else if (nChar == VK_DELETE) {
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	else if (nChar == VK_BACK) {
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	else if (nChar == VK_PRIOR) {
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	else if (nChar == VK_NEXT) {
		keyAction = new PageDownKeyAction(this->notepadForm);
	}
	return keyAction;
}
