#ifndef _SCROLLACTIONFACTORY_H
#define _SCROLLACTIONFACTORY_H
#include <afxwin.h>

class ScrollAction;
class NotepadForm;
class ScrollActionFactory {
public:
	ScrollActionFactory(NotepadForm *notepadForm = 0);
	ScrollActionFactory(const ScrollActionFactory& source);
	~ScrollActionFactory();
	ScrollAction* MakeHScrollAction(UINT nSBCode);
	ScrollAction* MakeVScrollAction(UINT nSBCode);
	ScrollActionFactory& operator=(const ScrollActionFactory& source);
private:
	NotepadForm *notepadForm;
};

#endif 