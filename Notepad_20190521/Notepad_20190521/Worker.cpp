/*
#include "Worker.h"
#include "NotepadForm.h"
#include "CompletedWork.h"
#include "Glyph.h"

Worker::Worker(NotepadForm *notepadForm )
:workLog(256){
	this->notepadForm = notepadForm;
}

Worker::Worker(const Worker& source)
:workLog(source.workLog){
	this->notepadForm = source.notepadForm;
}

Worker::~Worker(){
}

/***************************************************************
함수 명칭: Record
기    능: 입력된 작업과 작업일지의 마지막 작업과 비교하여 연속된 작업
          이 아니면 작업일지를 교체후에 기록한다.
입    력: workType, row, column
출    력: 


void Worker::Record(WorkType workType, Glyph *glyph, Long row, Long column){
	CompletedWork *lastWork = 0;
	Long rowOfLastWork;
	Long columnOfLastWork;
	
	if (this->workLog.GetLength() > 0) {//1. 작업일지에 작업 내용이 있으면 마지막 작업을 읽는다.
		lastWork = this->workLog.GetAt(this->workLog.GetLength() - 1);
		rowOfLastWork = lastWork->GetRow();
		columnOfLastWork = lastWork->GetColumn();
		if (workType == AddCharacter || workType == LineSeperation) {//1.1. 현재 작업이 쓰기(AddCharacter or LineSeperation)작업이면
			if (dynamic_cast<AddCharacterWork*>(lastWork)) { //1.1.1. 마지막 작업이 AddCharacter였으면
				if (!(rowOfLastWork == row && columnOfLastWork == column - 1)) {//1.1.1.1. lastRow = row, lastColumn = column - 1을 만족하지 않으면 작업일지를 교체한다.
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<LineSeperationWork*>(lastWork)) {//1.1.2. 마지막 작업이 LineSeperation였으면
				if (!(rowOfLastWork == row - 1 && column == 0  )) {//1.1.2.1. lastRow + 1 = row가 , column = 0 을 만족하지 않으면 작업일지를 교체한다.
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork) ||
				dynamic_cast<LineCombinationWork*>(lastWork)) {//1.1.3. 마지막 작업이 RemoveCharacterWork 또는 LineCombinationWork였으면
				if (!(rowOfLastWork == row && columnOfLastWork == column)) {
					this->workLog.Reset();
				}
			}
		}
		else if (workType == RemoveCharacter) { //1.2. 입력 작업이 RemoveCharacter 작업이면
			if(dynamic_cast<AddCharacterWork*>(lastWork) ||
				dynamic_cast<LineSeperationWork*>(lastWork)) {//1.2.1. 마지막 작업이 AddCharacter 또는 LineSeperation이면 작업일지를 교체한다.
				this->workLog.Reset();
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork) || 
				dynamic_cast<LineCombinationWork*>(lastWork)) { 
				//1.2.2. lastRow = row, lastColumn = column OR lastColumn = column + 1 을 만족하지 않으면 작업일지를 교체한다.
				if (!(rowOfLastWork == row && (columnOfLastWork == column || columnOfLastWork == column + 1))) {
					this->workLog.Reset();
				}
			}
		}
		else if (workType == LineCombination) { //1.3. 입력 작업이 LineCombination 작업이면
			if (dynamic_cast<AddCharacterWork*>(lastWork) ||
				dynamic_cast<LineSeperationWork*>(lastWork)) {//1.3.1. 마지막 작업이 AddCharacter 또는 LineSeperation이면 작업일지를 교체한다.
				this->workLog.Reset();
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork)) {//1.3.2. 마지막 작업이 RemoveCharacter이면
				//1.3.2.1. (lastRow = row, lastColumn = column) OR (lastRow+1 = row, lastColumn = 0) 을 만족하지 않으면 작업일지를 교체한다.
				if (!((rowOfLastWork == row && columnOfLastWork == column) || (rowOfLastWork == row + 1 && columnOfLastWork == 0))) {
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<LineCombinationWork*>(lastWork)) {//1.3.3. 마지막 작업이 LineCombination이면
				//1.3.3.1. lastRow - 1 = row OR lastRow = row, lastColumn = 0 을 만족하지 않으면 작업일지를 교체한다.
				if (!((rowOfLastWork == row + 1  && columnOfLastWork == 0) || (rowOfLastWork == row && columnOfLastWork == column))) {
					this->workLog.Reset();
				}
			}
		}
	}

	// 2. CompletedWorkFactory를 만든다.
	CompletedWorkFactory completedWowrFactory;
	// 3. CompletedWorkFactory에서 작업을 만든다.
	CompletedWork *work = completedWowrFactory.Make(workType, glyph, row, column);
	// 4. 작업일지에서 기록한다.
	this->workLog.Log(work);
}

void Worker::Undo(){
	CompletedWorkFactory completedWorkFactory;
	CompletedWork *work;
	Glyph *glyph;
	Glyph *line;
	Long row;
	Long column;
	Long index;
	CompletedWork *reverseWork;
	WorkLog newWorkLog;
	Long i = this->workLog.GetLength() - 1;


	while (i >= 0) {
		work = this->workLog.GetAt(i);

		glyph = work->GetGlyph();
		row = work->GetRow();
		column = work->GetColumn();

		// ADD 작업이면
		if (dynamic_cast<AddCharacterWork*>(work)) {
			//해당 글자를지운다.
			index = this->notepadForm->note->Move(row);
			line = this->notepadForm->note->GetAt(index);
			line->Remove(column);
			reverseWork = completedWorkFactory.Make(RemoveCharacter, glyph->Clone(), row, column);
		}// glyph가 Line분리 작업이면
		else if (dynamic_cast<LineSeperationWork*>(work)) {
			//row번째 줄과 row+1번째 줄을 병합한다.
			this->notepadForm->current = this->notepadForm->note->GetAt(row);
			index = this->notepadForm->current->GetLength();
			Glyph *bottomLine = this->notepadForm->note->GetAt(row + 1);
			this->notepadForm->current->Combine(bottomLine);
			this->notepadForm->current->Move(index);
			// row+1번째 줄을 삭제한다.
			this->notepadForm->note->Remove(row + 1);
			// Line 병합 작업을 만든다.
			reverseWork = completedWorkFactory.Make(LineCombination, 0, row, column);
		}
		//Remove 작업이면
		else if (dynamic_cast<RemoveCharacterWork*>(work)) {
			index = this->notepadForm->note->Move(row);
			line = this->notepadForm->note->GetAt(index);
			// 글자를 쓴다.
			line->Add(column, glyph->Clone());
			reverseWork = completedWorkFactory.Make(AddCharacter, glyph->Clone(), row, column);
		}// glyph가 0이면(줄 병합 작업이였으면)
		else if (dynamic_cast<LineCombinationWork*>(work)) {
			// 줄을 분리한다.
			glyph = this->notepadForm->note->GetAt(row);
			glyph = glyph->Divide(column);
			// 아랫 줄에 추가한다.
			this->notepadForm->note->Add(row + 1, glyph);
			this->notepadForm->current = this->notepadForm->note->GetAt(row + 1);
			this->notepadForm->current->First();
			// 줄 분리 작업을 만든다.
			reverseWork = completedWorkFactory.Make(LineSeperation, 0, row, column);
		}

		newWorkLog.Log(reverseWork);
		i--;
	}
	this->workLog = newWorkLog;

	row = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(row);
}

Worker& Worker::operator=(const Worker& source){
	this->workLog = source.workLog;
	this->notepadForm = source.notepadForm;

	return *this;
}

*/