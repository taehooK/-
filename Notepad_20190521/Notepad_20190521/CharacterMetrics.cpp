#include "CharacterMetrics.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Character.h"
#include "Font.h"

CharacterMetrics::CharacterMetrics(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
	CDC *dc = this->notepadForm->GetDC();
	CString buffer;
	CSize size;
	CFont *oldFont;
	CFont font;
	this->notepadForm->font->Create(font);
	oldFont = dc->SelectObject(&font);

	Long i = 1;
	while (i < 128) {
		buffer.Format("%c", i);
		size = dc->GetTextExtent(buffer);
		this->widths[i] = size.cx;
		i++;
	}
	this->widths[9] = this->widths[32] * 8; //Tab 처리
	this->widths[0] = this->widths[32];
	size = dc->GetTextExtent("가");
	this->widths[128] = size.cx;
	memset(&this->textmetrica, 0, sizeof(this->textmetrica));
	dc->GetTextMetrics(&this->textmetrica);
	this->height = this->textmetrica.tmHeight;
	dc->SelectObject(oldFont);
}
CharacterMetrics::CharacterMetrics(const CharacterMetrics& source) {
	this->notepadForm = source.notepadForm;

	Long i = 0;
	while (i < 129) {
		this->widths[i] = source.widths[i];
		i++;
	}

	this->height = source.height;
	this->textmetrica = source.textmetrica;
}

CharacterMetrics::~CharacterMetrics() {

}

/******
줄을 입력받아서 줄너비를 출력
*******/

Long CharacterMetrics::GetWidth(Glyph *line) {
	Long x = 0;

	Long i = 0;
	while (i < line->GetLength()) {
		Glyph *character = line->GetAt(i);

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			string content = character->GetContent(); // \t
			if (content.length() == 1) { // Tab이 아닌 경우
				x += this->widths[content[0]];
			}
			else {
				x += this->widths[9]; //Tab 문자 처리
			}
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			x += this->widths[128];
		}
		i++;
	}

	return x;
}

/*************************************************************
줄과 인덱스를 입력받아 줄에서 해당 인덱스 글자 위치 까지에 너비를 반환
**************************************************************/

Long CharacterMetrics::GetWidth(Glyph *line, Long index) {
	Long x = 0;

	Long i = 0;
	while (i < index) {
		Glyph *character = line->GetAt(i);

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			string content = character->GetContent();
			if (content.length() == 1) { // Tab이 아닌 경우
				x += this->widths[content[0]];
			}
			else {
				x += this->widths[9]; //Tab
			}
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			x += this->widths[128];
		}
		i++;
	}
	return x;
}

/**********************************************************
string을 입력받아 string의 길이를 반환
***********************************************************/

Long CharacterMetrics::GetWidth(const string& buffer) {
	Long x = 0;

	Long i = 0;
	while (i < buffer.length()) {
		if (!(buffer[i] & 0x80)) { // 영문자, 숫자, 기호인 경우
			if (buffer[i] != '\t') {
				x += this->widths[buffer[i]];
			}
			else {
				x += this->widths[9]; //Tab
			}
			i++;
		}
		else if (buffer[i] & 0x80) { // 한글인 경우
			x += this->widths[128];
			i += 2;
		}
	}

	return x;
}

Long CharacterMetrics::GetHeight(Long index) {
	return this->height * index;
}

Long CharacterMetrics::GetColumn(Glyph *line, Long x) {
	Long index = 0;
	Long currentX = 0;
	Long previousX = -1;
	Glyph *character;
	string content;
	Long asciiIndex;
	Long width;

	while (index < line->GetLength() && currentX < x) {
		character = line->GetAt(index);
		content = character->GetContent();

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			if (content[1] == '\0') { // 탭이 아닐 때
				asciiIndex = content[0];
			}
			else {
				asciiIndex = 9;
			}
			width = this->widths[asciiIndex];
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			width = this->widths[128]; // 한글
		}
		previousX = currentX;
		currentX += width;
		index++;
	}
	
	if ((x - previousX) < (currentX - x)) { // 가장 가까운 위치로 이동
		index--;
	}

	return index;
}


Long CharacterMetrics::GetColumnBeforeX(Glyph *line, Long x) {
	Long index = 0;
	Long currentX = 0;
	Long previousX = -1;
	Glyph *character;
	string content;
	Long asciiIndex;
	Long width;

	while (index < line->GetLength() && currentX < x) {
		character = line->GetAt(index);
		content = character->GetContent();

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			if (content[1] == '\0') { // 탭이 아닐 때
				asciiIndex = content[0];
			}
			else {
				asciiIndex = 9;
			}
			width = this->widths[asciiIndex];
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			width = this->widths[128]; // 한글
		}
		previousX = currentX;
		currentX += width;
		if (currentX < x) {
			index++;
		}
	}

	return index;
}

Long CharacterMetrics::GetRow(Long y) {
	return y / this->height;
}

Long CharacterMetrics::GetNoteWidth(Glyph *note) {
	Long i = 0;
	Long j;
	Glyph *line;
	Glyph *character;
	Long lineWidth;
	Long noteWidth = 0;
	string content;

	while (i < note->GetLength()) {
		line = note->GetAt(i);
		lineWidth = 0;
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			content = character->GetContent();

			if (dynamic_cast<SingleByteCharacter*> (character)) {
				if (content.length() == 1) {
					lineWidth += this->widths[content[0]];
				}
				else {
					lineWidth += this->widths[9];
				}
			}
			else if (dynamic_cast<DoubleByteCharacter*>(character)) {
				lineWidth += this->widths[128];
			}
			j++;
		}
		if (noteWidth < lineWidth) {
			noteWidth = lineWidth;
		}
		i++;
	}

	return noteWidth;
}

Long CharacterMetrics::GetWidthAverage() {
	return this->textmetrica.tmAveCharWidth;
}

CharacterMetrics& CharacterMetrics::operator =(const CharacterMetrics& source) {
	this->notepadForm = source.notepadForm;

	Long i = 0;
	while (i < 129) {
		this->widths[i] = source.widths[i];
		i++;
	}

	this->height = source.height;
	this->textmetrica = source.textmetrica;

	return *this;
}
