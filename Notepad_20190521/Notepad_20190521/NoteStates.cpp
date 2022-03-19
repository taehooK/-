//NoteState.cpp

#include "NoteStates.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "KeyActionFactory.h"
#include "Scroll.h"
#include "KeyActions.h"
#include "AutoLineBreaker.h"
#include "SelectedAreaHandler.h"
#include "EditWorks.h"


void NoteState::OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){
	GlyphFactory glyphFactory;
	TCHAR content[2];
	Long index = notepadForm->current->GetCurrent();
	

	if (nChar >= 32 || nChar == VK_TAB) {
		content[0] = nChar;
		Glyph *character = glyphFactory.Make(content);

		if (index >= notepadForm->current->GetLength()) {
			notepadForm->current->Add(character);
		}
		else {
			notepadForm->current->Add(index, character);
		}
		notepadForm->editorialMemory.Push(ADD, character->Clone(), notepadForm->note->GetCurrent(), index);
	}
	else if (nChar == VK_RETURN) {

		if (index < notepadForm->current->GetLength()) {
			notepadForm->current = notepadForm->current->Divide(index);
			index = notepadForm->note->GetCurrent();
			notepadForm->note->Add(index + 1, notepadForm->current);
			notepadForm->current->First();
		}
		else {
			notepadForm->current = glyphFactory.Make("\r\n");
			index = notepadForm->note->GetCurrent();
			notepadForm->note->Add(index + 1, notepadForm->current);
		}	
		notepadForm->editorialMemory.Push(ADD, notepadForm->current->Clone(), notepadForm->note->GetCurrent(), 0);
	}

}
void NoteState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	Long index;

	if (buffer[0] != '\0') {
		GlyphFactory glyphFactory;
		Glyph *doubleByteCharacter = glyphFactory.Make(buffer);
		index = notepadForm->current->GetCurrent();

		if (index >= notepadForm->current->GetLength()) {
			notepadForm->current->Add(doubleByteCharacter);
		}
		else {
			notepadForm->current->Add(index, doubleByteCharacter);
		}
	}
}
void NoteState::OnImeChar(NotepadForm *notepadForm, char(*buffer)){}
void NoteState::OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){}
void NoteState::OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){}
void NoteState::OnLButtonDblClk(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){}
void NoteState::OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){}

void NoteState::ChangeState(NotepadForm *notepadForm, NoteState *noteState) {
	notepadForm->ChangeState(noteState);
}

//GeneralState.cpp

GeneralState *GeneralState::instance = 0;
GeneralState* GeneralState::Instance() {
	if (instance == 0) {
		instance = new GeneralState;
	}
	return instance;
}

GeneralState::GeneralState() {
}

void GeneralState::OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags) {

	if (nChar >= 32 || nChar == VK_TAB || nChar == VK_RETURN) {
		NoteState::OnChar(notepadForm, nChar, nRepCnt, nFlags);
		notepadForm->isSaved = FALSE;
		
		notepadForm->Notify();
		// 스마트스크롤 부분	
		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
		notepadForm->scrollController->SmartScrollToPoint(x, y);
		
	}
}

void GeneralState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	NoteState::OnImeComposition(notepadForm, buffer);

	ChangeState(notepadForm, ComposingState::Instance());
	notepadForm->isSaved = FALSE;
	notepadForm->Notify();
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	notepadForm->scrollController->SmartScrollToPoint(x, y);
}


void GeneralState::OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){
	KeyActionFactory keyActionFactory(notepadForm);
	KeyAction *keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		Point startPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		

		if (dynamic_cast<ShiftLeftKeyAction*>(keyAction) || dynamic_cast<ShiftRightKeyAction*>(keyAction) ||
			dynamic_cast<ShiftUpKeyAction*>(keyAction) || dynamic_cast<ShiftDownKeyAction*>(keyAction)
			|| dynamic_cast<CtrlShiftHomeKeyAction*>(keyAction) || dynamic_cast<CtrlShiftEndKeyAction*>(keyAction) ||
			dynamic_cast<ShiftHomeKeyAction*>(keyAction) || dynamic_cast<ShiftEndKeyAction*>(keyAction) ||
			dynamic_cast<CtrlShiftLeftKeyAction*>(keyAction) || dynamic_cast<CtrlShiftRightKeyAction*>(keyAction) ||
			dynamic_cast<CtrlAKeyAction*>(keyAction)) {
			Point endPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
			if (!dynamic_cast<CtrlAKeyAction*>(keyAction)) {
				notepadForm->selectedAreaHandler->Select(startPoint, endPoint);
			}
			ChangeState(notepadForm, SelectedState::Instance());
		}

		delete keyAction;

		notepadForm->Notify();

		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
		notepadForm->scrollController->SmartScrollToPoint(x, y);
		
	}
}

