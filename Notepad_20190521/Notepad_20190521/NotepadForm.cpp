#include "NotepadForm.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "CaretController.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include <WinUser.h>
#include <imm.h>
#include <afxdlgs.h>
#include "resource.h"
#include "FontCommand.h"
#include "Commands.h"
#include "CommandFactory.h"
#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "ScrollController.h"
#include "SaveAsCommand.h"
#include "SaveCommand.h"
#include "OpenCommand.h"
#include "NewCommand.h"
#include "Scroll.h"
#include "ScrollActionFactory.h"
#include "ScrollActions.h"
#include "AutoLineBreaker.h"
#include "NoteStates.h"
#include "Clipboard.h"
#include "FindingForm.h"
#include "ChangingForm.h"
#include "SelectedAreaHandler.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(IDM_FORMAT_FONT, IDM_AUTO_LINEBREAK, OnCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()

NotepadForm::NotepadForm(){
	this->note = NULL;
	this->current = NULL;
	this->caretController = NULL;
	this->font = NULL;
	this->characterMetrics = NULL;
	this->isComposing = FALSE;
	this->scrollController = NULL;
	this->autoLineBreaker = NULL;
	this->isSaved = TRUE;
	this->noteSize = CSize(0, 0);
	this->noteState = NULL;
	this->clipboard = NULL;
	this->findingForm = NULL;
	this->changingForm = NULL;
	this->selectedAreaHandler = NULL;
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	GlyphFactory glyphFactory;
	this->note = glyphFactory.Make("");
	this->current = glyphFactory.Make("\r\n");
	this->note->Add(this->current);
	this->font = new Font(this);
	this->characterMetrics = new CharacterMetrics(this);
	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&menu);
	this->fileName = "";
	this->noteState = GeneralState::Instance();
	this->clipboard = new Clipboard;
	this->findingForm = new FindingForm(this);
	this->changingForm = new ChangingForm(this);
	this->selectedAreaHandler = new SelectedAreaHandler(this);

	return 0;
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->noteState->OnChar(this, nChar, nRepCnt, nFlags);
}

LRESULT NotepadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[2];
	
	if (lParam & GCS_COMPSTR) {
		buffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		buffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));

		this->noteState->OnImeComposition(this, buffer);
		this->isSaved = FALSE;
		this->Notify();
		this->Invalidate();
		// 스마트스크롤 부분	
		Long x = this->characterMetrics->GetWidth(this->current, this->current->GetCurrent()) + 1; // 
		Long y = this->characterMetrics->GetHeight(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
		this->scrollController->SmartScrollToPoint(x, y);
	}

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[2];
	

	if (IsDBCSLeadByte((BYTE)(wParam >> 8)) == TRUE) {
		buffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		buffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
	}
	else {
		buffer[0] = (TCHAR)wParam;
	}

	this->noteState->OnImeChar(this, buffer);

	return 0;
}

LRESULT NotepadForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return 0;
}

void NotepadForm::OnPaint() {
	Glyph *line;
	string content;
	string subContent;
	CPaintDC dc(this);
	CFont *oldFont;
	COLORREF oldTextColor;
	COLORREF oldBkColor = dc.GetBkColor();
	COLORREF backgroundColor;

	CFont font;
	this->font->Create(font);
	oldFont = dc.SelectObject(&font);
	oldTextColor = dc.SetTextColor(this->font->GetColor());

	Long x = this->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->scrollController->GetVerticalScroll()->GetPosition();

	RECT rect;
	this->GetClientRect(&rect);
	Long clientTop = y;
	Long clientBottom = (rect.bottom - rect.top) + y;

	Point startPoint = { 0,0 };
	Point endPoint = { 0,0 };

	Long row;
	Long currentX;
	
	if (dynamic_cast<SelectedState*>(this->noteState)) {
		startPoint = this->selectedAreaHandler->GetStartPoint();
		endPoint = this->selectedAreaHandler->GetEndPoint();

	}
	if (startPoint.y > endPoint.y || (startPoint.y == endPoint.y && startPoint.x > endPoint.x)) {
		startPoint = this->selectedAreaHandler->GetEndPoint();
		endPoint = this->selectedAreaHandler->GetStartPoint();
	}

	Long begin = this->characterMetrics->GetRow(y);
	Long end = this->characterMetrics->GetRow(clientBottom);


	Long i = begin;
	while (i < this->note->GetLength() && i < end) {
		line = this->note->GetAt(i);
		row = i * (this->characterMetrics->GetHeight()) - y;

		if (dynamic_cast<SelectedState*>(this->noteState) && startPoint.y <= i && i <= endPoint.y) {// 선택 상태이고 선택 영역이 포함된 줄이면
			currentX = 0;
			Long j = 0;
			while (j < line->GetLength()) {// 줄의 글의 개수만큼 반복한다.
				backgroundColor = oldBkColor;
				if (startPoint.y == endPoint.y && startPoint.x <= j && j < endPoint.x) {// 선택 영역이 한줄 안에 있고 글이 시작점 칸과 끝점 칸 사이에 있으면 
					backgroundColor = RGB(0x99, 0xCC, 0xFF);
				}
				// 선택 영역이 두 줄 이상이고 현재 줄이 선택 시작점이 있는 줄이고 선택 시작칸보다 크거나 같으면
				else if (startPoint.y < endPoint.y && startPoint.y == i && startPoint.x <= j) {
					backgroundColor = RGB(0x99, 0xCC, 0xFF);
				}
				// 선택 영역이 두 줄 이상이고 현재 줄이 선택 끝점이 있는 줄이고 선택 끝칸보다 작거나 같으면
				else if (startPoint.y < endPoint.y && endPoint.y == i && j < endPoint.x) {
					backgroundColor = RGB(0x99, 0xCC, 0xFF);
				}
				// 선택 영역이 두 줄 이상이고 현재 줄이 선택 시작점과 끝점 사이에 있는 줄이면
				else if (startPoint.y < endPoint.y && startPoint.y < i && i < endPoint.y) {
					backgroundColor = RGB(0x99, 0xCC, 0xFF);
				}
				
				dc.SetBkColor(backgroundColor);
				Glyph *character = line->GetAt(j);
				content = character->GetContent();
				dc.TextOutA(-x + currentX, row, content.c_str());
				dc.SetBkColor(oldBkColor);
				currentX += this->characterMetrics->GetWidth(content);
				j++;
			}

		}
		else {
			content = line->GetContent();
			dc.TextOutA(-x, row, content.c_str());
		}
		
		i++;
	}
	
	dc.SelectObject(oldFont);
	dc.SetTextColor(oldTextColor);
	
}

