#ifndef _SELECTEDAREAHANDLER_H
#define _SELECTEDAREAHANDLER_H

#include "Glyph.h"


class NotepadForm;
class SelectedAreaHandler {
public:
	SelectedAreaHandler(NotepadForm *notepadForm = 0);
	SelectedAreaHandler(const SelectedAreaHandler& source);
	~SelectedAreaHandler();
	void Select(Point startPoint, Point endPoint);
	void Remove();
	void Cut();
	void Copy();
	SelectedAreaHandler& operator=(const SelectedAreaHandler& other);
	
	Point GetStartPoint();
	Point GetEndPoint();

private:
	NotepadForm *notepadForm;
	Point startPoint;
	Point endPoint;
};


#endif // !_SELECTEDAREAHANDLER_H
