#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H

#include <afxwin.h>

class NotepadForm;
class KeyAction {
public:
	KeyAction(NotepadForm *notepadForm = 0);
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	KeyAction& operator=(const KeyAction& source);
protected:
	NotepadForm *notepadForm;
};

class LeftKeyAction : public KeyAction {
public:
	LeftKeyAction(NotepadForm *notepadForm = 0);
	LeftKeyAction(const LeftKeyAction& source);
	virtual ~LeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	LeftKeyAction& operator=(const LeftKeyAction& source);
};

class RightKeyAction : public KeyAction {
public:
	RightKeyAction(NotepadForm *notepadForm = 0);
	RightKeyAction(const RightKeyAction& source);
	virtual ~RightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	RightKeyAction& operator=(const RightKeyAction& source);
};

class UpKeyAction : public KeyAction {
public:
	UpKeyAction(NotepadForm *notepadForm = 0);
	UpKeyAction(const UpKeyAction& source);
	virtual ~UpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	UpKeyAction& operator = (const UpKeyAction& source);
};

class DownKeyAction : public KeyAction {
public:
	DownKeyAction(NotepadForm *notepadForm = 0);
	DownKeyAction(const DownKeyAction& source);
	virtual ~DownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	DownKeyAction& operator=(const DownKeyAction& source);
};

class HomeKeyAction : public KeyAction {
public:
	HomeKeyAction(NotepadForm *notepadForm = 0);
	HomeKeyAction(const HomeKeyAction& source);
	virtual ~HomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	HomeKeyAction& operator =(const HomeKeyAction& source);
};

class EndKeyAction : public KeyAction {
public:
	EndKeyAction(NotepadForm *notepadForm = 0);
	EndKeyAction(const EndKeyAction& source);
	virtual ~EndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	EndKeyAction& operator =(const EndKeyAction& source);
};

class CtrlLeftKeyAction : public KeyAction {
public:
	CtrlLeftKeyAction(NotepadForm *notepadForm = 0);
	CtrlLeftKeyAction(const CtrlLeftKeyAction& source);
	virtual ~CtrlLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlLeftKeyAction& operator =(const CtrlLeftKeyAction& source);
};

class CtrlRightKeyAction : public KeyAction {
public:
	CtrlRightKeyAction(NotepadForm *notepadForm = 0);
	CtrlRightKeyAction(const CtrlRightKeyAction& source);
	virtual ~CtrlRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlRightKeyAction& operator =(const CtrlRightKeyAction& source);
};

class CtrlHomeKeyAction : public KeyAction {
public:
	CtrlHomeKeyAction(NotepadForm *notepadForm = 0);
	CtrlHomeKeyAction(const CtrlHomeKeyAction& source);
	virtual ~CtrlHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlHomeKeyAction& operator =(const CtrlHomeKeyAction& source);
};

class CtrlEndKeyAction : public KeyAction {
public:
	CtrlEndKeyAction(NotepadForm *notepadForm = 0);
	CtrlEndKeyAction(const CtrlEndKeyAction& source);
	virtual ~CtrlEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlEndKeyAction& operator =(const CtrlEndKeyAction& source);
};

class DeleteKeyAction : public KeyAction {
public:
	DeleteKeyAction(NotepadForm *notepadForm = 0);
	DeleteKeyAction(const DeleteKeyAction& source);
	virtual ~DeleteKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DeleteKeyAction& operator =(const DeleteKeyAction& source);
};

class BackSpaceKeyAction : public KeyAction {
public:
	BackSpaceKeyAction(NotepadForm *notepadForm = 0);
	BackSpaceKeyAction(const BackSpaceKeyAction& source);
	virtual ~BackSpaceKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BackSpaceKeyAction& operator =(const BackSpaceKeyAction& source);
};

class PageUpKeyAction : public KeyAction {
public:
	PageUpKeyAction(NotepadForm *notepadForm = 0);
	PageUpKeyAction(const PageUpKeyAction& source);
	virtual ~PageUpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	PageUpKeyAction& operator=(const PageUpKeyAction& source);
};

class PageDownKeyAction : public KeyAction {
public:
	PageDownKeyAction(NotepadForm *notepadForm = 0);
	PageDownKeyAction(const PageDownKeyAction& source);
	virtual ~PageDownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	PageDownKeyAction& operator=(const PageDownKeyAction& source);
};

class CtrlZ :public KeyAction {
public:
	CtrlZ(NotepadForm *notepadForm = 0);
	CtrlZ(const CtrlZ& source);
	virtual ~CtrlZ();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlZ& operator=(const CtrlZ& source);
};

class ShiftLeftKeyAction : public KeyAction {
public:
	ShiftLeftKeyAction(NotepadForm *notepadForm = 0);
	ShiftLeftKeyAction(const ShiftLeftKeyAction& source);
	virtual ~ShiftLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftLeftKeyAction& operator=(const ShiftLeftKeyAction& source);
};

class ShiftRightKeyAction : public KeyAction {
public:
	ShiftRightKeyAction(NotepadForm *notepadForm = 0);
	ShiftRightKeyAction(const ShiftRightKeyAction& source);
	virtual ~ShiftRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftRightKeyAction& operator=(const ShiftRightKeyAction& source);
};

