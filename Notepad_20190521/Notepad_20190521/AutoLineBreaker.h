#ifndef _AUTOLINEBREAKER_H
#define _AUTOLINEBREAKER_H
#include "Glyph.h"
#include "Observer.h"

typedef signed long int Long;

class NotepadForm;

class AutoLineBreaker : public Observer {
public:
	AutoLineBreaker(NotepadForm *notepadForm = 0);
	AutoLineBreaker(const AutoLineBreaker& source);
	~AutoLineBreaker();
	virtual void Update();
	void Update(Long lineNumber);
	void UpdateAll();
	void Cancel();
	void Compact(Long lineNumber);
	Point CovPosToAutoLineBreak(Point posNonAutoLineBreak);
	Point CovPosToNonAutoLineBreak(Point posAutoLineBreak);
	virtual Observer* GetClone();

	AutoLineBreaker& operator=(const AutoLineBreaker& source);
private:
	NotepadForm *notepadForm;
};

#endif // !_AUTOLINEBREAKER_H