void GeneralState::OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	Point start = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };

	Long row = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
	if (row >= notepadForm->note->GetLength()) {
		row = notepadForm->note->GetLength() - 1;
	}
	Long index = notepadForm->note->Move(row);
	notepadForm->current = notepadForm->note->GetAt(index);
	Long column = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
	notepadForm->current->Move(column);
	
	SHORT isShift = GetKeyState(VK_SHIFT) & 0x8000;
	if (isShift) {// 1. 쉬프트키가 눌려져있으면
		// 1.1.처음지점을 정한다.
		Point end = { column, index };// 1.2. 끝지점을 정한다.
		notepadForm->selectedAreaHandler->Select(start, end);// 1.3. 메모장 윈도우의 메모장에서 선택한다.
		ChangeState(notepadForm, SelectedState::Instance());// 1.4. 메모장 윈도우를 선택상태로 변경한다.
	}

	notepadForm->Notify();
}

void GeneralState::OnLButtonDblClk(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	//2. 마우스 좌버튼을 더블클릭 했을 때
	//2.1.현재줄의 현재 열에 해당되는 글자가 포함된 단어의 시작점을 찾는다.
	Point startPoint = { notepadForm->current->MovePreviousWord(), notepadForm->note->GetCurrent() };
	//2.2.열을 기준으로 다음 단어 이전 지점을 찾는다.
	Point endPoint = { notepadForm->current->MoveNextWord(), notepadForm->note->GetCurrent() };
	//2.3.메모장 윈도우의 메모장에서 선택한다.
	notepadForm->selectedAreaHandler->Select(startPoint, endPoint);
	ChangeState(notepadForm, SelectedState::Instance());
	//2.4 통지한다.
	notepadForm->Notify();
}

void GeneralState::OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	if (nFlags & MK_LBUTTON) {// 1. 왼쪽 버튼이 눌려져 있는 상태이면
		Point startPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
		//  1.1. 커서 좌표와 가까운 줄을 찾는다.
		Long lineNumber = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
		if (lineNumber >= notepadForm->note->GetLength()) {
			lineNumber = notepadForm->note->GetLength() - 1;
		}
		//  1.2. 메모장 윈도우의 메모장에서 이동한다.
		notepadForm->note->Move(lineNumber);
		//  1.3. 메모장 윈도우의 current를 변경한다.
		notepadForm->current = notepadForm->note->GetAt(lineNumber);
		//  1.3. 메모장 윈도우의 current에서 커서 좌표와 가까운 글자를 찾는다.
		Long characterNumber = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
		//  1.4. 메모장 윈도우의 current에서 이동한다.
		notepadForm->current->Move(characterNumber);
		//  1.5. 메모장 윈도우의 메모장에서 선택한다.
		Point endPoint = { characterNumber, lineNumber };
		
		if (endPoint.x != startPoint.x || endPoint.y != startPoint.y) { // 선택 범위가 달라진 경우에만 새로 선택및 갱신
			notepadForm->selectedAreaHandler->Select(startPoint, endPoint);
			ChangeState(notepadForm, SelectedState::Instance());
			notepadForm->Notify();
		}
	}
}


//SelectedState.cpp
SelectedState *SelectedState::instance = 0;
SelectedState* SelectedState::Instance() {
	if (instance == 0) {
		instance = new SelectedState;
	}
	return instance;
}

SelectedState::SelectedState() {

}

void SelectedState::OnChar(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){
	if (nChar >= 32 || nChar == VK_TAB || nChar == VK_RETURN) {
		notepadForm->selectedAreaHandler->Remove();

		// 기존 OnChar 동작
		NoteState::OnChar(notepadForm, nChar, nRepCnt, nFlags);

		// 일반상태로 변경
		ChangeState(notepadForm, GeneralState::Instance());

		notepadForm->isSaved = FALSE;
		notepadForm->Notify();

		// 스마트스크롤 부분	
		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
		notepadForm->scrollController->SmartScrollToPoint(x, y);
	}

	
}

void SelectedState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	notepadForm->selectedAreaHandler->Remove();

	NoteState::OnImeComposition(notepadForm, buffer);
	ChangeState(notepadForm, ComposingState::Instance());

	notepadForm->isSaved = FALSE;
	notepadForm->Notify();

	// 스마트스크롤 부분	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	notepadForm->scrollController->SmartScrollToPoint(x, y);
}

void SelectedState::OnImeChar(NotepadForm *notepadForm, char(*buffer)){
}

