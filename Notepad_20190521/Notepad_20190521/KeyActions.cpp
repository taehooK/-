#include "KeyActions.h"
#include "KeyActionFactory.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "Character.h"
#include "Note.h"
#include "Line.h"
#include "Composite.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "CaretController.h"
#include "Caret.h"
#include "resource.h"
#include "AutoLineBreaker.h"
#include "DummyLine.h"
#include "Clipboard.h"
#include "NoteStates.h"
#include "GlyphFactory.h"
#include "Scanner.h"
#include "Character.h"
#include "FindingForm.h"
#include "ChangingForm.h"
#include "SelectedAreaHandler.h"

KeyAction::KeyAction(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->notepadForm = source.notepadForm;
}

KeyAction::~KeyAction() {

}

KeyAction& KeyAction::operator=(const KeyAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

//LeftKeyAction
LeftKeyAction::LeftKeyAction(NotepadForm *notepadForm) 
:KeyAction(notepadForm){

}

LeftKeyAction::LeftKeyAction(const LeftKeyAction& source)
	: KeyAction(notepadForm) {

}

LeftKeyAction::~LeftKeyAction() {

}

void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->current->GetCurrent() > 0) {
		this->notepadForm->current->Previous();
	}
	else if(this->notepadForm->note->GetCurrent() > 0){
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Last();
	}
}

LeftKeyAction& LeftKeyAction::operator = (const LeftKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//RightKeyAction
RightKeyAction::RightKeyAction(NotepadForm *notepadForm) 
:KeyAction(notepadForm){
}

RightKeyAction::RightKeyAction(const RightKeyAction& source)
	:KeyAction(notepadForm) {
}

RightKeyAction::~RightKeyAction() {
}

void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->current->GetCurrent() < this->notepadForm
		->current->GetLength()) {
		this->notepadForm->current->Next();
	}
	else if (this->notepadForm->note->GetCurrent() < this->notepadForm
		->note->GetLength() - 1 ) {
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->First();
	}
}

RightKeyAction& RightKeyAction::operator=(const RightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//UpKeyAction
UpKeyAction::UpKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){

}

UpKeyAction::UpKeyAction(const UpKeyAction& source) 
:KeyAction(source){

}

UpKeyAction::~UpKeyAction() {

}

void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() > 0) {
		Long x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, this->notepadForm->current->GetCurrent());
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}
}

UpKeyAction& UpKeyAction::operator = (const UpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//DownKeyAction
DownKeyAction::DownKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){

}

DownKeyAction::DownKeyAction(const DownKeyAction& source) 
:KeyAction(notepadForm){

}

DownKeyAction::~DownKeyAction() {

}

void DownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() < this->notepadForm
		->note->GetLength() - 1) {
		Long x = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, this->notepadForm->current->GetCurrent());
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}
}

DownKeyAction& DownKeyAction::operator=(const DownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;

}

//HomeKeyAction
HomeKeyAction::HomeKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

HomeKeyAction::HomeKeyAction(const HomeKeyAction& source)
	: KeyAction(source) {
}

HomeKeyAction::~HomeKeyAction() {
}

void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->current->First();
}

HomeKeyAction& HomeKeyAction::operator =(const HomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//EndKeyAction
EndKeyAction::EndKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

EndKeyAction::EndKeyAction(const EndKeyAction& source)
	: KeyAction(source) {
}

EndKeyAction::~EndKeyAction() {
}

void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->current->Last();
}

