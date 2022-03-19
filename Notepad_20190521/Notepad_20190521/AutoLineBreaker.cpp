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
함수명:Update
입력: 
출력:
기능: NotepadForm의 current줄을 자동개행한다.
***********************************/

void AutoLineBreaker::Update() {

	Glyph *line;
	Glyph *bottomLine = 0;

	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left; // 1. 클라이언트 너비를 재다.
	Long lineWidth;
	Long characterWidth = 0;
	Long i;
	Long index;
	Long count;
	Long lineNumber = this->notepadForm->note->GetCurrent();

	Point previousPosition = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	previousPosition = this->CovPosToNonAutoLineBreak(previousPosition);

	// 2. 줄의 너비를 재다.
	line = this->notepadForm->note->GetAt(lineNumber);
	lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

	// 3. 아래 줄이 더미 줄이면 아래 줄의 첫 글자의 너비를재다.
	if (lineNumber + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(lineNumber + 1);
		
	}

	if (dynamic_cast<DummyLine*>(bottomLine)) {
		characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);
	}
	// 4. 아래 줄이 더미 줄이고 줄의 너비와 글자의 너비를 합한게 클라이언트 너비보다 작으면
	if (dynamic_cast<DummyLine*>(bottomLine) && lineWidth + characterWidth < clientWidth) {
		// 4.1. 아래 줄을 줄끝에 옮겨 적는다.
		line->Combine(bottomLine);
		this->notepadForm->note->Remove(lineNumber + 1);
		// 4.2. 줄의 너비를 재다.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
	}

	if (lineWidth >= clientWidth) {// 5. 클라이언트 너비보다 크면
		// 5.1. 줄부터 마지막 줄까지 그리고 줄의 너비가 클라이언트 너비보다 큰 동안 반복한다.
		i = lineNumber;
		while (i <this->notepadForm->note->GetLength() && lineWidth >= clientWidth) {
			//  5.1.1. 클라이언트 너비를 벗어난 지점을 찾는다.
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth);
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}

			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  5.1.2. 아래 줄이 더미 줄이 아니면
				//   5.1.2.1. 더미 줄을 만들다.
				bottomLine = new DummyLine;
				//   5.1.2.2. 아래에 끼운다.
				this->notepadForm->note->Add(i + 1, bottomLine);
			}

			count = line->GetLength() - index;
			Long j = 0;
			while (j < count) {//  5.1.3. 클라이언트 너비를 벗어난 부분을 아래 줄의 첫 부분에 옮겨 적는다.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
			// 5.2. 아래 줄로 이동한다.
			i++;
			line = bottomLine;
			// 5.3. 줄 너비를 재다.
			lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
		}

		Point currentPosition = this->CovPosToAutoLineBreak(previousPosition);
		this->notepadForm->note->Move(currentPosition.y);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentPosition.y);
		this->notepadForm->current->Move(currentPosition.x);
	}

	
	
}


