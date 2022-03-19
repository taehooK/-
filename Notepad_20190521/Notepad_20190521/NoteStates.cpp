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
		// ����Ʈ��ũ�� �κ�	
		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
		notepadForm->scrollController->SmartScrollToPoint(x, y);
		
	}
}

void GeneralState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	NoteState::OnImeComposition(notepadForm, buffer);

	ChangeState(notepadForm, ComposingState::Instance());
	notepadForm->isSaved = FALSE;
	notepadForm->Notify();
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
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
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
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
	if (isShift) {// 1. ����ƮŰ�� ������������
		// 1.1.ó�������� ���Ѵ�.
		Point end = { column, index };// 1.2. �������� ���Ѵ�.
		notepadForm->selectedAreaHandler->Select(start, end);// 1.3. �޸��� �������� �޸��忡�� �����Ѵ�.
		ChangeState(notepadForm, SelectedState::Instance());// 1.4. �޸��� �����츦 ���û��·� �����Ѵ�.
	}

	notepadForm->Notify();
}

void GeneralState::OnLButtonDblClk(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	//2. ���콺 �¹�ư�� ����Ŭ�� ���� ��
	//2.1.�������� ���� ���� �ش�Ǵ� ���ڰ� ���Ե� �ܾ��� �������� ã�´�.
	Point startPoint = { notepadForm->current->MovePreviousWord(), notepadForm->note->GetCurrent() };
	//2.2.���� �������� ���� �ܾ� ���� ������ ã�´�.
	Point endPoint = { notepadForm->current->MoveNextWord(), notepadForm->note->GetCurrent() };
	//2.3.�޸��� �������� �޸��忡�� �����Ѵ�.
	notepadForm->selectedAreaHandler->Select(startPoint, endPoint);
	ChangeState(notepadForm, SelectedState::Instance());
	//2.4 �����Ѵ�.
	notepadForm->Notify();
}

void GeneralState::OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	if (nFlags & MK_LBUTTON) {// 1. ���� ��ư�� ������ �ִ� �����̸�
		Point startPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
		//  1.1. Ŀ�� ��ǥ�� ����� ���� ã�´�.
		Long lineNumber = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
		if (lineNumber >= notepadForm->note->GetLength()) {
			lineNumber = notepadForm->note->GetLength() - 1;
		}
		//  1.2. �޸��� �������� �޸��忡�� �̵��Ѵ�.
		notepadForm->note->Move(lineNumber);
		//  1.3. �޸��� �������� current�� �����Ѵ�.
		notepadForm->current = notepadForm->note->GetAt(lineNumber);
		//  1.3. �޸��� �������� current���� Ŀ�� ��ǥ�� ����� ���ڸ� ã�´�.
		Long characterNumber = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
		//  1.4. �޸��� �������� current���� �̵��Ѵ�.
		notepadForm->current->Move(characterNumber);
		//  1.5. �޸��� �������� �޸��忡�� �����Ѵ�.
		Point endPoint = { characterNumber, lineNumber };
		
		if (endPoint.x != startPoint.x || endPoint.y != startPoint.y) { // ���� ������ �޶��� ��쿡�� ���� ���ù� ����
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

		// ���� OnChar ����
		NoteState::OnChar(notepadForm, nChar, nRepCnt, nFlags);

		// �Ϲݻ��·� ����
		ChangeState(notepadForm, GeneralState::Instance());

		notepadForm->isSaved = FALSE;
		notepadForm->Notify();

		// ����Ʈ��ũ�� �κ�	
		Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
		notepadForm->scrollController->SmartScrollToPoint(x, y);
	}

	
}

void SelectedState::OnImeComposition(NotepadForm *notepadForm, char(*buffer)){
	notepadForm->selectedAreaHandler->Remove();

	NoteState::OnImeComposition(notepadForm, buffer);
	ChangeState(notepadForm, ComposingState::Instance());

	notepadForm->isSaved = FALSE;
	notepadForm->Notify();

	// ����Ʈ��ũ�� �κ�	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
	notepadForm->scrollController->SmartScrollToPoint(x, y);
}

void SelectedState::OnImeChar(NotepadForm *notepadForm, char(*buffer)){
}

void SelectedState::OnKeyDown(NotepadForm *notepadForm, UINT nChar, UINT nRepCnt, UINT nFlags){
	KeyActionFactory keyActionFactory(notepadForm);
	KeyAction *keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		

		// ���� �̺�Ʈ�� ���� �̺�Ʈ �׸��� ã�� �ٲٱ� �̺�Ʈ�� �� �ƴϸ� �Ϲݻ��·� ����
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
		Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
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
	if (isShift) {//1. ShiftŰ�� ������������
		Point start = notepadForm->selectedAreaHandler->GetStartPoint();// 1.1. ó�������� ���Ѵ�.
		Point end = { column, index }; // 1.2. �������� ���Ѵ�.
		notepadForm->selectedAreaHandler->Select(start, end);// 1.3. �޸��� �������� �޸��忡�� �����Ѵ�.
	}
	else {//2. ShiftŰ�� ���������� ������
		ChangeState(notepadForm, GeneralState::Instance());// 2.1. �޸��� �����츦 �Ϲݻ��·� �����Ѵ�.
	}
	notepadForm->Notify();
}

void SelectedState::OnMouseMove(NotepadForm *notepadForm, UINT nFlags, Long pointX, Long pointY){
	if (nFlags & MK_LBUTTON) {// 1. ���� ��ư�� ������ �ִ� �����̸�
		Point currentPoint = { notepadForm->current->GetCurrent(), notepadForm->note->GetCurrent() };
		//  1.1. Ŀ�� ��ǥ�� ����� ���� ã�´�.
		Long lineNumber = notepadForm->characterMetrics->GetRow(notepadForm->scrollController->GetVerticalScroll()->GetPosition() + pointY);
		if (lineNumber >= notepadForm->note->GetLength()) {
			lineNumber = notepadForm->note->GetLength() - 1;
		}
		//  1.2. �޸��� �������� �޸��忡�� �̵��Ѵ�.
		notepadForm->note->Move(lineNumber);
		//  1.3. �޸��� �������� current�� �����Ѵ�.
		notepadForm->current = notepadForm->note->GetAt(lineNumber);
		//  1.3. �޸��� �������� current���� Ŀ�� ��ǥ�� ����� ���ڸ� ã�´�.
		Long characterNumber = notepadForm->characterMetrics->GetColumn(notepadForm->current, notepadForm->scrollController->GetHorizontalScroll()->GetPosition() + pointX);
		//  1.4. �޸��� �������� current���� �̵��Ѵ�.
		notepadForm->current->Move(characterNumber);
		//  1.5. �޸��� �������� �޸��忡�� �����Ѵ�.
		Point endPoint = { characterNumber, lineNumber };

		if (endPoint.x != currentPoint.x || endPoint.y != currentPoint.y) { // ���� ������ �޶��� ��쿡�� ���� ���ù� ����
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

	// ����Ʈ��ũ�� �κ�	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
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
	// ����Ʈ��ũ�� �κ�	
	Long x = notepadForm->characterMetrics->GetWidth(notepadForm->current, notepadForm->current->GetCurrent()) + 1; // 
	Long y = notepadForm->characterMetrics->GetHeight(notepadForm->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
	notepadForm->scrollController->SmartScrollToPoint(x, y);

	ChangeState(notepadForm, GeneralState::Instance());


}




