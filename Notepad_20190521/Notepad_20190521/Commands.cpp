//Commands.cpp

#include "Commands.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "GlyphFactory.h"
#include "CaretController.h"
#include "DummyLine.h"
#include "ScrollController.h"
#include "AutoLineBreaker.h"
#include "resource.h"
#include "NoteStates.h"

Command::Command(NotepadForm *notepadForm ) {
	this->notepadForm = notepadForm;
}

Command::Command(const Command& source){
	this->notepadForm = source.notepadForm;
}

Command::~Command() {

}

Command& Command::operator=(const Command& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

// AutoLineBreakCommand
AutoLineBreakCommand::AutoLineBreakCommand(NotepadForm *notepadForm)
:Command(notepadForm){
}

AutoLineBreakCommand::AutoLineBreakCommand(const AutoLineBreakCommand& source)
:Command(source.notepadForm){
}

AutoLineBreakCommand::~AutoLineBreakCommand(){
}

void AutoLineBreakCommand::Execute(){
	UINT state = this->notepadForm->menu.GetMenuState(IDM_AUTO_LINEBREAK, MF_BYCOMMAND);

	if (this->notepadForm->scrollController != 0) {
		this->notepadForm->scrollController->SmartScrollToPoint(0, 0);
		delete this->notepadForm->scrollController;
	}

	if (state == MF_UNCHECKED) { 
		//�ڵ������ڸ� �����.
		this->notepadForm->autoLineBreaker = new AutoLineBreaker(this->notepadForm);
		//�ڵ������ڿ��� ��� ���� ������Ʈ �Ѵ�.
		this->notepadForm->autoLineBreaker->UpdateAll();
		//�ڵ����� �޴��� ���û��·� �����.
		this->notepadForm->menu.CheckMenuItem(IDM_AUTO_LINEBREAK, MF_CHECKED);
	}
	else if (state == MF_CHECKED) { 
		this->notepadForm->autoLineBreaker->Cancel();

		//�ڵ����� �޴��� ���û��·� �����.
		this->notepadForm->menu.CheckMenuItem(IDM_AUTO_LINEBREAK, MF_UNCHECKED);

		//�ڵ������ڸ� ���ش�.
		if (this->notepadForm->autoLineBreaker != 0) {
			delete this->notepadForm->autoLineBreaker;
			this->notepadForm->autoLineBreaker = 0;
		}
		
	}

	//�޸��� �����찡 ���û��¶�� �Ϲ� ���·� �����Ѵ�.
	if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {
		this->notepadForm->ChangeState(GeneralState::Instance());
	}

	// Ŀ���� �޸����� ó����ġ�� �ű��.
	Long index = this->notepadForm->note->Move(0);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Move(0);
	// ��ũ�� ��Ʈ�ѷ��� �����
	
	this->notepadForm->scrollController = new ScrollController(this->notepadForm);
	
	// �޸����� ������Ʈ�Ѵ�.
	this->notepadForm->Notify();
}

AutoLineBreakCommand& AutoLineBreakCommand::operator=(const AutoLineBreakCommand& source){
	Command::operator=(source);
	return *this;
}


