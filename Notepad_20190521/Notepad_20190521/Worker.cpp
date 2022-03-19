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
�Լ� ��Ī: Record
��    ��: �Էµ� �۾��� �۾������� ������ �۾��� ���Ͽ� ���ӵ� �۾�
          �� �ƴϸ� �۾������� ��ü�Ŀ� ����Ѵ�.
��    ��: workType, row, column
��    ��: 


void Worker::Record(WorkType workType, Glyph *glyph, Long row, Long column){
	CompletedWork *lastWork = 0;
	Long rowOfLastWork;
	Long columnOfLastWork;
	
	if (this->workLog.GetLength() > 0) {//1. �۾������� �۾� ������ ������ ������ �۾��� �д´�.
		lastWork = this->workLog.GetAt(this->workLog.GetLength() - 1);
		rowOfLastWork = lastWork->GetRow();
		columnOfLastWork = lastWork->GetColumn();
		if (workType == AddCharacter || workType == LineSeperation) {//1.1. ���� �۾��� ����(AddCharacter or LineSeperation)�۾��̸�
			if (dynamic_cast<AddCharacterWork*>(lastWork)) { //1.1.1. ������ �۾��� AddCharacter������
				if (!(rowOfLastWork == row && columnOfLastWork == column - 1)) {//1.1.1.1. lastRow = row, lastColumn = column - 1�� �������� ������ �۾������� ��ü�Ѵ�.
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<LineSeperationWork*>(lastWork)) {//1.1.2. ������ �۾��� LineSeperation������
				if (!(rowOfLastWork == row - 1 && column == 0  )) {//1.1.2.1. lastRow + 1 = row�� , column = 0 �� �������� ������ �۾������� ��ü�Ѵ�.
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork) ||
				dynamic_cast<LineCombinationWork*>(lastWork)) {//1.1.3. ������ �۾��� RemoveCharacterWork �Ǵ� LineCombinationWork������
				if (!(rowOfLastWork == row && columnOfLastWork == column)) {
					this->workLog.Reset();
				}
			}
		}
		else if (workType == RemoveCharacter) { //1.2. �Է� �۾��� RemoveCharacter �۾��̸�
			if(dynamic_cast<AddCharacterWork*>(lastWork) ||
				dynamic_cast<LineSeperationWork*>(lastWork)) {//1.2.1. ������ �۾��� AddCharacter �Ǵ� LineSeperation�̸� �۾������� ��ü�Ѵ�.
				this->workLog.Reset();
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork) || 
				dynamic_cast<LineCombinationWork*>(lastWork)) { 
				//1.2.2. lastRow = row, lastColumn = column OR lastColumn = column + 1 �� �������� ������ �۾������� ��ü�Ѵ�.
				if (!(rowOfLastWork == row && (columnOfLastWork == column || columnOfLastWork == column + 1))) {
					this->workLog.Reset();
				}
			}
		}
		else if (workType == LineCombination) { //1.3. �Է� �۾��� LineCombination �۾��̸�
			if (dynamic_cast<AddCharacterWork*>(lastWork) ||
				dynamic_cast<LineSeperationWork*>(lastWork)) {//1.3.1. ������ �۾��� AddCharacter �Ǵ� LineSeperation�̸� �۾������� ��ü�Ѵ�.
				this->workLog.Reset();
			}
			else if (dynamic_cast<RemoveCharacterWork*>(lastWork)) {//1.3.2. ������ �۾��� RemoveCharacter�̸�
				//1.3.2.1. (lastRow = row, lastColumn = column) OR (lastRow+1 = row, lastColumn = 0) �� �������� ������ �۾������� ��ü�Ѵ�.
				if (!((rowOfLastWork == row && columnOfLastWork == column) || (rowOfLastWork == row + 1 && columnOfLastWork == 0))) {
					this->workLog.Reset();
				}
			}
			else if (dynamic_cast<LineCombinationWork*>(lastWork)) {//1.3.3. ������ �۾��� LineCombination�̸�
				//1.3.3.1. lastRow - 1 = row OR lastRow = row, lastColumn = 0 �� �������� ������ �۾������� ��ü�Ѵ�.
				if (!((rowOfLastWork == row + 1  && columnOfLastWork == 0) || (rowOfLastWork == row && columnOfLastWork == column))) {
					this->workLog.Reset();
				}
			}
		}
	}

	// 2. CompletedWorkFactory�� �����.
	CompletedWorkFactory completedWowrFactory;
	// 3. CompletedWorkFactory���� �۾��� �����.
	CompletedWork *work = completedWowrFactory.Make(workType, glyph, row, column);
	// 4. �۾��������� ����Ѵ�.
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

		// ADD �۾��̸�
		if (dynamic_cast<AddCharacterWork*>(work)) {
			//�ش� ���ڸ������.
			index = this->notepadForm->note->Move(row);
			line = this->notepadForm->note->GetAt(index);
			line->Remove(column);
			reverseWork = completedWorkFactory.Make(RemoveCharacter, glyph->Clone(), row, column);
		}// glyph�� Line�и� �۾��̸�
		else if (dynamic_cast<LineSeperationWork*>(work)) {
			//row��° �ٰ� row+1��° ���� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(row);
			index = this->notepadForm->current->GetLength();
			Glyph *bottomLine = this->notepadForm->note->GetAt(row + 1);
			this->notepadForm->current->Combine(bottomLine);
			this->notepadForm->current->Move(index);
			// row+1��° ���� �����Ѵ�.
			this->notepadForm->note->Remove(row + 1);
			// Line ���� �۾��� �����.
			reverseWork = completedWorkFactory.Make(LineCombination, 0, row, column);
		}
		//Remove �۾��̸�
		else if (dynamic_cast<RemoveCharacterWork*>(work)) {
			index = this->notepadForm->note->Move(row);
			line = this->notepadForm->note->GetAt(index);
			// ���ڸ� ����.
			line->Add(column, glyph->Clone());
			reverseWork = completedWorkFactory.Make(AddCharacter, glyph->Clone(), row, column);
		}// glyph�� 0�̸�(�� ���� �۾��̿�����)
		else if (dynamic_cast<LineCombinationWork*>(work)) {
			// ���� �и��Ѵ�.
			glyph = this->notepadForm->note->GetAt(row);
			glyph = glyph->Divide(column);
			// �Ʒ� �ٿ� �߰��Ѵ�.
			this->notepadForm->note->Add(row + 1, glyph);
			this->notepadForm->current = this->notepadForm->note->GetAt(row + 1);
			this->notepadForm->current->First();
			// �� �и� �۾��� �����.
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