class ShiftUpKeyAction : public KeyAction {
public:
	ShiftUpKeyAction(NotepadForm *notepadForm = 0);
	ShiftUpKeyAction(const ShiftUpKeyAction& source);
	virtual ~ShiftUpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftUpKeyAction& operator=(const ShiftUpKeyAction& source);
};

class ShiftDownKeyAction : public KeyAction {
public:
	ShiftDownKeyAction(NotepadForm *notepadForm = 0);
	ShiftDownKeyAction(const ShiftDownKeyAction& source);
	virtual ~ShiftDownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftDownKeyAction& operator=(const ShiftDownKeyAction& source);
};

class CtrlShiftHomeKeyAction : public KeyAction {
public:
	CtrlShiftHomeKeyAction(NotepadForm *notepadForm = 0);
	CtrlShiftHomeKeyAction(const CtrlShiftHomeKeyAction& source);
	virtual ~CtrlShiftHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftHomeKeyAction& operator=(const CtrlShiftHomeKeyAction& source);
};

class CtrlShiftEndKeyAction : public KeyAction {
public:
	CtrlShiftEndKeyAction(NotepadForm *notepadForm = 0);
	CtrlShiftEndKeyAction(const CtrlShiftEndKeyAction& source);
	virtual ~CtrlShiftEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftEndKeyAction& operator=(const CtrlShiftEndKeyAction& source);
};

class ShiftHomeKeyAction : public KeyAction {
public:
	ShiftHomeKeyAction(NotepadForm *notepadForm = 0);
	ShiftHomeKeyAction(const ShiftHomeKeyAction& source);
	virtual ~ShiftHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftHomeKeyAction& operator=(const ShiftHomeKeyAction& source);
};

class ShiftEndKeyAction : public KeyAction {
public:
	ShiftEndKeyAction(NotepadForm *notepadForm = 0);
	ShiftEndKeyAction(const ShiftEndKeyAction& source);
	virtual ~ShiftEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftEndKeyAction& operator=(const ShiftEndKeyAction& source);
};

class CtrlShiftLeftKeyAction : public KeyAction {
public:
	CtrlShiftLeftKeyAction(NotepadForm *notepadForm = 0);
	CtrlShiftLeftKeyAction(const CtrlShiftLeftKeyAction& source);
	virtual ~CtrlShiftLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftLeftKeyAction& operator=(const CtrlShiftLeftKeyAction& source);
};

class CtrlShiftRightKeyAction : public KeyAction {
public:
	CtrlShiftRightKeyAction(NotepadForm *notepadForm = 0);
	CtrlShiftRightKeyAction(const CtrlShiftRightKeyAction& source);
	virtual ~CtrlShiftRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftRightKeyAction& operator=(const CtrlShiftRightKeyAction& source);
};

class CtrlAKeyAction : public KeyAction {
public:
	CtrlAKeyAction(NotepadForm *notepadForm = 0);
	CtrlAKeyAction(const CtrlAKeyAction& source);
	virtual ~CtrlAKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlAKeyAction& operator=(const CtrlAKeyAction& source);
};

class DelAnBSKeyActionInSelState : public KeyAction {
public:
	DelAnBSKeyActionInSelState(NotepadForm *notepadForm = 0);
	DelAnBSKeyActionInSelState(const DelAnBSKeyActionInSelState& source);
	virtual ~DelAnBSKeyActionInSelState();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DelAnBSKeyActionInSelState& operator=(const DelAnBSKeyActionInSelState& source);
};


class CtrlCKeyAction : public KeyAction {
public:
	CtrlCKeyAction(NotepadForm *notepadForm = 0);
	CtrlCKeyAction(const CtrlCKeyAction& source);
	virtual ~CtrlCKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlCKeyAction& operator=(const CtrlCKeyAction& source);
};

class CtrlXKeyAction : public KeyAction {
public:
	CtrlXKeyAction(NotepadForm *notepadForm = 0);
	CtrlXKeyAction(const CtrlXKeyAction& source);
	virtual ~CtrlXKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlXKeyAction& operator=(const CtrlXKeyAction& source);
};


class CtrlVKeyAction : public KeyAction {
public:
	CtrlVKeyAction(NotepadForm *notepadForm = 0);
	CtrlVKeyAction(const CtrlVKeyAction& source);
	virtual ~CtrlVKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlVKeyAction& operator=(const CtrlVKeyAction& source);
};

class CtrlFKeyAction : public KeyAction {
public:
	CtrlFKeyAction(NotepadForm *notepadForm = 0);
	CtrlFKeyAction(const CtrlFKeyAction& source);
	virtual ~CtrlFKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlFKeyAction& operator=(const CtrlFKeyAction& source);
};

class CtrlHKeyAction : public KeyAction {
public:
	CtrlHKeyAction(NotepadForm *notepadForm = 0);
	CtrlHKeyAction(const CtrlHKeyAction& source);
	virtual ~CtrlHKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlHKeyAction& operator=(const CtrlHKeyAction& source);
};


#endif //_KEYACTIONS_H