void NotepadForm::OnSetFocus(CWnd *pNewWnd) {
	if (this->caretController == NULL) {
		this->caretController = new CaretController(this);
		
	}
	this->Notify();

}

void NotepadForm::OnKillFocus(CWnd *pNewWnd) {
	if (this->caretController != NULL) {
		delete this->caretController;
		this->caretController = NULL;
	}
}

void NotepadForm::OnCommandRange(UINT uID) {
	CommandFactory commandFactory;
	
	Command *command = commandFactory.Make(uID, this);
	if (command != NULL) {
		command->Execute();
		delete command;
	}
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->noteState->OnKeyDown(this, nChar, nRepCnt, nFlags);
}

void NotepadForm::OnLButtonDown(UINT nFlag, CPoint point) {
	this->noteState->OnLButtonDown(this, nFlag, point.x, point.y);
}

void NotepadForm::OnLButtonDblClk(UINT nFlags, CPoint point) {
	this->noteState->OnLButtonDblClk(this, nFlags, point.x, point.y);
}



void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	ScrollActionFactory scrollFactory(this);
	ScrollAction *hScrollAction = scrollFactory.MakeHScrollAction(nSBCode);
	if (hScrollAction != NULL) {
		hScrollAction->OnScroll(nSBCode, nPos, pScrollBar);
		delete hScrollAction;
	}
	this->Invalidate();
}

void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	ScrollActionFactory scrollActionFactory(this);
	
	ScrollAction *vScrollAction = scrollActionFactory.MakeVScrollAction(nSBCode);
	if (vScrollAction != NULL) {
		vScrollAction->OnScroll(nSBCode, nPos, pScrollBar);
		delete vScrollAction;
	}
	this->Invalidate();

}

void NotepadForm::OnMouseMove(UINT nFlags, CPoint point) {
	this->noteState->OnMouseMove(this, nFlags, point.x, point.y);
}

BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {

	LONG style = this->GetStyle();

	if ((style & WS_VSCROLL) !=0) {
		Long position = this->scrollController->Rotate(zDelta);
		Long previousPosition = this->SetScrollPos(SB_VERT, position, TRUE);
		position = this->GetScrollPos(SB_VERT);
		this->scrollController->MoveVerticalScroll(position);
		this->ScrollWindow(0, previousPosition - position);
		this->Notify();
	}
	return TRUE;
}

void NotepadForm::OnSize(UINT nType, int cs, int cy) {
	if (this->scrollController == NULL) {
		this->scrollController = new ScrollController(this);
	}
	if (this->note != NULL) {
		if (this->autoLineBreaker != 0) {
			this->autoLineBreaker->UpdateAll();
		}
		
		this->Notify();
	}
	
}

void NotepadForm::OnClose() {
	if (this->characterMetrics != NULL) {
		delete this->characterMetrics;
		this->characterMetrics = NULL;
	}
	if (this->font != NULL) {
		delete this->font;
	}
	if (this->note != NULL) {
		delete this->note;
		this->note = NULL;
	}
	if (this->clipboard != NULL) {
		delete this->clipboard;
		this->clipboard = NULL;
	}
	if (this->findingForm != NULL) {
		delete this->findingForm;
		this->findingForm = NULL;
	}

	if (this->changingForm != NULL) {
		delete this->changingForm;
		this->changingForm = NULL;
	}
	if (this->selectedAreaHandler != NULL) {
		delete this->selectedAreaHandler;
		this->selectedAreaHandler = NULL;
	}

	CFrameWnd::OnClose();
}

void NotepadForm::ChangeState(NoteState * noteState) {
	this->noteState = noteState;
}