/**********************************
함수명:Update
입력: 줄 번호
출력: 
기능: NotepadForm의 줄번호의 해당하는 줄을 자동개행한다.
***********************************/
void AutoLineBreaker::Update(Long lineNumber){
	
	Glyph *line;
	Glyph *bottomLine = 0;
	
	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left; // 1. 클라이언트 너비를 재다.
	Long lineWidth;
	Long characterWidth = 0;
	Long i;
	Long index;
	Long count;

	// 2. 줄의 너비를 재다.
	line = this->notepadForm->note->GetAt(lineNumber);
	lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

	// 3. 아래 줄이 더미 줄이면 아래 줄의 첫 글자의 너비를재다.
	if (lineNumber + 1 < this->notepadForm->note->GetLength()) {
		bottomLine = this->notepadForm->note->GetAt(lineNumber + 1);
	}

	if (dynamic_cast<DummyLine*>(bottomLine)) {
		characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);
	}


	// 4. 아래 줄이 더미 줄이고 줄의 너비와 글자의 너비를 합한게 클라이언트 너비보다 작으면
	if (dynamic_cast<DummyLine*>(bottomLine) && lineWidth + characterWidth < clientWidth) {
		// 4.1. 아래 줄을 줄끝에 옮겨 적는다.
		line->Combine(bottomLine);
		this->notepadForm->note->Remove(lineNumber + 1);
		// 4.2. 줄의 너비를 재다.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
	}
	
	if (lineWidth > clientWidth) {// 5. 클라이언트 너비보다 크면
		// 5.1. 줄부터 마지막 줄까지 그리고 줄의 너비가 클라이언트 너비보다 큰 동안 반복한다.
		i = lineNumber;
		while (i <this->notepadForm->note->GetLength() && lineWidth > clientWidth) {
			//  5.1.1. 클라이언트 너비를 벗어난 지점을 찾는다.
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth);
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}
			
			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  5.1.2. 아래 줄이 더미 줄이 아니면
				//   5.1.2.1. 더미 줄을 만들다.
				bottomLine = new DummyLine;
				//   5.1.2.2. 아래에 끼운다.
				this->notepadForm->note->Add(i + 1, bottomLine);
			}
			
			count = line->GetLength() - index;
			Long j = 0;
			while (j < count) {//  5.1.3. 클라이언트 너비를 벗어난 부분을 아래 줄의 첫 부분에 옮겨 적는다.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
			// 5.2. 아래 줄로 이동한다.
			i++;
			line = bottomLine;
			// 5.3. 줄 너비를 재다.
			lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
		}
		
	}

}
/****************************
함수 명칭: UpdateAll
기능: 메모장의 모든 줄들이 클라이언트의 너비에 맞게 자동개행 되었는지 검사하고 조정한다.
입력:
출력:
******************************/
void AutoLineBreaker::UpdateAll() {
	Glyph *line;
	Glyph *bottomLine ;
	RECT clientRect;
	Long clientWidth ; // 1. 클라이언트 너비를 재다.
	Long lineWidth;
	Long characterWidth;
	Long i = 0;
	Long index;
	Long count;
	Long j;

	//1. 자동개행전의 위치를 구하다.
	Point previousPosition = { this->notepadForm->current->GetCurrent(), this->notepadForm->note->GetCurrent() };
	previousPosition = this->CovPosToNonAutoLineBreak(previousPosition);
	//2. 클라이언트 너비를 재다.
	this->notepadForm->GetClientRect(&clientRect);
	clientWidth = clientRect.right - clientRect.left;
	
	while (i < this->notepadForm->note->GetLength()) {//3. 메모장의 길이만큼 반복한다.
		// 3.1. 줄의 너비를 재다.
		line = this->notepadForm->note->GetAt(i);
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line);

		bottomLine = 0;
		if (i + 1 < this->notepadForm->note->GetLength()) {
			bottomLine = this->notepadForm->note->GetAt(i + 1);
		}
		
		if (dynamic_cast<DummyLine*>(bottomLine)) {// 3.2. 아래 줄이 더미 줄이면
			
			characterWidth = this->notepadForm->characterMetrics->GetWidth(bottomLine, 1);//  3.2.1 아래 줄의 첫 글자의 너비를 재다.

			if (lineWidth + characterWidth < clientWidth) { //  3.2.2. 줄의 너비와 글자의 너비의 합이 클라이언트 너비보다 작으면 
				line->Combine(bottomLine); //   3.2.2.1. 아래 줄을 줄 끝에 옮겨 적는다.
				this->notepadForm->note->Remove(i + 1); //   3.2.2.2. 줄의 너비를 재다.
				
				lineWidth = this->notepadForm->characterMetrics->GetWidth(line);
			}
		}
		
		if (lineWidth > clientWidth) {// 3.3. 줄의 너비가 클라이언트 너비보다 크면
			index = this->notepadForm->characterMetrics->GetColumnBeforeX(line, clientWidth); //  3.3.1. 줄에서 벗어난 지점을 찾는다.
			bottomLine = 0;
			if (i + 1 < this->notepadForm->note->GetLength()) {
				bottomLine = this->notepadForm->note->GetAt(i + 1);
			}

			if (!dynamic_cast<DummyLine*>(bottomLine)) {//  3.3.2. 아래 줄이 더미 줄이 아니면
				bottomLine = new DummyLine;//   3.3.2.1. 더미 줄을 만든다.
				this->notepadForm->note->Add(i + 1, bottomLine);//   3.3.2.2. 아래에 끼운다.
			}

			count = line->GetLength() - index;
			j = 0;
			while (j < count) {//  3.3.3. 아래 줄 첫 부분에 줄에서 벗어난 부분을 옮겨 적는다.
				bottomLine->Add(j, line->GetAt(index)->Clone());
				line->Remove(index);
				j++;
			}
		}
		i++;
	}
	
	// 4. 자동 개행후의 위치를 구하다.
	Point currentPosition = this->CovPosToAutoLineBreak(previousPosition);
	// 5. 메모장윈도우의 메모장에서 이동하다.
	this->notepadForm->note->Move(currentPosition.y);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentPosition.y);
	this->notepadForm->current->Move(currentPosition.x);
}

