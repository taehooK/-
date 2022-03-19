#ifndef _NOTE_H
#define _NOTE_H

#include "Composite.h"
#include <string>

typedef signed long int Long;

using namespace std;
class Glyph;
class Note : public Composite {
public:
	Note(Long capacity = 250);
	Note(const Note& source);
	~Note();
	virtual Long Add(Glyph *glyph);
	virtual Long Add(Long index, Glyph *glyph);
	virtual Long Remove(Long index);
	virtual Long Next();
	virtual Long Last();
	virtual Long MovePreviousWord();
	virtual Long MoveNextWord();
	
	Note& operator =(const Note& source);
	virtual string GetContent();
	virtual string GetContent(Point startPoint, Point endPoint);
	virtual Glyph* Clone();
};


#endif //_NOTE_H