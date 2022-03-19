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

	// ���� ��ġ�� �ٿ� �� �� �׸��� �ڵ����� �����̰� �Ʒ� ���� ���� ���̸� �Ʒ� �ٰ� ���� ���� ��ģ��.
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
	
	if (lineCurrent < this->notepadForm->current->GetLength()) { // ���� ��ġ�� �ٿ� �� ���� �ƴ� ��� ���� ��ġ�� ���ڸ� �����. 
		this->notepadForm->editorialMemory.Push(REMOVE, this->notepadForm->current->GetAt(lineCurrent)->Clone(),
			noteCurrent, lineCurrent);
		this->notepadForm->current->Remove(lineCurrent);


	} // ���� ��ġ�� �ٿ� �� ���̰� �ؿ� ���� ���� ��� �ؿ� �ٰ� ���� ���� ��ģ��.
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
	// ���ڰ� ���������� ���� ��쿡�� ó���Ѵ�.
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long index;
	Long i;

	//  ���� ��ġ�� �ٿ��� �� ���̰� ���� ���̸�
	if (lineCurrent == 0 && dynamic_cast<DummyLine*>(this->notepadForm->current)) {
		// ���� ���� �� �� ���� �Ű� ���´�.
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

	if (lineCurrent > 0) {// ���� ��ġ�� �ٿ��� �� ���� �ƴ� ��쿡�� ���� ��ġ �� ���ڸ� �����.
		Glyph *character = this->notepadForm->current->GetAt(lineCurrent-1);
		this->notepadForm->editorialMemory.Push(REMOVE, character->Clone(), noteCurrent, lineCurrent - 1);
		this->notepadForm->current->Remove(lineCurrent - 1);
	}// ���� ��ġ�� �ٿ��� �� ���̰� ��Ʈ�� ù��° ���� �ƴϸ� ���� �ٰ� ��ģ��.
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

	// Caret��ǥ�� ���� ��ũ�� ���� ���� ��Ʈ�� ���� �ٰ� ���� ���� ���ڸ� ���Ѵ�.

	// Caret�� x���� ��Ʈ�� ��ġ�� ��ũ�� ��ġ�� �� ���¿� ���� �� �ִ�. ���� ���� x���� �ٽ� ���Ѵ�.
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	// Caret�� x���� ��Ʈ�� ��ġ�� ��ũ�� ��ġ�� �� ���¿� ���� �� �ִ�.���� ���� y���� �ٽ� ���Ѵ�.
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
	if (this->notepadForm->current->GetCurrent() > 0) {//1.1.���� �ٿ��� current�� ���� ó���� �ƴϸ�
		this->notepadForm->current->Previous();//1.1.1.���� �ٿ��� �������� �̵��Ѵ�.
		//1.1.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
	}
	//1.2.���� �ٿ��� current�� ���� ó���̰� ���� ���� ������
	else if (this->notepadForm->current->GetCurrent() == 0 && this->notepadForm->note->GetCurrent() > 0) {
		//1.2.1.���� �ٷ� �޸��� �������� �޸��忡�� �̵��Ѵ�.
		Long index = this->notepadForm->note->Previous();
		//1.2.2.�ٿ��� ������ �̵��Ѵ�.
		//1.2.3.�޸��� �������� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Last();
		//1.2.4.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	if (this->notepadForm->current->GetCurrent() < this->notepadForm->current->GetLength()) {//1.1.���� �ٿ��� current�� ���� ���� �ƴϸ�
		this->notepadForm->current->Next();//1.1.1.���� �ٿ��� �������� �̵��Ѵ�.
		//1.1.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
	}
	//1.2.���� �ٿ��� current�� ���̰� ���� ���� ������
	else if (this->notepadForm->current->GetCurrent() >= this->notepadForm->current->GetLength()
		&& this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
		//1.2.1.���� �ٷ� �޸��� �������� �޸��忡�� �̵��Ѵ�.
		Long index = this->notepadForm->note->Next();
		//1.2.2.�ٿ��� ó������ �̵��Ѵ�.
		//1.2.3.�޸��� �������� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->First();
		//1.2.4.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	if (this->notepadForm->note->GetCurrent() > 0) {//3.1.�� ���� ������
		//3.1.1.�޸��� �������� �޸��忡�� �� �ٷ� �̵��Ѵ�.
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		
		//3.1.2.���ٿ��� ���� ��ġ�� X��ǥ�� ���� ����� ���� ã�´�.
		Long x = this->notepadForm->caretController->GetCaretX() + 
			this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		//3.1.3.�ٿ��� �̵��Ѵ�.
		this->notepadForm->current->Move(column);
		//3.1.4.�޸��� �������� ���� ���� �����Ѵ�
		//3.1.5.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {//4.1.�Ʒ� ���� ������
		//4.1.1.�޸��� �������� �޸��忡�� �Ʒ� �ٷ� �̵��Ѵ�.
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);

		//4.1.2.�ٿ��� ���� ��ġ�� X��ǥ�� ���� ����� ���� ã�´�.
		Long x = this->notepadForm->caretController->GetCaretX() +
			this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		//4.1.3.�ٿ��� �̵��Ѵ�.
		this->notepadForm->current->Move(column);
		//4.1.4.�޸��� �������� ���� ���� �����Ѵ�
		//4.1.5.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	Long index = this->notepadForm->note->First();//5.1.�޸��� �������� �޸��忡�� ù°�ٷ� �̵��Ѵ�.
	//5.2.�ٿ��� ó������ �̵��Ѵ�,
	//5.3.�޸��� �������� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
	//5.3.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	Long index = this->notepadForm->note->Last();//6.1.�޸��� �������� �޸��忡�� �������ٷ� �̵��Ѵ�.
	//6.2.�ٿ��� ���������� �̵��Ѵ�,
	//6.3.�޸��� �������� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
	//6.3.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	//7.1.�޸��� �������� ���� �ٿ��� ó������ �̵��Ѵ�.
	this->notepadForm->current->First();
	//7.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	//8.1.�޸��� �������� ���� �ٿ��� ������ �̵��Ѵ�.
	this->notepadForm->current->Last();
	//8.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	//10.1.�޸��� �������� �޸��忡�� ���� �ܾ�� �̵��Ѵ�.
	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	//10.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	//9.1.�޸��� �������� �޸��忡�� ���� �ܾ�� �̵��Ѵ�.
	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	//9.2.���� ��ġ�� �� ��ġ�� �޸��� �������� �޸��忡�� �����Ѵ�.
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
	Long index = this->notepadForm->note->Last();//11.1.�޸��� �������� �޸��忡�� ������ �ٷ� �̵��Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();//11.2.�ٿ��� ���������� �̵��Ѵ�.
	//11.3.ù��° �ٿ� ó�� ��ġ�� ���� ��ġ�� �׸��� ���� ��ġ�� �� ��ġ�� �Ͽ� �޸��� �������� �޸��忡�� �����Ѵ�.
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

	// ���� ���� ���� ���̰� ���� ���̰� 0���� �۰ų� ������ 
	if (dynamic_cast<DummyLine*>(this->notepadForm->current) && this->notepadForm->current->GetLength() <= 0) {
		//  ���� ���� �����.
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
	
	if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {//i. �޸��� �����찡 ���� �����̸�
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
		// ���� �����̸� ���õ� ���� �����.
		if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {//i. �޸��� �����찡 ���� �����̸�
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


		// �ڵ����� �����̸�
		if (this->notepadForm->autoLineBreaker != 0) {
			// 1. �޸��� �������� �ڵ������ڿ��� ���� ��ġ�� �ڵ����� �� ��ġ�� ���ϴ�.
			Point position = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
			Point previousPosition = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(position);
			Long previousLength;
		
			index = this->notepadForm->note->GetCurrent() - addLineCount + 1;
			Long i = 1;

			while (i <= addLineCount) { // 2. �ٿ��ֱ��� �ٺ��� �ٿ��ֱ��� ���� ������ŭ �ݺ��ϴ�.
				//  2.1. �޸��� �������� �ڵ������ڿ��� ���� �ڵ������Ѵ�.
				previousLength = this->notepadForm->note->GetLength();
				this->notepadForm->autoLineBreaker->Update(index);
				//  2.2. �޸��� �������� �޸����� �þ ���̿� 1�� ���� ��ġ�� �ٷ� �̵��Ѵ�.
				index = index + (this->notepadForm->note->GetLength() - previousLength + 1);
				i++;
			}

			// 3. �޸��� �������� �ڵ������ڿ��� �ڵ� ���� �� ��ġ�� ���ϴ�.
			position = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(previousPosition);
			// 4. �޸��� �������� �޸��忡�� �̵��ϴ�.
			this->notepadForm->note->Move(position.y);
			this->notepadForm->current = this->notepadForm->note->GetAt(position.y);
			this->notepadForm->current->Move(position.x);
		}


		// �ٿ��ֱ��� �κ�
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