EndKeyAction& EndKeyAction::operator =(const EndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlLeftKeyAction
CtrlLeftKeyAction::CtrlLeftKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlLeftKeyAction::CtrlLeftKeyAction(const CtrlLeftKeyAction& source)
	: KeyAction(source) {
}

CtrlLeftKeyAction::~CtrlLeftKeyAction() {
}

void CtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

CtrlLeftKeyAction& CtrlLeftKeyAction::operator =(const CtrlLeftKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlRightKeyAction
CtrlRightKeyAction::CtrlRightKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlRightKeyAction::CtrlRightKeyAction(const CtrlRightKeyAction& source)
	: KeyAction(source) {
}

CtrlRightKeyAction::~CtrlRightKeyAction() {
}

void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

CtrlRightKeyAction& CtrlRightKeyAction::operator =(const CtrlRightKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlHomeKeyAction
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlHomeKeyAction::CtrlHomeKeyAction(const CtrlHomeKeyAction& source)
	: KeyAction(source) {
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {
}

void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
}

CtrlHomeKeyAction& CtrlHomeKeyAction::operator =(const CtrlHomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlEndKeyAction
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlEndKeyAction::CtrlEndKeyAction(const CtrlEndKeyAction& source)
	: KeyAction(source) {
}

CtrlEndKeyAction::~CtrlEndKeyAction() {
}

void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
}

CtrlEndKeyAction& CtrlEndKeyAction::operator =(const CtrlEndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}


//DeleteKeyAction
DeleteKeyAction::DeleteKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

DeleteKeyAction::DeleteKeyAction(const DeleteKeyAction& source)
	: KeyAction(source) {
}

DeleteKeyAction::~DeleteKeyAction() {
}

void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	
	Glyph *bottomLine = 0;
	Long i;
	Long index;

	if (noteCurrent + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(noteCurrent + 1);
	}

	// 현재 위치가 줄에 맨 끝 그리고 자동개행 상태이고 아래 줄이 더미 줄이면 아래 줄과 현재 줄을 합친다.
	if (lineCurrent >= this->notepadForm->current->GetLength() && dynamic_cast<DummyLine*>(bottomLine)) {
		i = 0;
		while (i <  bottomLine->GetLength()) {
			index = this->notepadForm->current->Add(bottomLine->GetAt(i)->Clone());
			this->notepadForm->editorialMemory.Push(ADD, bottomLine->GetAt(i)->Clone(), noteCurrent, index);
			i++;
		}
		this->notepadForm->current->Move(lineCurrent);

		this->notepadForm->editorialMemory.Push(REMOVE, bottomLine->Clone(), noteCurrent + 1, 0);
		this->notepadForm->note->Remove(noteCurrent + 1);
		
	}
	
	if (lineCurrent < this->notepadForm->current->GetLength()) { // 현재 위치가 줄에 맨 끝이 아닌 경우 현재 위치의 글자를 지운다. 
		this->notepadForm->editorialMemory.Push(REMOVE, this->notepadForm->current->GetAt(lineCurrent)->Clone(),
			noteCurrent, lineCurrent);
		this->notepadForm->current->Remove(lineCurrent);


	} // 현재 위치가 줄에 맨 끝이고 밑에 줄이 있을 경우 밑에 줄과 현재 줄을 합친다.
	else if(lineCurrent >= this->notepadForm->current->GetLength() && noteCurrent + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(noteCurrent + 1);
		i = 0;
		while (i < bottomLine->GetLength()) {
			index = this->notepadForm->current->Add(bottomLine->GetAt(i)->Clone());
			this->notepadForm->editorialMemory.Push(ADD, bottomLine->GetAt(i)->Clone(), noteCurrent, index);
			i++;
		}
		this->notepadForm->current->Move(lineCurrent);

		this->notepadForm->editorialMemory.Push(REMOVE, bottomLine->Clone(), noteCurrent + 1, 0);
		this->notepadForm->note->Remove(noteCurrent + 1);
	} 

}

DeleteKeyAction& DeleteKeyAction::operator =(const DeleteKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//BackSpaceKeyAction
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

BackSpaceKeyAction::BackSpaceKeyAction(const BackSpaceKeyAction& source)
	: KeyAction(source) {
}

BackSpaceKeyAction::~BackSpaceKeyAction() {
}
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	// 글자가 조립중이지 않을 경우에만 처리한다.
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long index;
	Long i;

	//  현재 위치가 줄에서 맨 앞이고 더미 줄이면
	if (lineCurrent == 0 && dynamic_cast<DummyLine*>(this->notepadForm->current)) {
		// 현재 줄을 윗 줄 끝에 옮겨 적는다.
		Glyph *line = this->notepadForm->current;
		this->notepadForm->note->Move(noteCurrent - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(noteCurrent - 1);
		lineCurrent = this->notepadForm->current->GetLength();

		i = 0;
		while (i < line->GetLength()) {
			index = this->notepadForm->current->Add(line->GetAt(i));
			this->notepadForm->editorialMemory.Push(ADD, line->GetAt(i)->Clone(), noteCurrent, index);
			i++;
		}
		this->notepadForm->current->Move(lineCurrent);

		this->notepadForm->editorialMemory.Push(REMOVE, line->Clone(), noteCurrent, 0);
		this->notepadForm->note->Remove(noteCurrent);

		noteCurrent = this->notepadForm->note->GetCurrent();
	}

	if (lineCurrent > 0) {// 현재 위치가 줄에서 맨 앞이 아닐 경우에는 현재 위치 앞 글자를 지운다.
		Glyph *character = this->notepadForm->current->GetAt(lineCurrent-1);
		this->notepadForm->editorialMemory.Push(REMOVE, character->Clone(), noteCurrent, lineCurrent - 1);
		this->notepadForm->current->Remove(lineCurrent - 1);
	}// 현재 위치가 줄에서 맨 앞이고 노트의 첫번째 줄이 아니면 이전 줄과 합친다.
	else if (lineCurrent <= 0 && noteCurrent > 0 ) { 
		Glyph *bottomLine = this->notepadForm->current;
		this->notepadForm->current = this->notepadForm->note->GetAt(noteCurrent - 1);

		index = this->notepadForm->current->GetLength();
		i = 0;
		while (i < bottomLine->GetLength()) {
			Long column = this->notepadForm->current->Add(bottomLine->GetAt(i)->Clone());
			this->notepadForm->editorialMemory.Push(ADD, bottomLine->GetAt(i)->Clone(), noteCurrent - 1, column);

			i++;
		}
		this->notepadForm->editorialMemory.Push(REMOVE, bottomLine->Clone(), noteCurrent, 0);
		this->notepadForm->note->Remove(noteCurrent);

		this->notepadForm->current->Move(index);
	}
}

BackSpaceKeyAction& BackSpaceKeyAction::operator =(const BackSpaceKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

PageUpKeyAction::PageUpKeyAction(NotepadForm *notepadForm) 
:KeyAction(notepadForm){

}

PageUpKeyAction::PageUpKeyAction(const PageUpKeyAction& source)
:KeyAction(source){

}

PageUpKeyAction::~PageUpKeyAction() {

}

void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long position = this->notepadForm->scrollController->PageUp();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);

	// Caret좌표를 토대로 스크롤 했을 때의 노트의 현재 줄과 줄의 현재 글자를 구한다.

	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다. 따라서 현재 x값을 다시 더한다.
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다.따라서 이전 y값을 다시 더한다.
	Long y = this->notepadForm->caretController->GetCaretY() +	previous;

	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row < 0) {
		row = 0;
	}
	Long index = this->notepadForm->note->Move(row); 
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);

}

