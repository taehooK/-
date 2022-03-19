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
		//��Ʈ ��ȭ ���ڿ��� ��Ʈ ������ �����´�.
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
		//�ڵ����� �����̸� �ڵ������ڿ��� �ڵ������Ѵ�.
		// �ڵ� ��������̸� �ڵ������ڿ��� �ڵ�����
		if (this->notepadForm->menu.GetMenuState(IDM_AUTO_LINEBREAK, MF_BYCOMMAND) == MF_CHECKED) {
			this->notepadForm->autoLineBreaker->UpdateAll();
		}
		//��ũ�� ��Ʈ�ѷ��� �ٽ� �����.
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