void SelectedState::OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){
	KeyActionFactory keyActionFactory(notepadForm);
	KeyAction *keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		

		// 선택 이벤트와 복사 이벤트 그리고 찾기 바꾸기 이벤트가 가 아니면 일반상태로 변경
		if (!(dynamic_cast<ShiftLeftKeyAction*>(keyAction) || dynamic_cast<ShiftRightKeyAction*>(keyAction) ||
			dynamic_cast<ShiftUpKeyAction*>(keyAction) || dynamic_cast<ShiftDownKeyAction*>(keyAction)
			|| dynamic_cast<CtrlShiftHomeKeyAction*>(keyAction) || dynamic_cast<CtrlShiftEndKeyAction*>(keyAction) ||
			dynamic_cast<ShiftHomeKeyAction*>(keyAction) || dynamic_cast<ShiftEndKeyAction*>(keyAction) ||
			dynamic_cast<CtrlShiftLeftKeyAction*>(keyAction) || dynamic_cast<CtrlShiftRightKeyAction*>(keyAction) ||
			dynamic_cast<CtrlAKeyAction*>(keyAction) || dynamic_cast<CtrlCKeyAction*>(keyAction) || dynamic_cast<CtrlFKeyAction*>(keyAction) || dynamic_cast<CtrlHKeyAction*>(keyAction))) {
			ChangeState(notepadForm, GeneralState::Instance());	
		}

		delete keyAction;

		notepadForm->Notify();
		
		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
		notepadForm->scrollController->SmartScrollToPoint(x, y);
	}
}

void SelectedState::OnLButtonDown(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	Long row = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
	if (row >= notepadForm->note->GetLength()) {
		row = notepadForm->note->GetLength() - 1;
	}
	Long index = notepadForm->note->Move(row);
	notepadForm->current = notepadForm->note->GetAt(index);
	Long column = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
	notepadForm->current->Move(column);

	SHORT isShift = GetKeyState(VK_SHIFT) & 0x8000;
	if (isShift) {//1. Shift키가 눌러져있으면
		Point start = notepadForm->selectedAreaHandler->GetStartPoint();// 1.1. 처음지정을 정한다.
		Point end = { column, index }; // 1.2. 끝지점을 정한다.
		notepadForm->selectedAreaHandler->Select(start, end);// 1.3. 메모장 윈도우의 메모장에서 선택한다.
	}
	else {//2. Shift키가 눌려져있지 않으면
		ChangeState(notepadForm, GeneralState::Instance());// 2.1. 메모장 윈도우를 일반상태로 변경한다.
	}
	notepadForm->Notify();
}

void SelectedState::OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	if (nFlags & MK_LBUTTON) {// 1. 왼쪽 버튼이 눌려져 있는 상태이면
		Point currentPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
		//  1.1. 커서 좌표와 가까운 줄을 찾는다.
		Long lineNumber = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
		if (lineNumber >= notepadForm->note->GetLength()) {
			lineNumber = notepadForm->note->GetLength() - 1;
		}
		//  1.2. 메모장 윈도우의 메모장에서 이동한다.
		notepadForm->note->Move(lineNumber);
		//  1.3. 메모장 윈도우의 current를 변경한다.
		notepadForm->current = notepadForm->note->GetAt(lineNumber);
		//  1.3. 메모장 윈도우의 current에서 커서 좌표와 가까운 글자를 찾는다.
		Long characterNumber = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
		//  1.4. 메모장 윈도우의 current에서 이동한다.
		notepadForm->current->Move(characterNumber);
		//  1.5. 메모장 윈도우의 메모장에서 선택한다.
		Point endPoint = { characterNumber, lineNumber };

		if (endPoint.x != currentPoint.x || endPoint.y != currentPoint.y) { // 선택 범위가 달라진 경우에만 새로 선택및 갱신
			notepadForm->selectedAreaHandler->Select(notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
			notepadForm->Notify();
		}
	}
}

//ComposingState.cpp
ComposingState *ComposingState::instance = 0;
ComposingState* ComposingState::Instance() {
	if (instance == 0) {
		instance = new ComposingState;
	}
	return instance;
}

ComposingState::ComposingState() {
}

void ComposingState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	
	Long index = notepadForm->current->GetCurrent();
	notepadForm->current->Remove(index - 1);
	
	if (buffer[0] == '\0') {
		ChangeState(notepadForm, GeneralState::Instance());
	}
	else {
		NoteState::OnImeComposition(notepadForm, buffer);
	}

	notepadForm->isSaved = FALSE;
	notepadForm->Notify();

	// 스마트스크롤 부분	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	notepadForm->scrollController->SmartScrollToPoint(x, y);
	
}

void ComposingState::OnImeChar(NotepadForm *notepadForm, char(*buffer)){
	Long index = notepadForm->current->GetCurrent() -1;
	notepadForm->current->Remove(index);

	GlyphFactory glyphFactory;
	Glyph *glyph = glyphFactory.Make(buffer);
	
	if (notepadForm->current->GetCurrent() >= notepadForm->current->GetLength()) {
		notepadForm->current->Add(glyph);
	}
	else {
		notepadForm->current->Add(index, glyph);
	}

	notepadForm->editorialMemory.Push(ADD, glyph->Clone(), notepadForm->note->GetCurrent(), index);
	
	notepadForm->isSaved = FALSE;
	notepadForm->Notify();
	notepadForm->Invalidate();
	// 스마트스크롤 부분	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	notepadForm->scrollController->SmartScrollToPoint(x, y);

	ChangeState(notepadForm, GeneralState::Instance());


}




