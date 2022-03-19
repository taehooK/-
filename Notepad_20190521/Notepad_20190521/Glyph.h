//Glyph.h
#ifndef _GLYPH_H
#define _GLYPH_H
#include <string>
using namespace std;
typedef signed long int Long;
typedef struct point {
	Long x;
	Long y;
}Point;
class Glyph {
public:
	Glyph();
	Glyph(const Glyph& source);
	virtual ~Glyph() = 0;
	virtual Long Add(Glyph *glyph);
	virtual Long Add(Long index, Glyph *glyph);
	virtual Glyph* Divide(Long index);
	virtual Glyph* Combine(Glyph *other);
	virtual Glyph* GetAt(Long index);
	virtual string GetContent() = 0;
	virtual string GetContent(Point startPoint, Point endPoint);
	virtual Glyph* Clone() = 0;
	virtual Long Remove(Long index);
	virtual Long Remove(Long index, Long count);
	virtual Long GetCurrent();
	virtual Long GetLength();
	virtual Long GetCapcity();
	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Move(Long index);
	virtual Long MovePreviousWord();
	virtual Long MoveNextWord();
	
};
#endif // ! _GLYPH_H
