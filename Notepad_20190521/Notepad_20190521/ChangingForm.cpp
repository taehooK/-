#include "ChangingForm.h"
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
#include "FindingForm.h"
#include "SelectedAreaHandler.h"

BEGIN_MESSAGE_MAP(ChangingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEXTFIND, OnNexFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnChangeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CHANGE, OnAllChangeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()

	ON_EN_CHANGE(IDC_EDIT_CONTENT_TO_FIND, OnEnChangeEditContentToFind)

END_MESSAGE_MAP()


ChangingForm::ChangingForm(NotepadForm *notepadForm, CWnd *parent)
:CDialog(ChangingForm::IDD, parent){
	this->notepadForm = notepadForm;
}

BOOL ChangingForm::OnInitDialog(){
	CDialog::OnInitDialog();
	CheckDlgButton(IDC_CHECK_CASESENSITIVE, 1);

	return FALSE;
}

BOOL ChangingForm::PreTranslateMessage(MSG *pMsg) {
	BOOL ret = CDialog::PreTranslateMessage(pMsg);

	if (pMsg->message == WM_KEYDOWN) {
		UINT nRepCnt = (UINT)((pMsg->lParam) & 0x0000FFFF);
		UINT nFlags = (UINT)((pMsg->lParam) >> 16);
		this->OnKeyDown(pMsg->wParam, nRepCnt, nFlags);
		ret = TRUE;
	}

	return ret;
}

void ChangingForm::OnNexFindButtonClicked(){
	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;

	if (!isCtrl) {
		CString text;

		GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);

		GlyphFactory glyphFactory;
		Glyph *pattern = glyphFactory.MakeLine(text.GetBuffer());

		int checkedButton = GetCheckedRadioButton(IDC_RADIO_UP, IDC_RADIO_DOWN);
		BOOL check_caseSensitive = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);

		Long lineNumber = this->notepadForm->note->GetCurrent();
		Finder finder;
		Long index = this->notepadForm->current->GetCurrent();
		NoteState *noteState = this->notepadForm->GetNoteState();
		Point startPosition = { 0, lineNumber };
		Point positionFinded;


		if (dynamic_cast<SelectedState*>(noteState)) {
			index = this->notepadForm->selectedAreaHandler->GetEndPoint().x;
		}
		startPosition.x = index;
		positionFinded = finder.Find(dynamic_cast<Line*>(pattern), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);


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

		if (pattern != 0) {
			delete pattern;
		}
	}
	

}

void ChangingForm::OnChangeButtonClicked(){
	CString contentToFind;
	CString contentToBeChanged;

	BOOL check_caseSensitive = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
	GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(contentToFind);
	GetDlgItem(IDC_EDIT_CONTENT_TO_BE_CHANGED)->GetWindowTextA(contentToBeChanged);

	GlyphFactory glyphFactory;
	Glyph *pattern = glyphFactory.MakeLine(contentToFind.GetBuffer());
	Glyph *glyphToReplace = glyphFactory.MakeLine(contentToBeChanged.GetBuffer());

	Point startPosition;
	Point startPoint;
	Point endPoint;
	NoteState *noteState = this->notepadForm->GetNoteState();
	Glyph *line;
	Long patternLength;
	Finder finder;

	if (dynamic_cast<SelectedState*>(noteState)) {
		startPoint = this->notepadForm->selectedAreaHandler->GetStartPoint();
		endPoint = this->notepadForm->selectedAreaHandler->GetEndPoint();

		if (startPoint.y == endPoint.y) {
			line = this->notepadForm->note->GetAt(startPoint.y);
			patternLength = pattern->GetLength();
			Long i = 0;
			while(endPoint.x - startPoint.x == patternLength && i < patternLength 
				&& finder.isSameCharacter(line->GetAt(i + startPoint.x), pattern->GetAt(i), check_caseSensitive)) {
				i++;
			}

			if (i >= patternLength) {
				line->Remove(startPoint.x, endPoint.x - startPoint.x);
				Long j = 0;
				while (j < glyphToReplace->GetLength()) {
					line->Add(startPoint.x + j, glyphToReplace->GetAt(j)->Clone());
					j++;
				}
				endPoint.x = startPoint.x + glyphToReplace->GetLength();
				this->notepadForm->selectedAreaHandler->Select(startPoint, endPoint);
				this->notepadForm->Notify();
			}
		}
		startPosition = endPoint;
	}
	else {
		startPosition = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	}

	Point findedPosition = finder.Find(dynamic_cast<Line*>(pattern), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);

	if (findedPosition.x >= 0) {
		endPoint = { findedPosition.x + pattern->GetLength() ,findedPosition.y };
		this->notepadForm->selectedAreaHandler->Select(findedPosition, endPoint);
		this->notepadForm->ChangeState(SelectedState::Instance());
		this->notepadForm->note->Move(findedPosition.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(findedPosition.y);
		this->notepadForm->current->Move(findedPosition.x);

		Long x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, findedPosition.x);
		Long y = this->notepadForm->characterMetrics->GetHeight(findedPosition.y + 1);

		this->notepadForm->scrollController->SmartScrollToPoint(x, y);

		x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, findedPosition.x + pattern->GetLength());
		this->notepadForm->scrollController->SmartScrollToPoint(x, y);
		this->notepadForm->Notify();
	
	}
	else {
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", contentToFind.GetBuffer());
		MessageBox((LPCTSTR)message, (LPCTSTR)"메모장", MB_ICONINFORMATION);
	}

	if (pattern != 0) {
		delete pattern;
	}
	if (glyphToReplace != 0) {
		delete glyphToReplace;
	}

}

