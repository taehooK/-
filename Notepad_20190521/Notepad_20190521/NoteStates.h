#ifndef _NOTESTATES_H

typedef unsigned int UINT;
typedef signed long int Long;
class NotepadForm;
class NoteState {
public:
	virtual void OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnImeComposition(NotepadForm *notepadForm, char(*buffer));
	virtual void OnImeChar(NotepadForm *notepadForm, char(*buffer));
	virtual void OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	virtual void OnLButtonDblClk(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	virtual void OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	
protected:
	void ChangeState(NotepadForm *notepadForm, NoteState *noteState);
};

class GeneralState : public NoteState {
public:
	static GeneralState* Instance();
	
	virtual void OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnImeComposition(NotepadForm *notepadForm, char(*buffer));
	virtual void OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	virtual void OnLButtonDblClk(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	virtual void OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
protected:
	GeneralState();
private:
	static GeneralState* instance;
};

class SelectedState : public NoteState {
public:
	static SelectedState* Instance();

	virtual void OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnImeComposition(NotepadForm *notepadForm, char(*buffer));
	virtual void OnImeChar(NotepadForm *notepadForm, char(*buffer));
	virtual void OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);
	virtual void OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY);

protected:
	SelectedState();
private:
	static SelectedState* instance;
};

class ComposingState : public NoteState {
public:
	static ComposingState* Instance();

	virtual void OnImeComposition(NotepadForm *notepadForm, char(*buffer));
	virtual void OnImeChar(NotepadForm *notepadForm, char(*buffer));
protected:
	ComposingState();
private:
	static ComposingState* instance;
};


#endif // !_NOTESTATES_H
