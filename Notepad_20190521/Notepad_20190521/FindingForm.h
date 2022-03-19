#ifndef _FINDINGFORM_H
#define _FINDINGFOMR_H
#include <afxwin.h>
#include "resource.h"

class NotepadForm;
class FindingForm : public CDialog {
public:
	enum { IDD = IDD_FINDINGFORM };

public:
	FindingForm(NotepadForm *notepadForm, CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
protected:
	afx_msg void OnNextFindButtonClicked();
	afx_msg void OnCancelButtonClicked();
	afx_msg void OnEnChangeEditContentToFind();
	afx_msg void OnKeyDown(UINT nchar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP();

protected:
	virtual void OnOK();

private:
	NotepadForm *notepadForm;
};

#endif // !_FINDINGFORM_H
