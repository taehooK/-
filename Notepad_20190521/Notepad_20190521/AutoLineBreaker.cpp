#include "AutoLineBreaker.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "GlyphFactory.h"
#include "CaretController.h"
#include "DummyLine.h"
#include "ScrollController.h"

AutoLineBreaker::AutoLineBreaker(NotepadForm *notepadForm){
	this->notepadForm = notepadForm;
	this->notepadForm->AttachObserver(0, this);
}

AutoLineBreaker::AutoLineBreaker(const AutoLineBreaker& source){
	this->notepadForm = source.notepadForm;
}

AutoLineBreaker::~AutoLineBreaker(){
	this->notepadForm->DettachObserver(this);
}

/**********************************
�Լ���:Update
�Է�: 
���:
���: NotepadForm�� current���� �ڵ������Ѵ�.
***********************************/

void AutoLineBreaker::Update() {

	Glyph *line;
	Glyph *bottomLine = 0;

	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left; // 1. Ŭ���̾�Ʈ �ʺ� ���.
	Long lineWidth;
	Long characterWidth = 0;
	Long i;
	Long index;
	Long count;
	Long lineNumber = this->notepadForm->note->GetCurrent();

	Point previousPosition = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	previousPosition = this->CovPosToNonAutoLineBreak(previousPosition);

	// 2. ���� �ʺ� ���.
	line = this->notepadForm->note->GetAt(lineNumber);
	lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

	// 3. �Ʒ� ���� ���� ���̸� �Ʒ� ���� ù ������ �ʺ����.
	if (lineNumber + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(lineNumber + 1);
		
	}

	if (dynamic_cast<DummyLine*>(bottomLine)) {
		characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);
	}
	// 4. �Ʒ� ���� ���� ���̰� ���� �ʺ�� ������ �ʺ� ���Ѱ� Ŭ���̾�Ʈ �ʺ񺸴� ������
	if (dynamic_cast<DummyLine*>(bottomLine) && lineWidth + characterWidth < clientWidth) {
		// 4.1. �Ʒ� ���� �ٳ��� �Ű� ���´�.
		line->Combine(bottomLine);
		this->notepadForm->note->Remove(lineNumber + 1);
		// 4.2. ���� �ʺ� ���.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
	}

	if (lineWidth >= clientWidth) {// 5. Ŭ���̾�Ʈ �ʺ񺸴� ũ��
		// 5.1. �ٺ��� ������ �ٱ��� �׸��� ���� �ʺ� Ŭ���̾�Ʈ �ʺ񺸴� ū ���� �ݺ��Ѵ�.
		i = lineNumber;
		while (i <this->notepadForm->note->GetLength() && lineWidth >= clientWidth) {
			//  5.1.1. Ŭ���̾�Ʈ �ʺ� ��� ������ ã�´�.
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth);
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}

			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  5.1.2. �Ʒ� ���� ���� ���� �ƴϸ�
				//   5.1.2.1. ���� ���� �����.
				bottomLine = new DummyLine;
				//   5.1.2.2. �Ʒ��� �����.
				this->notepadForm->note->Add(i + 1, bottomLine);
			}

			count = line->GetLength() - index;
			Long j = 0;
			while (j < count) {//  5.1.3. Ŭ���̾�Ʈ �ʺ� ��� �κ��� �Ʒ� ���� ù �κп� �Ű� ���´�.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
			// 5.2. �Ʒ� �ٷ� �̵��Ѵ�.
			i++;
			line = bottomLine;
			// 5.3. �� �ʺ� ���.
			lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
		}

		Point currentPosition = this->CovPosToAutoLineBreak(previousPosition);
		this->notepadForm->note->Move(currentPosition.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentPosition.y);
		this->notepadForm->current->Move(currentPosition.x);
	}

	
	
}


