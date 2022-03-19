#ifndef _FINDER_H
#define _FINDER_H
typedef signed long int Long;

#include "Glyph.h"

class Note;
class Line;
class Finder {
public:
	Finder();
	Finder(const Finder& source);
	~Finder();
	Long Find(Line* pattern, Line* line, Long indexOfStart, bool isCaseSensitive = true);
	Point Find(Line* pattern, Note* note, Point startPosition, bool isCaseSensitive = true);

	Long FindBackwards(Line* pattern, Line* line, Long indexOfStart, bool isCaseSensitive = true);
	Point FindBackwards(Line* pattern, Note* note, Point startPosition, bool isCaseSensitive = true);

	bool isSameCharacter(Glyph *one, Glyph *other, bool isCase = true);
	
	Finder& operator=(const Finder& source);
};





#endif // !_FINDER_H
