#ifndef _SCROLLACTIONS_H
#define _SCROLLACTIONS_H

#include <afxwin.h>
typedef signed long int Long;
class NotepadForm;

class ScrollAction {
public:
	ScrollAction(NotepadForm *notepadForm = 0);
	ScrollAction(const ScrollAction& source);
	virtual ~ScrollAction() = 0;
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) = 0;
	ScrollAction& operator=(const ScrollAction& source);
protected:
	NotepadForm *notepadForm;
};


class LineUpAction : public ScrollAction {
public:
	LineUpAction(NotepadForm *notepadForm = 0);
	LineUpAction(const LineUpAction& source);
	virtual ~LineUpAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	LineUpAction& operator=(const LineUpAction& source);
};

class LineDownAction : public ScrollAction {
public:
	LineDownAction(NotepadForm *notepadForm = 0);
	LineDownAction(const LineDownAction& source);
	virtual ~LineDownAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	LineDownAction& operator=(const LineDownAction& source);
};

class PageUpAction : public ScrollAction {
public:
	PageUpAction(NotepadForm *notepadForm = 0);
	PageUpAction(const PageUpAction& source);
	virtual ~PageUpAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	PageUpAction& operator=(const PageUpAction& source);
};

class PageDownAction : public ScrollAction {
public:
	PageDownAction(NotepadForm *notepadForm = 0);
	PageDownAction(const PageDownAction& source);
	virtual ~PageDownAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	PageDownAction& operator=(const PageDownAction& source);
};

class ThumbTrackVScrollAction : public ScrollAction {
public:
	ThumbTrackVScrollAction(NotepadForm *notepadForm = 0);
	ThumbTrackVScrollAction(const ThumbTrackVScrollAction& source);
	virtual ~ThumbTrackVScrollAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	ThumbTrackVScrollAction& operator=(const ThumbTrackVScrollAction& source);
};


class LineLeftAction : public ScrollAction {
public:
	LineLeftAction(NotepadForm *notepadForm = 0);
	LineLeftAction(const LineLeftAction& source);
	virtual ~LineLeftAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	LineLeftAction& operator=(const LineLeftAction& source);
};

class LineRightAction : public ScrollAction {
public:
	LineRightAction(NotepadForm *notepadForm = 0);
	LineRightAction(const LineRightAction& source);
	virtual ~LineRightAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	LineRightAction& operator=(const LineRightAction& source);
};

class PageLeftAction : public ScrollAction {
public:
	PageLeftAction(NotepadForm *notepadForm = 0);
	PageLeftAction(const PageLeftAction& source);
	virtual ~PageLeftAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	PageLeftAction& operator=(const PageLeftAction& source);
};

class PageRightAction : public ScrollAction {
public:
	PageRightAction(NotepadForm *notepadForm = 0);
	PageRightAction(const PageRightAction& source);
	virtual ~PageRightAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	PageRightAction& operator=(const PageRightAction& source);
};

class ThumbTrackHScrollAction : public ScrollAction {
public:
	ThumbTrackHScrollAction(NotepadForm *notepadForm = 0);
	ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source);
	virtual ~ThumbTrackHScrollAction();
	virtual void OnScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	ThumbTrackHScrollAction& operator=(const ThumbTrackHScrollAction& source);
};

#endif // !_SCROLLACTIONS_H