/**********************************
�Լ���:Update
�Է�: �� ��ȣ
���: 
���: NotepadForm�� �ٹ�ȣ�� �ش��ϴ� ���� �ڵ������Ѵ�.
***********************************/
void AutoLineBreaker::Update(Long lineNumber){
	
	Glyph *line;
	Glyph *bottomLine = 0;
	
	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left; // 1. Ŭ���̾�Ʈ �ʺ� ���.
	Long lineWidth;
	Long characterWidth = 0;
	Long i;
	Long index;
	Long count;

	// 2. ���� �ʺ� ���.
	line = this->notepadForm->note->GetAt(lineNumber);
	lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

	// 3. �Ʒ� ���� ���� ���̸� �Ʒ� ���� ù ������ �ʺ����.
	if (lineNumber + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(lineNumber + 1);
	}

	if (dynamic_cast<DummyLine*>(bottomLine)) {
		characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);
	}


	// 4. �Ʒ� ���� ���� ���̰� ���� �ʺ�� ������ �ʺ� ���Ѱ� Ŭ���̾�Ʈ �ʺ񺸴� ������
	if (dynamic_cast<DummyLine*>(bottomLine) && lineWidth + characterWidth < clientWidth) {
		// 4.1. �Ʒ� ���� �ٳ��� �Ű� ���´�.
		line->Combine(bottomLine);
		this->notepadForm->note->Remove(lineNumber + 1);
		// 4.2. ���� �ʺ� ���.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
	}
	
	if (lineWidth > clientWidth) {// 5. Ŭ���̾�Ʈ �ʺ񺸴� ũ��
		// 5.1. �ٺ��� ������ �ٱ��� �׸��� ���� �ʺ� Ŭ���̾�Ʈ �ʺ񺸴� ū ���� �ݺ��Ѵ�.
		i = lineNumber;
		while (i <this->notepadForm->note->GetLength() && lineWidth > clientWidth) {
			//  5.1.1. Ŭ���̾�Ʈ �ʺ� ��� ������ ã�´�.
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth);
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}
			
			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  5.1.2. �Ʒ� ���� ���� ���� �ƴϸ�
				//   5.1.2.1. ���� ���� �����.
				bottomLine = new DummyLine;
				//   5.1.2.2. �Ʒ��� �����.
				this->notepadForm->note->Add(i + 1, bottomLine);
			}
			
			count = line->GetLength() - index;
			Long j = 0;
			while (j < count) {//  5.1.3. Ŭ���̾�Ʈ �ʺ� ��� �κ��� �Ʒ� ���� ù �κп� �Ű� ���´�.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
			// 5.2. �Ʒ� �ٷ� �̵��Ѵ�.
			i++;
			line = bottomLine;
			// 5.3. �� �ʺ� ���.
			lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
		}
		
	}

}
/****************************
�Լ� ��Ī: UpdateAll
���: �޸����� ��� �ٵ��� Ŭ���̾�Ʈ�� �ʺ� �°� �ڵ����� �Ǿ����� �˻��ϰ� �����Ѵ�.
�Է�:
���:
******************************/
void AutoLineBreaker::UpdateAll() {
	Glyph *line;
	Glyph *bottomLine ;
	RECT clientRect;
	Long clientWidth ; // 1. Ŭ���̾�Ʈ �ʺ� ���.
	Long lineWidth;
	Long characterWidth;
	Long i = 0;
	Long index;
	Long count;
	Long j;

	//1. �ڵ��������� ��ġ�� ���ϴ�.
	Point previousPosition = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	previousPosition = this->CovPosToNonAutoLineBreak(previousPosition);
	//2. Ŭ���̾�Ʈ �ʺ� ���.
	this->notepadForm->GetClientRect(&clientRect);
	clientWidth = clientRect.right - clientRect.left;
	
	while (i < this->notepadForm->note->GetLength()) {//3. �޸����� ���̸�ŭ �ݺ��Ѵ�.
		// 3.1. ���� �ʺ� ���.
		line = this->notepadForm->note->GetAt(i);
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

		bottomLine = 0;
		if (i + 1 < this->notepadForm->note->GetLength()) {
			bottomLine = this->notepadForm->note->GetAt(i + 1);
		}
		
		if (dynamic_cast<DummyLine*>(bottomLine)) {// 3.2. �Ʒ� ���� ���� ���̸�
			
			characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);//  3.2.1 �Ʒ� ���� ù ������ �ʺ� ���.

			if (lineWidth + characterWidth < clientWidth) { //  3.2.2. ���� �ʺ�� ������ �ʺ��� ���� Ŭ���̾�Ʈ �ʺ񺸴� ������ 
				line->Combine(bottomLine); //   3.2.2.1. �Ʒ� ���� �� ���� �Ű� ���´�.
				this->notepadForm->note->Remove(i + 1); //   3.2.2.2. ���� �ʺ� ���.
				
				lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
			}
		}
		
		if (lineWidth > clientWidth) {// 3.3. ���� �ʺ� Ŭ���̾�Ʈ �ʺ񺸴� ũ��
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth); //  3.3.1. �ٿ��� ��� ������ ã�´�.
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}

			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  3.3.2. �Ʒ� ���� ���� ���� �ƴϸ�
				bottomLine = new DummyLine;//   3.3.2.1. ���� ���� �����.
				this->notepadForm->note->Add(i + 1, bottomLine);//   3.3.2.2. �Ʒ��� �����.
			}

			count = line->GetLength() - index;
			j = 0;
			while (j < count) {//  3.3.3. �Ʒ� �� ù �κп� �ٿ��� ��� �κ��� �Ű� ���´�.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
		}
		i++;
	}
	
	// 4. �ڵ� �������� ��ġ�� ���ϴ�.
	Point currentPosition = this->CovPosToAutoLineBreak(previousPosition);
	// 5. �޸����������� �޸��忡�� �̵��ϴ�.
	this->notepadForm->note->Move(currentPosition.y);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentPosition.y);
	this->notepadForm->current->Move(currentPosition.x);
}

