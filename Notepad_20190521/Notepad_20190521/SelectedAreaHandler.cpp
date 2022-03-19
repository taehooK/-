#include "SelectedAreaHandler.h"
#include "NotepadForm.h"
#include "AutoLineBreaker.h"
#include "Clipboard.h"
#include "EditWorks.h"


SelectedAreaHandler::SelectedAreaHandler(NotepadForm *notepadForm){
	this->notepadForm = notepadForm;
	this->startPoint = { 0,0 };
	this->endPoint = { 0,0 };
}

SelectedAreaHandler::SelectedAreaHandler(const SelectedAreaHandler& source){
	this->notepadForm = notepadForm;
	this->startPoint = { 0,0 };
	this->endPoint = { 0,0 };
}

SelectedAreaHandler::~SelectedAreaHandler(){
}


/*********
함수 명칭 : Select
기능 : 시작지점과 끝지점을 입력받아서 멤버변수에 저장한다. 
       자동 개행 상태이면 자동 개행 전의 좌표를 구해서 저장한다.
입력 : 시작지점, 끝지점
출력 :
********/

void SelectedAreaHandler::Select(Point startPoint, Point endPoint){
	// 자동개행 상태가 아니면
	if (this->notepadForm->autoLineBreaker == 0) {
		this->startPoint = startPoint;
		this->endPoint = endPoint;
	}
	else {// 자동개행 상태이면 
		// 자동개행 전의 좌표를 구한다.
		this->startPoint = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(startPoint);
		this->endPoint = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(endPoint);
	}
}

/*************
함수 명칭 : Remove
기능: 메모장의 선택 되어있는 영역을 지운다.
입력:
출력:
***************/
void SelectedAreaHandler::Remove() {
	// 자동개행 상태이면 자동개행 후의 좌표로 바꾼다.
	Point start =this->startPoint;
	Point end = this->endPoint;
	Point temp;
	Long i;
	Long count;
	
	if (this->notepadForm->autoLineBreaker != 0) {
		start = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->startPoint);
		end = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->endPoint);
	}

	if (start.y > end.y || (start.y == end.y && start.x > end.x)) { // start지점이 end지점보다 뒤에 있으면
		temp = start;
		start = end;
		end = temp;
	}

	if (start.y == end.y) {//5) 한 줄 안에서 선택한 경우 
		i = 1;
		count = end.x - start.x;
		while (i <= count) {//a) 메모장 윈도우의 현재 줄의 처음 위치 끝 위치전까지 글자를 지운다.
			this->notepadForm->editorialMemory.Push(REMOVE, this->notepadForm->current->GetAt(start.x)->Clone(),
				this->notepadForm->note->GetCurrent(), start.x);
			this->notepadForm->current->Remove(start.x);
			i++;
		}
		
	}
	else if (start.y < end.y) {//6) 두 줄 이상에서 선택한 경우
		//a) 처음 위치의 열부터 줄 끝까지 지운다.
		Glyph *line = this->notepadForm->note->GetAt(start.y);
		i = 1;
		count = line->GetLength() - start.x;
		while (i <= count) {
			this->notepadForm->editorialMemory.Push(REMOVE, line->GetAt(start.x)->Clone(), start.y, start.x);
			
			line->Remove(start.x);
			i++;
		}

		//b) 다음 줄부터 끝 위치 줄 전까지 줄을 지운다. 
		i = 1;
		count = end.y - start.y - 1;
		while (i <= count) {
			Glyph * cloneLine = this->notepadForm->note->GetAt(start.y + 1)->Clone();
			this->notepadForm->editorialMemory.Push(REMOVE, cloneLine, start.y + 1, 0);
			this->notepadForm->note->Remove(start.y + 1);
			i++;
		}
	
		//c) 선택된 영역의 마지막 줄에서 선택된 영역만큼 글자를 지운다.
		Glyph *nextLine = this->notepadForm->note->GetAt(start.y + 1);
		i = 1;
		while (i <= end.x) {
			this->notepadForm->editorialMemory.Push(REMOVE, nextLine->GetAt(0)->Clone(), start.y + 1, 0);
			nextLine->Remove(0);
			i++;
		}
		//d) 선택된 영역의 처음 위치의 줄과 마지막 줄을 합친다.
		i = 0;
		while (i < nextLine->GetLength()) {
			Glyph *character = nextLine->GetAt(i);
			Long column = line->Add(character->Clone());
			this->notepadForm->editorialMemory.Push(ADD, character->Clone(), start.y, column);
			i++;
		}


		this->notepadForm->editorialMemory.Push(REMOVE, nextLine->Clone(), start.y + 1, 0);
		this->notepadForm->note->Remove(start.y + 1);

		//e) 커서 위치를 시작 위치로 옮긴다.
		this->notepadForm->note->Move(start.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(start.y);
		this->notepadForm->current->Move(start.x);
	}

	this->startPoint = { 0,0 };
	this->endPoint = { 0,0 };
}

/*************
함수 명칭 : Cut
기능: 메모장의 선택 되어있는 영역을 클립보드에 복사하고 선택되어있는 영역을 지운다.
입력:
출력:
***************/
void SelectedAreaHandler::Cut(){
	Point start = this->startPoint;
	Point end = this->endPoint;

	if (this->notepadForm->autoLineBreaker != 0) {
		start = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(start);
		end = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(end);
	}
	this->notepadForm->clipboard->Copy(this->notepadForm->note->GetContent(start, end));
	this->Remove();
}

/*************
함수 명칭 : Copy
기능: 메모장의 선택 되어있는 영역을 클립보드에 복사한다.
입력:
출력:
***************/
void SelectedAreaHandler::Copy(){
	Point start = this->startPoint;
	Point end = this->endPoint;

	if (this->notepadForm->autoLineBreaker != 0) {
		start = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(start);
		end = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(end);
	}
	
	this->notepadForm->clipboard->Copy(this->notepadForm->note->GetContent(start, end));
}

SelectedAreaHandler& SelectedAreaHandler::operator=(const SelectedAreaHandler& other){
	this->notepadForm = other.notepadForm;
	this->startPoint = other.startPoint;
	this->endPoint = other.endPoint;
	return *this;
}

/***************************************
함수 명칭 : GetStartPoint
기능: 선택된 영역의 시작 위치를 반환한다. 자동 개행 상태인 경우 자동 개행 후의 위치로 변환 후 반환한다.
입력:
출력: 시작 위치
*****************************************/
Point SelectedAreaHandler::GetStartPoint(){
	Point ret = this->startPoint;
	if (this->notepadForm->autoLineBreaker != 0) {
		ret = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->startPoint);
	}
	return ret;
}

/***************************************
함수 명칭 : GetEndPoint
기능: 선택된 영역의 끝 위치를 반환한다. 자동 개행 상태인 경우 자동 개행 후의 위치로 변환 후 반환한다.
입력:
출력: 끝 위치
*****************************************/
Point SelectedAreaHandler::GetEndPoint() {
	Point ret = this->endPoint;
	if (this->notepadForm->autoLineBreaker != 0) {
		ret = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->endPoint);
	}
	return ret;
}
