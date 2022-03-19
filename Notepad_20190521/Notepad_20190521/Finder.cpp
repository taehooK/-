#include "Finder.h"
#include "Glyph.h"
#include "Character.h"
#include "GlyphFactory.h"
#include "Scanner.h"
#include "Line.h"
#include "Note.h"
#include <iostream>

Finder::Finder() {
}

Finder::Finder(const Finder& source) {
}

Finder::~Finder() {
}

Long Finder::Find(Line* pattern, Line* line, Long indexOfStart, bool isCaseSensitive) {
	Long indexOfFinded = -1;
	Long *next = new Long[pattern->GetLength()];
	Long i = 0;
	Long j = -1;

	// next배열을 구한다.
	while (i < pattern->GetLength()) {

		if (j != -1 && this->isSameCharacter(pattern->GetAt(i), pattern->GetAt(j), isCaseSensitive)) {
			next[i] = next[j];
		}
		else {
			next[i] = j;
		}

		while (j >= 0 && !(this->isSameCharacter(pattern->GetAt(i), pattern->GetAt(j), isCaseSensitive))) {
			j = next[j];
		}

		j++;
		i++;
	}

	// KMP 알고리즘 방식으로 탐색
	i = indexOfStart;
	j = 0;
	while (i < line->GetLength() && j < pattern->GetLength()) {

		while (j >= 0 && !(this->isSameCharacter(line->GetAt(i), pattern->GetAt(j), isCaseSensitive))) {
			j = next[j];
		}
		j++;
		i++;
	}

	if (j == pattern->GetLength()) {
		indexOfFinded = i - pattern->GetLength();
	}

	return indexOfFinded;
}


Point Finder::Find(Line* pattern, Note* note, Point startPosition, bool isCaseSensitive) {
	Point result = { -1, -1 };
	Long ret;

	Glyph *line = note->GetAt(startPosition.y);
	ret = this->Find(dynamic_cast<Line*>(pattern), dynamic_cast<Line*>(line), startPosition.x, isCaseSensitive);

	Long i = startPosition.y + 1;
	while (i < note->GetLength() && ret == -1) {
		line = note->GetAt(i);
		ret = this->Find(dynamic_cast<Line*>(pattern), dynamic_cast<Line*>(line), 0, isCaseSensitive);
		i++;
	}

	result.x = ret;
	result.y = i - 1;

	return result;
}



Long Finder::FindBackwards(Line* pattern, Line* line, Long indexOfStart, bool isCaseSensitive) {
	Long indexOfFinded = -1;
	Long *next = new Long[pattern->GetLength()];
	Long i = 0;
	Long j = -1;


	// next배열을 구한다.
	while (i < pattern->GetLength()) {

		if (j != -1 && this->isSameCharacter(pattern->GetAt(pattern->GetLength() - 1 - i), pattern->GetAt(pattern->GetLength() - 1 - j), isCaseSensitive)) {
			next[i] = next[j];
		}
		else {
			next[i] = j;
		}

		while (j >= 0 && !(this->isSameCharacter(pattern->GetAt(pattern->GetLength() - 1 - i), pattern->GetAt(pattern->GetLength() - 1 - j), isCaseSensitive))) {
			j = next[j];
		}

		j++;
		i++;
	}

	// KMP 알고리즘 방식으로 탐색
	i = line->GetLength() - 1 - indexOfStart;
	j = 0;
	while (i < line->GetLength() && j < pattern->GetLength()) {

		while (j >= 0 && !(this->isSameCharacter(line->GetAt(line->GetLength() - 1 - i), pattern->GetAt(pattern->GetLength() - 1 - j), isCaseSensitive))) {
			j = next[j];
		}
		j++;
		i++;
	}

	if (j == pattern->GetLength()) {
		indexOfFinded = line->GetLength() - i;
	}

	return indexOfFinded;

}

Point Finder::FindBackwards(Line* pattern, Note* note, Point startPosition, bool isCaseSensitive) {
	Point result = { -1, -1 };
	Long ret;

	Glyph *line = note->GetAt(startPosition.y);
	ret = this->FindBackwards(dynamic_cast<Line*>(pattern), dynamic_cast<Line*>(line), startPosition.x, isCaseSensitive);

	Long i = startPosition.y - 1;
	while (i > -1 && ret == -1) {
		line = note->GetAt(i);
		ret = this->FindBackwards(dynamic_cast<Line*>(pattern), dynamic_cast<Line*>(line), line->GetLength() - 1, isCaseSensitive);
		i--;
	}

	result.x = ret;
	result.y = i + 1;

	return result;
}

bool Finder::isSameCharacter(Glyph *one, Glyph *other, bool isCaseSensitive) {
	bool ret = false;

	if (dynamic_cast<SingleByteCharacter*>(one) && dynamic_cast<SingleByteCharacter*>(other)) {
		string contentOne = one->GetContent();

		string contentOther = other->GetContent();
		if (isCaseSensitive == false) {
			if (contentOne[0] >= 0x61 && contentOne[0] <= 0x7A) {
				contentOne[0] = contentOne[0] - 0x20;
			}

			if (contentOther[0] >= 0x61 && contentOther[0] <= 0x7A) {
				contentOther[0] = contentOther[0] - 0x20;
			}
		}

		if (contentOne[0] == contentOther[0]) {
			ret = true;
		}

	}
	else if (dynamic_cast<DoubleByteCharacter*>(one) && dynamic_cast<DoubleByteCharacter*>(other)) {
		ret = dynamic_cast<DoubleByteCharacter*>(one)->IsEqual(*dynamic_cast<DoubleByteCharacter*>(other));
	}



	return ret;
}

Finder& Finder::operator=(const Finder& source) {
	return *this;
}

