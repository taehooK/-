//Composite.h

#ifndef _COMPOSITE_H
#define _COMPOSITE_H
#include "Array.h"
#include "Glyph.h"

typedef signed long int Long;

class Composite : public Glyph {
public:
	Composite(Long capacity = 256);
	Composite(const Composite& source);
	virtual ~Composite() = 0;
	virtual Long Add(Glyph *glyph);
	virtual Long Add(Long index, Glyph *glyph);
	virtual Glyph* GetAt(Long index);
	virtual Long Remove(Long index);
	virtual Long Remove(Long index, Long count);

	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Move(Long index);

	Composite& operator =(const Composite& source);
	Glyph* operator[] (Long index);
	virtual Long GetCapacity();
	virtual Long GetLength();
	virtual Long GetCurrent();
protected:
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
	Long current;
};


#endif // !_COMPOSITE_H
