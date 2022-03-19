#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H
#include <afxwin.h>
#include "Subject.h"
#include "EditorialMemory.h"


class Font;
class CaretController;
class ScrollController;
class CharacterMetrics;
class Glyph;
class AutoLineBreaker;
class NoteState;
class Clipboard;
class FindingForm;
class ChangingForm;
class SelectedAreaHandler;
class Command;
class NotepadForm : public CFrameWnd, public Subject {
public:
	NotepadForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	Glyph *note;
	Glyph *current;
	CaretController *caretController; 
	ScrollController *scrollController;
	AutoLineBreaker *autoLineBreaker;
	Font *font;
	CharacterMetrics *characterMetrics;
	CString fileName;
	BOOL isSaved;
	CSize noteSize;
	CMenu menu;
	Clipboard *clipboard;
	FindingForm *findingForm;
	ChangingForm *changingForm;
	SelectedAreaHandler *selectedAreaHandler;
	EditorialMemory editorialMemory;

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd *pNewWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlag, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	BOOL GetIsComposing() const;
	NoteState* GetNoteState() const;
	


private:
	friend class NoteState;
	friend class FindingForm;
	friend class ChangingForm;
	friend class AutoLineBreakCommand;
	void ChangeState(NoteState * noteState);
	
private:
	BOOL isComposing;
	NoteState* noteState;
};
inline BOOL NotepadForm::GetIsComposing() const {
	return this->isComposing;
}
inline NoteState* NotepadForm::GetNoteState() const {
	return this->noteState;
}

#endif // !_NOTEPADFORM_H
