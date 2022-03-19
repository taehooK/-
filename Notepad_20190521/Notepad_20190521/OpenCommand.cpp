#include "OpenCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include "Line.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "Scanner.h"
#include "ScrollController.h"
#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>
#include "resource.h"
#include "AutoLineBreaker.h"

OpenCommand::OpenCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

OpenCommand::OpenCommand(const OpenCommand& source)
	: Command(source.notepadForm) {
}

OpenCommand::~OpenCommand() {
}

void OpenCommand::Execute() {
	GlyphFactory glyphFactory;
	int message = 0;
	static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.txt)|*.txt|")
		_T("모든 파일 (*.*)|*.*||");

	if (this->notepadForm->isSaved == FALSE) {
		CString messageText;
		CString fileName = this->notepadForm->fileName;
		if (this->notepadForm->fileName.Compare("") == 0) {
			fileName = "제목 없음";
		}
		
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", (LPCTSTR)fileName);
		message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		
		
		if (message == IDYES) {
			if (this->notepadForm->fileName.Compare("") == 0) {
				CFileDialog fileDialog(FALSE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();
					File file((LPCTSTR)pathName, ios_base::out);
					string content = this->notepadForm->note->GetContent();
					file.Save(content);
					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
					this->notepadForm->fileName = pathName;
				}
			}
			else {
				File file((LPCTSTR)this->notepadForm->fileName, ios_base::out);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}
			this->notepadForm->isSaved = TRUE;
		}
	}

	if (message != IDCANCEL) {
		CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			File file((LPCTSTR)pathName, ios_base::in);
			string content = file.Load();
			if (this->notepadForm->note != NULL) {
				delete this->notepadForm->note;
			}
			this->notepadForm->note = glyphFactory.Make("");
			this->notepadForm->current = glyphFactory.Make("\r\n");
			this->notepadForm->note->Add(this->notepadForm->current);
			Scanner scanner(content);
			while (scanner.IsEnd() == FALSE) {
				string token = scanner.GetToken();
				Glyph *glyph = glyphFactory.Make(token.c_str());
				if (token != "\r\n") {
					this->notepadForm->current->Add(glyph);
				}
				else {
					Long index = this->notepadForm->note->Add(glyph);
					this->notepadForm->current = this->notepadForm->note->GetAt(index);
				}
				scanner.Next();
			}
			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);
			this->notepadForm->fileName = pathName;
			this->notepadForm->isSaved = TRUE;

			Long index = this->notepadForm->note->First();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();

			// 스크롤 컨트롤러 재생성
			if (this->notepadForm->scrollController != NULL) {
				delete this->notepadForm->scrollController;
				this->notepadForm->scrollController = new ScrollController(this->notepadForm);
			}
			// 자동 개행상태이면 자동개행자에서 자동개행
			if (this->notepadForm->menu.GetMenuState(IDM_AUTO_LINEBREAK, MF_BYCOMMAND) == MF_CHECKED) {
				this->notepadForm->autoLineBreaker->UpdateAll();
			}

			this->notepadForm->Notify();
		}
	}
}

OpenCommand& OpenCommand::operator =(const OpenCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}