void ChangingForm::OnAllChangeButtonClicked(){
	CString pattern;
	CString contentTobeReplaced;

	BOOL check_caseSensitive = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
	GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(pattern);
	GetDlgItem(IDC_EDIT_CONTENT_TO_BE_CHANGED)->GetWindowTextA(contentTobeReplaced);

	GlyphFactory glyphFactory;

	Glyph *patternGlyph = glyphFactory.MakeLine(pattern.GetBuffer());
	Glyph *glyphToBeReplaced = glyphFactory.MakeLine(contentTobeReplaced.GetBuffer());

	Finder finder;

	Point startPosition = { 0, 0 };
	Point findedPosition = finder.Find(dynamic_cast<Line*>(patternGlyph), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);

	while (findedPosition.x >= 0) {
		Glyph *line = this->notepadForm->note->GetAt(findedPosition.y);
		line->Remove(findedPosition.x, patternGlyph->GetLength());
		Long i = 0;
		while (i < glyphToBeReplaced->GetLength()) {
			line->Add(findedPosition.x + i, glyphToBeReplaced->GetAt(i)->Clone());
			i++;
		}
		startPosition = { findedPosition.x + glyphToBeReplaced->GetLength(), findedPosition.y };
		findedPosition = finder.Find(dynamic_cast<Line*>(patternGlyph), dynamic_cast<Note*>(this->notepadForm->note), startPosition, check_caseSensitive);
	}

	Long index = this->notepadForm->note->Move(0);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Move(0);

	this->notepadForm->scrollController->SmartScrollToPoint(0, 0);
	this->notepadForm->ChangeState(GeneralState::Instance());
	this->notepadForm->Notify();


	if (patternGlyph != 0) {
		delete patternGlyph;
	}
	if (glyphToBeReplaced != 0) {
		delete glyphToBeReplaced;
	}

}

void ChangingForm::OnCancelButtonClicked(){
	EndDialog(0);
}

void ChangingForm::OnEnChangeEditContentToFind(){
	CString text;
	GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
	if (text.GetLength() <= 0) {
		((CButton*)GetDlgItem(IDC_BUTTON_NEXTFIND))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CHANGE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_ALL_CHANGE))->EnableWindow(FALSE);

	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_NEXTFIND))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_CHANGE))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_ALL_CHANGE))->EnableWindow(TRUE);
	}
}

void ChangingForm::OnClose(){
	EndDialog(0);
}

void ChangingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;

	if (isCtrl && (nChar == 'F' || nChar == 'F')) {
		
		if (this->notepadForm->findingForm->GetSafeHwnd() == NULL) {
			this->notepadForm->findingForm->Create(FindingForm::IDD);
		}

		CString text;

		GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
		BOOL isCheck = IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
		this->notepadForm->findingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->SetWindowTextA((LPCSTR)text.GetBuffer());
		this->notepadForm->findingForm->CheckDlgButton(IDC_CHECK_CASESENSITIVE, isCheck);

		EndDialog(0);
		this->notepadForm->findingForm->ShowWindow(SW_SHOW);

		
	}
}

void ChangingForm::OnOK(){
}