/*****
�Լ� ��Ī: ConvPosToAutoLineBreak;
���: �ڵ� ���� ���� ��ġ�� �Է¹޾Ƽ� �ڵ� ���� ���� ��ġ�� ����Ѵ�.
�Է�: �ڵ� ���� ���� ��ġ
���: �ڵ� ���� ���� ��ġ

***/
Point AutoLineBreaker::CovPosToAutoLineBreak(Point posNonAutoLineBreak) {
	Point posAutoLineBreak = { posNonAutoLineBreak.x, -1 };
	Glyph *line;
	Long lineCount = 0;
	Long i = 0;

	while (i < this->notepadForm->note->GetLength() && lineCount < posNonAutoLineBreak.y + 1) {
		line = this->notepadForm->note->GetAt(i);
		if (!dynamic_cast<DummyLine*>(line)) {
			lineCount++;
		}
		posAutoLineBreak.y++;

		i++;
	}

	line = this->notepadForm->note->GetAt(posAutoLineBreak.y);
	while (posAutoLineBreak.x > line->GetLength()) {
		posAutoLineBreak.x = posAutoLineBreak.x - line->GetLength();
		posAutoLineBreak.y++;
		line = this->notepadForm->note->GetAt(posAutoLineBreak.y);
	}

	return posAutoLineBreak;
}


/*****
�Լ� ��Ī: ConvPosToNonAutoLineBreak;
���: �ڵ� ���� ���� ��ġ�� �Է¹޾Ƽ� �ڵ� ���� ���� ��ġ�� ����Ѵ�.
�Է�: �ڵ� ���� ���� ��ġ
���: �ڵ� ���� ���� ��ġ

***/
Point AutoLineBreaker::CovPosToNonAutoLineBreak(Point posAutoLineBreak) {
	Point posNonAutoLineBreak = { 0,-1 };
	Glyph *line;
	Long startNumber;
	Long i = 0;
	while (i <= posAutoLineBreak.y) {
		line = this->notepadForm->note->GetAt(i);
		if (!dynamic_cast<DummyLine*>(line) ){
			posNonAutoLineBreak.y++;
			startNumber = i;
		}
		i++;
	}
	i = startNumber;
	while (i < posAutoLineBreak.y) {
		line = this->notepadForm->note->GetAt(i);
		posNonAutoLineBreak.x += line->GetLength();

		i++;
	}
	posNonAutoLineBreak.x += posAutoLineBreak.x;
	
	return posNonAutoLineBreak;
}




