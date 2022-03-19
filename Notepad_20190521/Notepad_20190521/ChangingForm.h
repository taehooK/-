#ifndef _CHANGINGFORM_H
#define _CHANGINGFORM_H

#include <afxwin.h>
#include "resource.h"

class NotepadForm;
class ChangingForm : public CDialog {
public:
	enum { IDD = IDD_CHANGINGFORM};
public:
	ChangingForm(NotepadForm *notepadForm, CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
protected:
	afx_msg void OnNexFindButtonClicked();
	afx_msg void OnChangeButtonClicked();
	afx_msg void OnAllChangeButtonClicked();
	afx_msg void OnCancelButtonClicked();
	afx_msg void OnEnChangeEditContentToFind();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP();

protected:
	virtual void OnOK();

private:
	NotepadForm *notepadForm;

};


#endif // !_CHANGINGFORM_H
