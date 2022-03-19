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
		//자동개행자를 만든다.
		this->notepadForm->autoLineBreaker = new AutoLineBreaker(this->notepadForm);
		//자동개행자에서 모든 줄을 업데이트 한다.
		this->notepadForm->autoLineBreaker->UpdateAll();
		//자동개행 메뉴를 선택상태로 만든다.
		this->notepadForm->menu.CheckMenuItem(IDM_AUTO_LINEBREAK, MF_CHECKED);
	}
	else if (state == MF_CHECKED) { 
		this->notepadForm->autoLineBreaker->Cancel();

		//자동개행 메뉴를 비선택상태로 만든다.
		this->notepadForm->menu.CheckMenuItem(IDM_AUTO_LINEBREAK, MF_UNCHECKED);

		//자동개행자를 없앤다.
		if (this->notepadForm->autoLineBreaker != 0) {
			delete this->notepadForm->autoLineBreaker;
			this->notepadForm->autoLineBreaker = 0;
		}
		
	}

	//메모장 윈도우가 선택상태라면 일반 상태로 변경한다.
	if (dynamic_cast<SelectedState*>(this->notepadForm->GetNoteState())) {
		this->notepadForm->ChangeState(GeneralState::Instance());
	}

	// 커서를 메모장의 처음위치로 옮긴다.
	Long index = this->notepadForm->note->Move(0);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Move(0);
	// 스크롤 컨트롤러를 만들다
	
	this->notepadForm->scrollController = new ScrollController(this->notepadForm);
	
	// 메모장을 업데이트한다.
	this->notepadForm->Notify();
}

AutoLineBreakCommand& AutoLineBreakCommand::operator=(const AutoLineBreakCommand& source){
	Command::operator=(source);
	return *this;
}


