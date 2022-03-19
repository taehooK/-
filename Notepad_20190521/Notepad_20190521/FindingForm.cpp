//FindingForm.cpp

#include "FindingForm.h"
#include "Scanner.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "Note.h"
#include "Line.h"
#include "Finder.h"
#include "NoteStates.h"
#include "CharacterMetrics.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "ChangingForm.h"
#include "SelectedAreaHandler.h"

BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEXTFIND, OnNextFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()

	ON_EN_CHANGE(IDC_EDIT_CONTENT_TO_FIND, &FindingForm::OnEnChangeEditContentToFind)
	
END_MESSAGE_MAP()

FindingForm::FindingForm(NotepadForm *notepadForm, CWnd* parent)
	:CDialog(FindingForm::IDD, parent)
{
	this->notepadForm = notepadForm;
}

BOOL FindingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	CWnd::CheckRadioButton(IDC_RADIO_UP, IDC_RADIO_DOWN, IDC_RADIO_UP);
	CheckDlgButton(IDC_CHECK_CASESENSITIVE, 1);

	return FALSE;
}

BOOL FindingForm::PreTranslateMessage(MSG *pMsg) {

	if (pMsg->message == WM_KEYDOWN) {
		UINT nRepCnt = (UINT)((pMsg->lParam) & 0x0000FFFF);
		UINT nFlags = (UINT)((pMsg->lParam) >> 16);
		this->OnKeyDown(pMsg->wParam, nRepCnt, nFlags);
		
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void FindingForm::OnOK() {
	if (GetDlgItem(IDC_BUTTON_NEXTFIND)->IsWindowEnabled()) {
		this->OnNextFindButtonClicked();
	}
}


void FindingForm::OnNextFindButtonClicked() {
	CString text;

	GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);

	GlyphFactory glyphFactory;
	Glyph *pattern = glyphFactory.MakeLine(text.GetBuffer());

	int checkedButton = CWnd::GetCheckedRadioButton(IDC_RADIO_UP, IDC_RADIO_DOWN);
	BOOL check_caseSensitive = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);


	Long lineNumber = this->notepadForm->note->GetCurrent();
	Finder finder;
	Long index = this->notepadForm->current->GetCurrent();
	NoteState *noteState = this->notepadForm->GetNoteState();
	Point startPosition = { 0, lineNumber };
	Point positionFinded;

	if (checkedButton == IDC_RADIO_DOWN) {
		if (dynamic_cast<SelectedState*>(noteState)) {
			index = this->notepadForm->selectedAreaHandler->GetEndPoint().x;
		}
		startPosition.x = index;
		positionFinded = finder.Find(dynamic_cast<Line*>(pattern), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);
	}
	else if (checkedButton == IDC_RADIO_UP) {
		if (dynamic_cast<SelectedState*>(noteState)) {
			index = this->notepadForm->selectedAreaHandler->GetStartPoint().x;
		}
		startPosition.x = index - 1;
		positionFinded = finder.FindBackwards(dynamic_cast<Line*>(pattern), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);
	}

	if (positionFinded.x >= 0) {
		Point endPoint = { positionFinded.x + pattern->GetLength() ,positionFinded.y };
		this->notepadForm->selectedAreaHandler->Select(positionFinded, endPoint);
		
		this->notepadForm->note->Move(positionFinded.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(positionFinded.y);
		this->notepadForm->current->Move(positionFinded.x);

		Long x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, positionFinded.x);
		Long y = this->notepadForm->characterMetrics->GetHeight(positionFinded.y + 1);

		this->notepadForm->scrollController->SmartScrollToPoint(x, y);

		x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, positionFinded.x + pattern->GetLength());
		this->notepadForm->scrollController->SmartScrollToPoint(x, y);
		this->notepadForm->ChangeState(SelectedState::Instance());

		this->notepadForm->Notify();

	}
	else {
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", text.GetBuffer());
		MessageBox((LPCTSTR)message, (LPCTSTR)"메모장", MB_ICONINFORMATION);
	}

}

void FindingForm::OnCancelButtonClicked() {
	EndDialog(0);
}

void FindingForm::OnClose() {
	EndDialog(0);
}


void FindingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;

	if (isCtrl && (nChar == 'H' || nChar == 'h')) {
		EndDialog(0);
		if (this->notepadForm->changingForm->GetSafeHwnd() == NULL) {
			this->notepadForm->changingForm->Create(ChangingForm::IDD);
		}
		CString text;

		GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
		this->notepadForm->changingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->SetWindowTextA((LPCSTR)text.GetBuffer());
		BOOL isCheck = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
		this->notepadForm->changingForm->CheckDlgButton(IDC_CHECK_CASESENSITIVE, isCheck);

		this->notepadForm->changingForm->ShowWindow(SW_SHOW);

		
	}


}


void FindingForm::OnEnChangeEditContentToFind()
{
	CString text;
	GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
	if (text.GetLength() <= 0) {
		((CButton*)GetDlgItem(IDC_BUTTON_NEXTFIND))->EnableWindow(FALSE);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_NEXTFIND))->EnableWindow(TRUE);
	}
}

