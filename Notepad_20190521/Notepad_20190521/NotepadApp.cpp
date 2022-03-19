//NotepadApp.cpp

#include "NotepadApp.h"
#include "NotePadForm.h"

BOOL NotepadApp::InitInstance() {
	NotepadForm *notepadForm = new NotepadForm;
	notepadForm->Create(NULL, "제목없음 - 메모장");
	notepadForm->ShowWindow(SW_NORMAL);
	notepadForm->UpdateWindow();
	this->m_pMainWnd = notepadForm;

	return TRUE;

}
NotepadApp notepadApp;