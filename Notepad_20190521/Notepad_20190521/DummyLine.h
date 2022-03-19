#ifndef _DUMMYLINE_H
#define _DUMMYLINE_H

#include "Line.h"

typedef signed long int Long;

class DummyLine : public Line {
public:
	DummyLine(Long capacity = 256);
	DummyLine(const DummyLine& source);
	DummyLine(Line& line);
	~DummyLine();
	DummyLine& operator=(const DummyLine& source);
};


#endif // !_DUMMYLINE_H