PageUpKeyAction& PageUpKeyAction::operator = (const PageUpKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}


PageDownKeyAction::PageDownKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

PageDownKeyAction::PageDownKeyAction(const PageDownKeyAction& source)
	: KeyAction(source) {

}

PageDownKeyAction::~PageDownKeyAction(){
}

void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	Long position = this->notepadForm->scrollController->PageDown();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);

	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->notepadForm->caretController->GetCaretY() + previous;
	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row > this->notepadForm->note->GetLength() - 1) {
		row = this->notepadForm->note->GetLength() - 1;
	}
	
	Long index = this->notepadForm->note->Move(row);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);
}

PageDownKeyAction& PageDownKeyAction::operator=(const PageDownKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}

CtrlZ::CtrlZ(NotepadForm *notepadForm)
:KeyAction(notepadForm){
	
}

CtrlZ::CtrlZ(const CtrlZ& source)
:KeyAction(source){

}

CtrlZ::~CtrlZ() {
}

void CtrlZ::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
}

CtrlZ& CtrlZ::operator=(const CtrlZ& source) {
	KeyAction::operator=(source);
	return *this;
}


ShiftLeftKeyAction::ShiftLeftKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

ShiftLeftKeyAction::ShiftLeftKeyAction(const ShiftLeftKeyAction& source)
:KeyAction(source){
}
ShiftLeftKeyAction::~ShiftLeftKeyAction(){
}

void ShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	if (this->notepadForm->current->GetCurrent() > 0) {//1.1.현재 줄에서 current가 줄의 처음이 아니면
		this->notepadForm->current->Previous();//1.1.1.현재 줄에서 이전으로 이동한다.
		//1.1.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	}
	//1.2.현재 줄에서 current가 줄의 처음이고 이전 줄이 있으면
	else if (this->notepadForm->current->GetCurrent() == 0 && this->notepadForm->note->GetCurrent() > 0) {
		//1.2.1.이전 줄로 메모장 윈도우의 메모장에서 이동한다.
		Long index = this->notepadForm->note->Previous();
		//1.2.2.줄에서 끝으로 이동한다.
		//1.2.3.메모장 윈도우의 현재 줄을 갱신한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Last();
		//1.2.4.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	}
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent()};
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);

}
ShiftLeftKeyAction& ShiftLeftKeyAction::operator=(const ShiftLeftKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}

ShiftRightKeyAction::ShiftRightKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

ShiftRightKeyAction::ShiftRightKeyAction(const ShiftRightKeyAction& source)
	: KeyAction(source) {
}
ShiftRightKeyAction::~ShiftRightKeyAction() {
}

void ShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->current->GetCurrent() < this->notepadForm->current->GetLength()) {//1.1.현재 줄에서 current가 줄의 끝이 아니면
		this->notepadForm->current->Next();//1.1.1.현재 줄에서 다음으로 이동한다.
		//1.1.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	}
	//1.2.현재 줄에서 current가 끝이고 다음 줄이 있으면
	else if (this->notepadForm->current->GetCurrent() >= this->notepadForm->current->GetLength()
		&& this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
		//1.2.1.다음 줄로 메모장 윈도우의 메모장에서 이동한다.
		Long index = this->notepadForm->note->Next();
		//1.2.2.줄에서 처음으로 이동한다.
		//1.2.3.메모장 윈도우의 현재 줄을 갱신한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->First();
		//1.2.4.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	}
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
ShiftRightKeyAction& ShiftRightKeyAction::operator=(const ShiftRightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

ShiftUpKeyAction::ShiftUpKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

ShiftUpKeyAction::ShiftUpKeyAction(const ShiftUpKeyAction& source)
	: KeyAction(source) {
}
ShiftUpKeyAction::~ShiftUpKeyAction() {
}

void ShiftUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() > 0) {//3.1.윗 줄이 있으면
		//3.1.1.메모장 윈도우의 메모장에서 윗 줄로 이동한다.
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		
		//3.1.2.윗줄에서 현재 위치에 X좌표와 가장 가까운 열을 찾는다.
		Long x = this->notepadForm->caretController->GetCaretX() + 
			this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		//3.1.3.줄에서 이동한다.
		this->notepadForm->current->Move(column);
		//3.1.4.메모장 윈도우의 현재 줄을 갱신한다
		//3.1.5.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
		Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
		this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
	}

}
ShiftUpKeyAction& ShiftUpKeyAction::operator=(const ShiftUpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

ShiftDownKeyAction::ShiftDownKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

ShiftDownKeyAction::ShiftDownKeyAction(const ShiftDownKeyAction& source)
	: KeyAction(source) {
}
ShiftDownKeyAction::~ShiftDownKeyAction() {
}

void ShiftDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {//4.1.아래 줄이 있으면
		//4.1.1.메모장 윈도우의 메모장에서 아래 줄로 이동한다.
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);

		//4.1.2.줄에서 현재 위치에 X좌표와 가장 가까운 열을 찾는다.
		Long x = this->notepadForm->caretController->GetCaretX() +
			this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		//4.1.3.줄에서 이동한다.
		this->notepadForm->current->Move(column);
		//4.1.4.메모장 윈도우의 현재 줄을 갱신한다
		//4.1.5.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
		Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
		this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
	}
}
ShiftDownKeyAction& ShiftDownKeyAction::operator=(const ShiftDownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

CtrlShiftHomeKeyAction::CtrlShiftHomeKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlShiftHomeKeyAction::CtrlShiftHomeKeyAction(const CtrlShiftHomeKeyAction& source)
	: KeyAction(source) {
}
CtrlShiftHomeKeyAction::~CtrlShiftHomeKeyAction() {
}

void CtrlShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->First();//5.1.메모장 윈도우의 메모장에서 첫째줄로 이동한다.
	//5.2.줄에서 처음으로 이동한다,
	//5.3.메모장 윈도우의 현재 줄을 갱신한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
	//5.3.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
CtrlShiftHomeKeyAction& CtrlShiftHomeKeyAction::operator=(const CtrlShiftHomeKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

CtrlShiftEndKeyAction::CtrlShiftEndKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlShiftEndKeyAction::CtrlShiftEndKeyAction(const CtrlShiftEndKeyAction& source)
	: KeyAction(source) {
}
CtrlShiftEndKeyAction::~CtrlShiftEndKeyAction() {
}

void CtrlShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->Last();//6.1.메모장 윈도우의 메모장에서 마지막줄로 이동한다.
	//6.2.줄에서 마지막으로 이동한다,
	//6.3.메모장 윈도우의 현재 줄을 갱신한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
	//6.3.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
CtrlShiftEndKeyAction& CtrlShiftEndKeyAction::operator=(const CtrlShiftEndKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

ShiftHomeKeyAction::ShiftHomeKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

ShiftHomeKeyAction::ShiftHomeKeyAction(const ShiftHomeKeyAction& source)
	: KeyAction(source) {
}
ShiftHomeKeyAction::~ShiftHomeKeyAction() {
}

void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//7.1.메모장 윈도우의 현재 줄에서 처음으로 이동한다.
	this->notepadForm->current->First();
	//7.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
ShiftHomeKeyAction& ShiftHomeKeyAction::operator=(const ShiftHomeKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

ShiftEndKeyAction::ShiftEndKeyAction(const ShiftEndKeyAction& source)
	: KeyAction(source) {
}
ShiftEndKeyAction::~ShiftEndKeyAction() {
}

void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//8.1.메모장 윈도우의 현재 줄에서 끝으로 이동한다.
	this->notepadForm->current->Last();
	//8.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
ShiftEndKeyAction& ShiftEndKeyAction::operator=(const ShiftEndKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

CtrlShiftLeftKeyAction::CtrlShiftLeftKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlShiftLeftKeyAction::CtrlShiftLeftKeyAction(const CtrlShiftLeftKeyAction& source)
	: KeyAction(source) {
}
CtrlShiftLeftKeyAction::~CtrlShiftLeftKeyAction() {
}

void CtrlShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//10.1.메모장 윈도우의 메모장에서 이전 단어로 이동한다.
	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	//10.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
CtrlShiftLeftKeyAction& CtrlShiftLeftKeyAction::operator=(const CtrlShiftLeftKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

CtrlShiftRightKeyAction::CtrlShiftRightKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlShiftRightKeyAction::CtrlShiftRightKeyAction(const CtrlShiftRightKeyAction& source)
	: KeyAction(source) {
}
CtrlShiftRightKeyAction::~CtrlShiftRightKeyAction() {
}

void CtrlShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//9.1.메모장 윈도우의 메모장에서 다음 단어로 이동한다.
	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	//9.2.현재 위치를 끝 위치로 메모장 윈도우의 메모장에서 선택한다.
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(this->notepadForm->selectedAreaHandler->GetStartPoint(), endPoint);
}
CtrlShiftRightKeyAction& CtrlShiftRightKeyAction::operator=(const CtrlShiftRightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

CtrlAKeyAction::CtrlAKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlAKeyAction::CtrlAKeyAction(const CtrlAKeyAction& source)
	: KeyAction(source) {
}
CtrlAKeyAction::~CtrlAKeyAction() {
}

void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->Last();//11.1.메모장 윈도우의 메모장에서 마지막 줄로 이동한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();//11.2.줄에서 마지막으로 이동한다.
	//11.3.첫번째 줄에 처음 위치를 시작 위치로 그리고 현재 위치를 끝 위치로 하여 메모장 윈도우의 메모장에서 선택한다.
	Point startPoint = { 0, 0 };
	Point endPoint = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	this->notepadForm->selectedAreaHandler->Select(startPoint, endPoint);

}
CtrlAKeyAction& CtrlAKeyAction::operator=(const CtrlAKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}


DelAnBSKeyActionInSelState::DelAnBSKeyActionInSelState(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

DelAnBSKeyActionInSelState::DelAnBSKeyActionInSelState(const DelAnBSKeyActionInSelState& source)
:KeyAction(source){
}

DelAnBSKeyActionInSelState::~DelAnBSKeyActionInSelState(){}

void DelAnBSKeyActionInSelState::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	this->notepadForm->selectedAreaHandler->Remove();

	// 현재 줄이 더미 줄이고 줄의 길이가 0보다 작거나 같으면 
	if (dynamic_cast<DummyLine*>(this->notepadForm->current) && this->notepadForm->current->GetLength() <= 0) {
		//  현재 줄을 지운다.
		Long index = this->notepadForm->note->GetCurrent();
		this->notepadForm->note->Remove(index);
		index = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Move(this->notepadForm->current->GetLength());
	}

	
}

DelAnBSKeyActionInSelState& DelAnBSKeyActionInSelState::operator=(const DelAnBSKeyActionInSelState& source){
	KeyAction::operator=(source);
	return *this;
}

CtrlCKeyAction::CtrlCKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

CtrlCKeyAction::CtrlCKeyAction(const CtrlCKeyAction& source)
: KeyAction(source.notepadForm) {
}

CtrlCKeyAction::~CtrlCKeyAction(){}

void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {
		this->notepadForm->selectedAreaHandler->Copy();
	}
}

CtrlCKeyAction& CtrlCKeyAction::operator=(const CtrlCKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}

CtrlXKeyAction::CtrlXKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

CtrlXKeyAction::CtrlXKeyAction(const CtrlXKeyAction& source)
:KeyAction(source){
}

CtrlXKeyAction::~CtrlXKeyAction(){}

void CtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	
	if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {//i. 메모장 윈도우가 선택 상태이면
		this->notepadForm->selectedAreaHandler->Cut();
	}
}

CtrlXKeyAction& CtrlXKeyAction::operator=(const CtrlXKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}

CtrlVKeyAction::CtrlVKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

CtrlVKeyAction::CtrlVKeyAction(const CtrlVKeyAction& source)
	: KeyAction(source) {
}

CtrlVKeyAction::~CtrlVKeyAction() {}

void CtrlVKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

	if (this->notepadForm->clipboard->IsTextDataAvailable() == TRUE) {
		// 선택 상태이면 선택된 글을 지운다.
		if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {//i. 메모장 윈도우가 선택 상태이면
			this->notepadForm->selectedAreaHandler->Remove();
		}
		string text = this->notepadForm->clipboard->GetTextData();
		GlyphFactory glyphFactory;
		string token;
		Long index;
		Long addLineCount = 1;
		Scanner scanner(text);
		
		while (!scanner.IsEnd()) {
			token = scanner.GetToken();
			Glyph * glyph = glyphFactory.Make(token.c_str());

			if (dynamic_cast<SingleByteCharacter*>(glyph) || dynamic_cast<DoubleByteCharacter*>(glyph)) {
				index = this->notepadForm->current->GetCurrent();
				Long length = this->notepadForm->current->GetLength();
				if (index < length) {
					this->notepadForm->current->Add(index, glyph);
				}
				else if (index == length) {
					this->notepadForm->current->Add(glyph);
				}
				
			}
			else if (dynamic_cast<Line*>(glyph)) {
				index = this->notepadForm->current->GetCurrent();
				delete glyph;
				glyph = this->notepadForm->current->Divide(index);
				glyph->Move(0);
				
				index = this->notepadForm->note->GetCurrent();
				this->notepadForm->note->Add(index + 1, glyph);
				this->notepadForm->current = this->notepadForm->note->GetAt(index + 1);
				addLineCount++;
			}
			scanner.Next();
		}


		// 자동개행 상태이면
		if (this->notepadForm->autoLineBreaker != 0) {
			// 1. 메모장 윈도우의 자동개행자에서 현재 위치로 자동개행 전 위치를 구하다.
			Point position = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
			Point previousPosition = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(position);
			Long previousLength;
		
			index = this->notepadForm->note->GetCurrent() - addLineCount + 1;
			Long i = 1;

			while (i <= addLineCount) { // 2. 붙여넣기한 줄부터 붙여넣기한 줄의 개수만큼 반복하다.
				//  2.1. 메모장 윈도우의 자동개행자에서 줄을 자동개행한다.
				previousLength = this->notepadForm->note->GetLength();
				this->notepadForm->autoLineBreaker->Update(index);
				//  2.2. 메모장 윈도우의 메모장의 늘어난 길이에 1을 더한 위치의 줄로 이동한다.
				index = index + (this->notepadForm->note->GetLength() - previousLength + 1);
				i++;
			}

			// 3. 메모장 윈도우의 자동개행자에서 자동 개행 후 위치를 구하다.
			position = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(previousPosition);
			// 4. 메모장 윈도우의 메모장에서 이동하다.
			this->notepadForm->note->Move(position.y);
			this->notepadForm->current = this->notepadForm->note->GetAt(position.y);
			this->notepadForm->current->Move(position.x);
		}


		// 붙여넣기한 부분
	}
	
}

CtrlVKeyAction& CtrlVKeyAction::operator=(const CtrlVKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}


CtrlFKeyAction::CtrlFKeyAction(NotepadForm *notepadForm )
:KeyAction(notepadForm){
}

CtrlFKeyAction::CtrlFKeyAction(const CtrlFKeyAction& source)
:KeyAction(source){
}

CtrlFKeyAction::~CtrlFKeyAction(){
}

void CtrlFKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	Long noteLength = this->notepadForm->note->GetLength();
	Long lineLength = this->notepadForm->current->GetLength();
	if (noteLength > 1 || lineLength > 0) {
		if (this->notepadForm->findingForm->GetSafeHwnd() == NULL) {
			this->notepadForm->findingForm->Create(FindingForm::IDD);
		}
		
		if (this->notepadForm->changingForm->GetSafeHwnd() != NULL) {
			CString text;
			this->notepadForm->changingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
			this->notepadForm->findingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->SetWindowTextA((LPCSTR)text.GetBuffer());
			BOOL isCheck = this->notepadForm->changingForm->IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
			this->notepadForm->findingForm->CheckDlgButton(IDC_CHECK_CASESENSITIVE, isCheck);
			this->notepadForm->changingForm->EndDialog(0);
		}

		this->notepadForm->findingForm->ShowWindow(SW_SHOW);
	}
	
}

CtrlFKeyAction& CtrlFKeyAction::operator=(const CtrlFKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}


CtrlHKeyAction::CtrlHKeyAction(NotepadForm *notepadForm)
:KeyAction(notepadForm){
}

CtrlHKeyAction::CtrlHKeyAction(const CtrlHKeyAction& source)
:KeyAction(source){
}

CtrlHKeyAction::~CtrlHKeyAction(){
}

void CtrlHKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	Long noteLength = this->notepadForm->note->GetLength();
	Long lineLength = this->notepadForm->current->GetLength();
	if (noteLength > 1 || lineLength > 0) {
		if (this->notepadForm->changingForm->GetSafeHwnd() == NULL) {
			this->notepadForm->changingForm->Create(ChangingForm::IDD);
		}

		if (this->notepadForm->findingForm->GetSafeHwnd() != NULL && this->notepadForm->findingForm->IsWindowVisible() == TRUE) {
			CString text;
			this->notepadForm->findingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->GetWindowTextA(text);
			this->notepadForm->changingForm->GetDlgItem(IDC_EDIT_CONTENT_TO_FIND)->SetWindowTextA((LPCSTR)text.GetBuffer());
			BOOL isCheck = this->notepadForm->findingForm->IsDlgButtonChecked(IDC_CHECK_CASESENSITIVE);
			this->notepadForm->changingForm->CheckDlgButton(IDC_CHECK_CASESENSITIVE, isCheck);

			this->notepadForm->findingForm->EndDialog(0);
		}

		this->notepadForm->changingForm->ShowWindow(SW_SHOW);

	}

}

CtrlHKeyAction& CtrlHKeyAction::operator=(const CtrlHKeyAction& source){
	KeyAction::operator=(source);
	return *this;
}

