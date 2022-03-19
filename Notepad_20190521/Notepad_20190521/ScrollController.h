#ifndef _SCROLLCONTROLLER
#define _SCROLLCONTROLLER

#include "Observer.h"

#define SCROLLCOUNT 2

class NotepadForm;
typedef signed long int Long;

class Scroll;
class ScrollController : public Observer {
public:
	ScrollController(NotepadForm *notepadForm = 0);
	ScrollController(const ScrollController& source);
	~ScrollController();
	virtual void Update();
	virtual Observer* GetClone();
	Long Left();
	Long Right();
	Long PageLeft();
	Long PageRight();
	Long Up();
	Long Down();
	Long PageUp();
	Long PageDown();
	Long Rotate(short delta);
	Long MoveHorizontalScroll(Long position);
	Long MoveVerticalScroll(Long position);
	void SmartScrollToPoint(Long x, Long y);

	Long GetNoteWidth() const;
	Long GetNoteHeight() const;
	Scroll* GetHorizontalScroll() const;
	Scroll* GetVerticalScroll() const;
private:
	NotepadForm *notepadForm;
	Scroll* scrolls[SCROLLCOUNT]; // ù ��°�� HorizontalScroll, �� ��°�� VerticalScroll��
};

inline Scroll* ScrollController::GetHorizontalScroll() const {
	return const_cast<Scroll*>(this->scrolls[0]);
}
inline Scroll* ScrollController::GetVerticalScroll() const {
	return const_cast<Scroll*>(this->scrolls[1]);
}

#endif //_SCROLLCONTROLLER