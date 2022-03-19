#include "SaveAsCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include <afxdlgs.h>

SaveAsCommand::SaveAsCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

SaveAsCommand::SaveAsCommand(const SaveAsCommand& source)
	: Command(source.notepadForm) {
}

SaveAsCommand::~SaveAsCommand() {
}

void SaveAsCommand::Execute() {
	static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.txt)|*.txt|")
		_T("모든 파일 (*.*)|*.*||");
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
		this->notepadForm->isSaved = TRUE;
	}
}
SaveAsCommand& SaveAsCommand::operator =(const SaveAsCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}