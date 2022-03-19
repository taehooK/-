#include "NewCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "File.h"
#include "GlyphFactory.h"
#include "CaretController.h"
#include "ScrollController.h"
#include <afxdlgs.h>


NewCommand::NewCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

NewCommand::NewCommand(const NewCommand& source)
	: Command(source.notepadForm) {
}

NewCommand::~NewCommand() {
}

void NewCommand::Execute() {
	GlyphFactory glyphFactory;
	int message = 0;
	if (this->notepadForm->isSaved == FALSE) {
		CString messageText;
		CString fileName = this->notepadForm->fileName;
		if (fileName.Compare("") == 0) {
			fileName.Format("제목 없음");
		}

		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", (LPCTSTR)fileName);
		message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (this->notepadForm->fileName.Compare("") == 0) { // 파일 이름이 정해지지 않은 경우
				static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.txt)|*.txt|")
					_T("모든 파일 (*.*)|*.*||");
				CFileDialog fileDialog(FALSE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);;
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
			else { // 파일 이름이 정해진 경우
				File file((LPCTSTR)this->notepadForm->fileName, ios_base::out);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}	
		}
	}
	if (message != IDCANCEL) { // 저장 유무에 대한 답이 취소가 아닌 경우 새로 만들기 실행
		if (this->notepadForm->note != NULL) {
			delete this->notepadForm->note;
		}
		this->notepadForm->note = glyphFactory.Make("");
		Glyph *line = glyphFactory.Make("\r\n");
		this->notepadForm->note->Add(line);
		this->notepadForm->SetWindowTextA("제목 없음 - 메모장");
		this->notepadForm->fileName = "";
		this->notepadForm->isSaved = TRUE;

		Long index = this->notepadForm->note->First();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		// 스크롤 컨트롤러 재생성
		if (this->notepadForm->scrollController != NULL) {
			delete this->notepadForm->scrollController;
			this->notepadForm->scrollController = new ScrollController(this->notepadForm);
		}
		// 스크롤 컨트롤러 재생성
		this->notepadForm->current->First();
		this->notepadForm->Notify();
	}
}

NewCommand& NewCommand::operator =(const NewCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}