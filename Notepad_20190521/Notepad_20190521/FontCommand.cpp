//FontCommand.cpp
#include "FontCommand.h"
#include "NotepadForm.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "AutoLineBreaker.h"
#include "resource.h"
#include <afxdlgs.h>


FontCommand::FontCommand(NotepadForm *notepadForm)
:Command(notepadForm){
}

FontCommand::FontCommand(const FontCommand& source) 
:Command(source.notepadForm){
}

FontCommand::~FontCommand() {

}

void FontCommand::Execute() {
	LOGFONT logFont = this->notepadForm->font->GetFont();
	COLORREF color;
	CFontDialog fontDialog(&logFont);
	int ret = fontDialog.DoModal();
	if (ret == IDOK) {
		//폰트 대화 상자에서 폰트 정보를 가져온다.
		fontDialog.GetCurrentFont(&logFont);
		color = fontDialog.GetColor();
		if (this->notepadForm->font != NULL) {
			delete this->notepadForm->font;
		}
		this->notepadForm->font = new Font(logFont, color, this->notepadForm);
		if (this->notepadForm->characterMetrics != NULL) {
			delete this->notepadForm->characterMetrics;
		}
		this->notepadForm->characterMetrics = new CharacterMetrics(this->notepadForm);
		//자동개행 상태이면 자동개행자에서 자동개행한다.
		// 자동 개행상태이면 자동개행자에서 자동개행
		if (this->notepadForm->menu.GetMenuState(IDM_AUTO_LINEBREAK, MF_BYCOMMAND) == MF_CHECKED) {
			this->notepadForm->autoLineBreaker->UpdateAll();
		}
		//스크롤 컨트롤러를 다시 만든다.
		if (this->notepadForm->scrollController != 0) {
			delete this->notepadForm->scrollController;
			this->notepadForm->scrollController = new ScrollController(this->notepadForm);
		}
		this->notepadForm->Notify();
		this->notepadForm->Invalidate();
	}
}

FontCommand& FontCommand::operator =(const FontCommand& source) {
	Command::operator=(source);
	return *this;
}
