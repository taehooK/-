#ifndef _CARETCONTROLLER_H
#define _CARETCONTROLLER_H

#include "Observer.h"

typedef signed long int Long;
class NotepadForm;
class Caret;

class CaretController : public Observer {
public:
	CaretController(NotepadForm *notepadForm = 0);
	CaretController(const CaretController& source);
	~CaretController();
	virtual void Update();
	virtual Observer* GetClone();
	Long GetCaretX();
	Long GetCaretY();
	CaretController& operator =(const CaretController& source);
private:
	NotepadForm *notepadForm;
	Caret *caret;
};

#endif //_CARETCONTROLLER_H