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
�Լ� ��Ī : Select
��� : ���������� �������� �Է¹޾Ƽ� ��������� �����Ѵ�. 
       �ڵ� ���� �����̸� �ڵ� ���� ���� ��ǥ�� ���ؼ� �����Ѵ�.
�Է� : ��������, ������
��� :
********/

void SelectedAreaHandler::Select(Point startPoint, Point endPoint){
	// �ڵ����� ���°� �ƴϸ�
	if (this->notepadForm->autoLineBreaker == 0) {
		this->startPoint = startPoint;
		this->endPoint = endPoint;
	}
	else {// �ڵ����� �����̸� 
		// �ڵ����� ���� ��ǥ�� ���Ѵ�.
		this->startPoint = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(startPoint);
		this->endPoint = this->notepadForm->autoLineBreaker->CovPosToNonAutoLineBreak(endPoint);
	}
}

/*************
�Լ� ��Ī : Remove
���: �޸����� ���� �Ǿ��ִ� ������ �����.
�Է�:
���:
***************/
void SelectedAreaHandler::Remove() {
	// �ڵ����� �����̸� �ڵ����� ���� ��ǥ�� �ٲ۴�.
	Point start =this->startPoint;
	Point end = this->endPoint;
	Point temp;
	Long i;
	Long count;
	
	if (this->notepadForm->autoLineBreaker != 0) {
		start = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->startPoint);
		end = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->endPoint);
	}

	if (start.y > end.y || (start.y == end.y && start.x > end.x)) { // start������ end�������� �ڿ� ������
		temp = start;
		start = end;
		end = temp;
	}

	if (start.y == end.y) {//5) �� �� �ȿ��� ������ ��� 
		i = 1;
		count = end.x - start.x;
		while (i <= count) {//a) �޸��� �������� ���� ���� ó�� ��ġ �� ��ġ������ ���ڸ� �����.
			this->notepadForm->editorialMemory.Push(REMOVE, this->notepadForm->current->GetAt(start.x)->Clone(),
				this->notepadForm->note->GetCurrent(), start.x);
			this->notepadForm->current->Remove(start.x);
			i++;
		}
		
	}
	else if (start.y < end.y) {//6) �� �� �̻󿡼� ������ ���
		//a) ó�� ��ġ�� ������ �� ������ �����.
		Glyph *line = this->notepadForm->note->GetAt(start.y);
		i = 1;
		count = line->GetLength() - start.x;
		while (i <= count) {
			this->notepadForm->editorialMemory.Push(REMOVE, line->GetAt(start.x)->Clone(), start.y, start.x);
			
			line->Remove(start.x);
			i++;
		}

		//b) ���� �ٺ��� �� ��ġ �� ������ ���� �����. 
		i = 1;
		count = end.y - start.y - 1;
		while (i <= count) {
			Glyph * cloneLine = this->notepadForm->note->GetAt(start.y + 1)->Clone();
			this->notepadForm->editorialMemory.Push(REMOVE, cloneLine, start.y + 1, 0);
			this->notepadForm->note->Remove(start.y + 1);
			i++;
		}
	
		//c) ���õ� ������ ������ �ٿ��� ���õ� ������ŭ ���ڸ� �����.
		Glyph *nextLine = this->notepadForm->note->GetAt(start.y + 1);
		i = 1;
		while (i <= end.x) {
			this->notepadForm->editorialMemory.Push(REMOVE, nextLine->GetAt(0)->Clone(), start.y + 1, 0);
			nextLine->Remove(0);
			i++;
		}
		//d) ���õ� ������ ó�� ��ġ�� �ٰ� ������ ���� ��ģ��.
		i = 0;
		while (i < nextLine->GetLength()) {
			Glyph *character = nextLine->GetAt(i);
			Long column = line->Add(character->Clone());
			this->notepadForm->editorialMemory.Push(ADD, character->Clone(), start.y, column);
			i++;
		}


		this->notepadForm->editorialMemory.Push(REMOVE, nextLine->Clone(), start.y + 1, 0);
		this->notepadForm->note->Remove(start.y + 1);

		//e) Ŀ�� ��ġ�� ���� ��ġ�� �ű��.
		this->notepadForm->note->Move(start.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(start.y);
		this->notepadForm->current->Move(start.x);
	}

	this->startPoint = { 0,0 };
	this->endPoint = { 0,0 };
}

/*************
�Լ� ��Ī : Cut
���: �޸����� ���� �Ǿ��ִ� ������ Ŭ�����忡 �����ϰ� ���õǾ��ִ� ������ �����.
�Է�:
���:
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
�Լ� ��Ī : Copy
���: �޸����� ���� �Ǿ��ִ� ������ Ŭ�����忡 �����Ѵ�.
�Է�:
���:
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
�Լ� ��Ī : GetStartPoint
���: ���õ� ������ ���� ��ġ�� ��ȯ�Ѵ�. �ڵ� ���� ������ ��� �ڵ� ���� ���� ��ġ�� ��ȯ �� ��ȯ�Ѵ�.
�Է�:
���: ���� ��ġ
*****************************************/
Point SelectedAreaHandler::GetStartPoint(){
	Point ret = this->startPoint;
	if (this->notepadForm->autoLineBreaker != 0) {
		ret = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->startPoint);
	}
	return ret;
}

/***************************************
�Լ� ��Ī : GetEndPoint
���: ���õ� ������ �� ��ġ�� ��ȯ�Ѵ�. �ڵ� ���� ������ ��� �ڵ� ���� ���� ��ġ�� ��ȯ �� ��ȯ�Ѵ�.
�Է�:
���: �� ��ġ
*****************************************/
Point SelectedAreaHandler::GetEndPoint() {
	Point ret = this->endPoint;
	if (this->notepadForm->autoLineBreaker != 0) {
		ret = this->notepadForm->autoLineBreaker->CovPosToAutoLineBreak(this->endPoint);
	}
	return ret;
}
