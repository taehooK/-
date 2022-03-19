#include "SaveCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include <afxdlgs.h>

SaveCommand::SaveCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

SaveCommand::SaveCommand(const SaveCommand& source)
	: Command(source.notepadForm) {
}

SaveCommand::~SaveCommand() {
}

void SaveCommand::Execute() {
	int ret = 0;
	if (this->notepadForm->fileName.Compare("") == 0) {
		static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.txt)|*.txt|")
			_T("모든 파일 (*.*)|*.*||");
		CFileDialog fileDialog(FALSE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		ret = fileDialog.DoModal();
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

	if (ret != IDCANCEL) {
		this->notepadForm->isSaved = TRUE;
	}

}

SaveCommand& SaveCommand::operator =(const SaveCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}