/*****
함수 명칭: ConvPosToAutoLineBreak;
기능: 자동 개행 전의 위치를 입력받아서 자동 개행 후의 위치를 출력한다.
입력: 자동 개행 전의 위치
출력: 자동 개행 후의 위치

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
함수 명칭: ConvPosToNonAutoLineBreak;
기능: 자동 개행 후의 위치를 입력받아서 자동 개행 전의 위치를 출력한다.
입력: 자동 개행 후의 위치
출력: 자동 개행 전의 위치

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
	while (i < this->notepadForm->note->GetLength()) {// 1. 메모장의 두번째 줄부터 마지막 줄까지 반복한다.
		line = this->notepadForm->note->GetAt(i);
		if (dynamic_cast<DummyLine*>(line)) {//  1.1. 더미 줄이면
			//   1.1.1. 윗 줄과 합친다.
			topLine = this->notepadForm->note->GetAt(i - 1);
			topLine->Combine(line);
			this->notepadForm->note->Remove(i);
		}
		else {
			i++;//  1.2. 더미 줄이 아니면 다음 줄로 넘어간다.
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
	
	while (i == lineNumber || dynamic_cast<DummyLine*>(line)) { //1. 번호에 해당하는 줄이거나 더미줄인 동안 반복한다.
		// 1.1.줄의 여백 폭을 잰다.
		lineWidth = this->notepadForm->characterMetrics->GetWidth(line, line->GetLength());
		whiteSpaceWidthOfLine = clientWidth - lineWidth;

		if (i + 1 < this->notepadForm->note->GetLength()) {
			bottomLine = this->notepadForm->note->GetAt(i + 1);
		}
		else {
			bottomLine = 0;
		}

		// 1.2.아래 줄이 있고 아래 줄이 더미 줄이면
		if (bottomLine != 0 && dynamic_cast<DummyLine*>(bottomLine)) {
			if (bottomLine->GetLength() > 0) {
				character = bottomLine->GetAt(0);
				characterWidth = this->notepadForm->characterMetrics->GetWidth(character->GetContent());
			}
			else {
				character = 0;
			}
			while (character != 0 && characterWidth < whiteSpaceWidthOfLine) {//  1.2.1. 아래 줄의 글자가 있고 첫 글자 폭이 여백 폭보다 작은 동안 반복한다.
				//   1.2.1.1. 아래 줄의 첫 글자를 현재 줄의 끝으로 옮긴다.
				line->Add(character->Clone());
				whiteSpaceWidthOfLine -= characterWidth;
				bottomLine->Remove(0);

				if (bottomLine->GetLength() > 0) {
					character = bottomLine->GetAt(0);
					characterWidth = this->notepadForm->characterMetrics->GetWidth(character->GetContent());
				}
				else {// 아래 줄의 글자가 더 이상 없을 경우 아래 줄을 지운다.
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
	
	// 자동 개행작업으로 바뀐 노트의 current 위치 원상 복구
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