void AutoLineBreaker::Cancel() {
	Long i;
	Glyph *line;
	Glyph *topLine;

	i = 1;
	while (i < this->notepadForm->note->GetLength()) {// 1. �޸����� �ι�° �ٺ��� ������ �ٱ��� �ݺ��Ѵ�.
		line = this->notepadForm->note->GetAt(i);
		if (dynamic_cast<DummyLine*>(line)) {//  1.1. ���� ���̸�
			//   1.1.1. �� �ٰ� ��ģ��.
			topLine = this->notepadForm->note->GetAt(i - 1);
			topLine->Combine(line);
			this->notepadForm->note->Remove(i);
		}
		else {
			i++;//  1.2. ���� ���� �ƴϸ� ���� �ٷ� �Ѿ��.
		}
	}
}

void AutoLineBreaker::Compact(Long lineNumber) {
	Long i = lineNumber;
	Glyph *line;
	Glyph *bottomLine;
	Glyph *character;
	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left;
	Long lineWidth;
	Long whiteSpaceWidthOfLine;
	Long characterWidth;
	Long indexOfNoteCurrent = this->notepadForm->note->GetCurrent();
	Long indexOfLineCurrent = this->notepadForm->note->GetAt(indexOfNoteCurrent)->GetCurrent();

	line = this->notepadForm->note->GetAt(i);
	
	while (i == lineNumber || dynamic_cast<DummyLine*>(line)) { //1. ��ȣ�� �ش��ϴ� ���̰ų� �������� ���� �ݺ��Ѵ�.
		// 1.1.���� ���� ���� ���.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line, line->GetLength());
		whiteSpaceWidthOfLine = clientWidth - lineWidth;

		if (i + 1 < this->notepadForm->note->GetLength()) {
			bottomLine = this->notepadForm->note->GetAt(i + 1);
		}
		else {
			bottomLine = 0;
		}

		// 1.2.�Ʒ� ���� �ְ� �Ʒ� ���� ���� ���̸�
		if (bottomLine != 0 && dynamic_cast<DummyLine*>(bottomLine)) {
			if (bottomLine->GetLength() > 0) {
				character = bottomLine->GetAt(0);
				characterWidth = this->notepadForm->characterMetrics->GetWidth(character->GetContent());
			}
			else {
				character = 0;
			}
			while (character != 0 && characterWidth < whiteSpaceWidthOfLine) {//  1.2.1. �Ʒ� ���� ���ڰ� �ְ� ù ���� ���� ���� ������ ���� ���� �ݺ��Ѵ�.
				//   1.2.1.1. �Ʒ� ���� ù ���ڸ� ���� ���� ������ �ű��.
				line->Add(character->Clone());
				whiteSpaceWidthOfLine -= characterWidth;
				bottomLine->Remove(0);

				if (bottomLine->GetLength() > 0) {
					character = bottomLine->GetAt(0);
					characterWidth = this->notepadForm->characterMetrics->GetWidth(character->GetContent());
				}
				else {// �Ʒ� ���� ���ڰ� �� �̻� ���� ��� �Ʒ� ���� �����.
					character = 0;
					
					this->notepadForm->note->Remove(i + 1);
				}
			}

		}

		i++;
		if (i < this->notepadForm->note->GetLength()) {
			line = this->notepadForm->note->GetAt(i);
		}
		else {
			line = 0;
		}
	}
	
	// �ڵ� �����۾����� �ٲ� ��Ʈ�� current ��ġ ���� ����
	this->notepadForm->note->Move(indexOfNoteCurrent);
	this->notepadForm->current = this->notepadForm->note->GetAt(indexOfNoteCurrent);
	this->notepadForm->current->Move(indexOfLineCurrent);
}

Observer* AutoLineBreaker::GetClone() {
	return new AutoLineBreaker(*this);
}


AutoLineBreaker& AutoLineBreaker::operator=(const AutoLineBreaker& source){
	this->notepadForm = source.notepadForm;
	return *this;
}
