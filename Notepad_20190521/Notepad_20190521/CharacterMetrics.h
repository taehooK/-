//CharacterMetrics.h
#ifndef CHARACTERMATRICS_H
#define CHARACTERMATRICS_H

#include <string>
#include <afxwin.h>

using namespace std;
typedef signed long int Long;
class NotepadForm;
class Glyph;

class CharacterMetrics {
public:
	CharacterMetrics(NotepadForm *notepadForm = 0);
	CharacterMetrics(const CharacterMetrics& source);
	~CharacterMetrics();
	Long GetWidth(Glyph *line);
	Long GetWidth(Glyph *line, Long index);
	Long GetWidth(const string& buffer);
	Long GetHeight(Long index);
	CharacterMetrics& operator=(const CharacterMetrics& source);
	Long GetColumn(Glyph *line, Long x);
	Long GetColumnBeforeX(Glyph *line, Long x);
	Long GetRow(Long y);
	Long GetNoteWidth(Glyph *note);
	Long GetWidthAverage();

	Long GetWidth(Long index) const;
	Long GetDoubleByteWidth() const;
	Long GetHeight() const;
private:
	NotepadForm *notepadForm;
	Long widths[129];
	Long height;
	TEXTMETRICA textmetrica;
};

inline Long CharacterMetrics::GetWidth(Long index) const {
	return this->widths[index];
}

inline Long CharacterMetrics::GetDoubleByteWidth() const {
	return this->widths[128];
}

inline Long CharacterMetrics::GetHeight() const {
	return height;
}

#endif // !CHARACTERMATRICS_H
