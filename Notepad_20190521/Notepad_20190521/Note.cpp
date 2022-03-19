#include "Note.h"
#include "DummyLine.h"
#include "Glyph.h"

Note::Note(Long capacity)
	: Composite(capacity) {
}

Note::Note(const Note& source)
	: Composite(source) {
}

Note::~Note() {
}

Note& Note::operator =(const Note& source) {
	Composite::operator=(source);
	return *this;
}

Long Note::Add(Glyph *glyph) {
	Long index = Composite::Add(glyph);
	this->current = index;

	return index;
}

Long Note::Add(Long index, Glyph *glyph) {
	Composite::Add(index, glyph);
	this->current = index;
	
	return this->current;
}

Long Note::Remove(Long index) {
	Composite::Remove(index);
	this->current = index - 1;

	return -1;
}

Long Note::Next() {
	this->current++;
	if (this->current >= this->length) {
		this->current = this->length - 1;
	}
	return this->current;
}

Long Note::Last() {
	this->current = this->length - 1;
	return this->current;
}

Long Note::MovePreviousWord() {
	Glyph *line;
	line = this->glyphs.GetAt(this->current);
	if (line->GetCurrent() >= 1) {
		line->MovePreviousWord();
	}
	// ���ο��� ù��° ��ġ�� ��� ���� �ٷ� �̵��Ѵ�.
	else if (line->GetCurrent() < 1 && this->current > 0) { 
		this->current--;
		line = this->glyphs.GetAt(this->current);
		line->Last();
	}
	return this->current;
}

Long Note::MoveNextWord() {
	Glyph *line;
	line = this->glyphs.GetAt(this->current);

	if (line->GetCurrent() < line->GetLength()) {
		line->MoveNextWord();
	}
	// ���ο��� ������ ��ġ�� ��� ���� �ٷ� �̵��Ѵ�.
	else if (line->GetCurrent() >= line->GetLength() &&
		this->current < this->length-1) {
		this->current++;
		line = this->glyphs.GetAt(this->current);
		line->First();
	}

	return this->current;
}

string Note::GetContent() {
	Long i = 0;
	string content("");
	string characterString;
	Glyph *line;
	Glyph *character;
	Long j = 0;
	while (i < this->length) {
	line = this->glyphs.GetAt(i);
	j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			characterString = character->GetContent();
			if (characterString.length() == 8) {// �ǹ��� ó��
				characterString = '\t';
			}
			content += characterString;
			j++;
		}
		if (i + 1 < this->length) {
			line = this->glyphs.GetAt(i + 1);
			if (!dynamic_cast<DummyLine*>(line)) {
				content += "\n";
			}
		}
		i++;
	}
	return content;

	/*
		string content = "";

		Long i = 0;
		while (i < this->length) {
			content += this->glyphs[i]->GetContent();
			i++;
		}

		return content;
		*/
}

string Note::GetContent(Point startPoint, Point endPoint) {
	string ret = "";
	string content;
	Long i ;
	Long j;
	Glyph *line;
	Glyph *character;

	Point start = startPoint;
	Point end = endPoint;

	if (startPoint.y > endPoint.y || (startPoint.y == endPoint.y && startPoint.x > endPoint.x)) { // startPoint�� endPoint���� �ڿ� �ִ� ���
		start = endPoint;
		end = startPoint;
	}


	if (start.y == end.y) { // ���� ���� ó��
		line = this->glyphs.GetAt(startPoint.y);
		i = start.x;
		while(i < end.x){
			character = line->GetAt(i);
			content = character->GetContent();
			ret = ret + content;

			i++;
		}
	}
	else if (start.y < end.y) {
		i = start.y;
		while (i <= end.y) {
			line = this->glyphs.GetAt(i);

			if (i == start.y) { // ù�� ° ���� ���
				j = start.x;
				while (j < line->GetLength()) {
					character = line->GetAt(j);
					content = character->GetContent();
					ret = ret + content;
					j++;
				}
			}
			else if (i == end.y) { // ������ ���� ���
				j = 0;
				while (j < end.x) {
					character = line->GetAt(j);
					content = character->GetContent();
					ret = ret + content;
					j++;
				}
			}
			else { // ���̿� ���� ���
				content = line->GetContent();
				ret = ret + content;
			}

			if (i < end.y) {
				line = this->glyphs.GetAt(i + 1);
				if (!dynamic_cast<DummyLine*>(line)) {
					ret = ret + "\r\n";
				}
			}

			i++;
		}
	}

	
	return ret;
}

Glyph* Note::Clone() {
	return new Note(*this);
}

