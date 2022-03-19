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
			fileName.Format("���� ����");
		}

		messageText.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", (LPCTSTR)fileName);
		message = MessageBox(NULL, (LPCTSTR)messageText, "�޸���", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (this->notepadForm->fileName.Compare("") == 0) { // ���� �̸��� �������� ���� ���
				static TCHAR BASED_CODE szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt|")
					_T("��� ���� (*.*)|*.*||");
				CFileDialog fileDialog(FALSE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);;
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();
					File file((LPCTSTR)pathName, ios_base::out);
					string content = this->notepadForm->note->GetContent();
					file.Save(content);
					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - �޸���");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
					this->notepadForm->fileName = pathName;
				}
			}
			else { // ���� �̸��� ������ ���
				File file((LPCTSTR)this->notepadForm->fileName, ios_base::out);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}	
		}
	}
	if (message != IDCANCEL) { // ���� ������ ���� ���� ��Ұ� �ƴ� ��� ���� ����� ����
		if (this->notepadForm->note != NULL) {
			delete this->notepadForm->note;
		}
		this->notepadForm->note = glyphFactory.Make("");
		Glyph *line = glyphFactory.Make("\r\n");
		this->notepadForm->note->Add(line);
		this->notepadForm->SetWindowTextA("���� ���� - �޸���");
		this->notepadForm->fileName = "";
		this->notepadForm->isSaved = TRUE;

		Long index = this->notepadForm->note->First();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		// ��ũ�� ��Ʈ�ѷ� �����
		if (this->notepadForm->scrollController != NULL) {
			delete this->notepadForm->scrollController;
			this->notepadForm->scrollController = new ScrollController(this->notepadForm);
		}
		// ��ũ�� ��Ʈ�ѷ� �����
		this->notepadForm->current->First();
		this->notepadForm->Notify();
	}
}

NewCommand& NewCommand::operator =(const NewCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}