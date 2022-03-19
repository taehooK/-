#include "Glyph.h"

Glyph::Glyph() {

}

Glyph::Glyph(const Glyph& source) {

}

Glyph::~Glyph() {

}

Long Glyph::Add(Glyph *glyph) {
	return -1;
}

Long Glyph::Add(Long index, Glyph *glyph) {
	return -1;
}

Glyph* Glyph::Divide(Long index) {
	return 0;
}

Glyph* Glyph::Combine(Glyph *other) {
	return 0;
}


Glyph* Glyph::GetAt(Long index) {
	return 0;
}

Long Glyph::Remove(Long index) {
	return -1;
}

Long Glyph::Remove(Long index, Long count) {
	return -1;
}

Long Glyph::GetCurrent() {
	return -1;
}

Long Glyph::GetLength() {
	return -1;
}

Long Glyph::GetCapcity() {
	return -1;
}

Long Glyph::First() {
	return -1;
}

Long Glyph::Last() {
	return -1;
}

Long Glyph::Previous() {
	return -1;
}

Long Glyph::Next() {
	return -1;
}

Long Glyph::Move(Long index) {
	return -1;
}

Long Glyph::MovePreviousWord() {
	return -1;
}

Long Glyph::MoveNextWord() {
	return -1;
}

string Glyph::GetContent(Point startPoint, Point endPoint) {
	return 0;
}


#if 0
#include "Composite.h"
#include "Note.h"
#include "Line.h"
#include "GlyphFactory.h"
#include "Character.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
	GlyphFactory glyphFactory;

	Glyph *note = glyphFactory.Make("");

	Glyph *line = glyphFactory.Make("\r\n");
	note->Add(line);

	Glyph *character = glyphFactory.Make("a");
	line->Add(character);

	character = glyphFactory.Make("한");
	line->Add(character);

	character = glyphFactory.Make(" ");
	line->Add(character);

	character = glyphFactory.Make("김");
	line->Add(character);

	line = glyphFactory.Make("\r\n");
	note->Add(line);

	character = glyphFactory.Make("1");
	line->Add(character);

	character = glyphFactory.Make("b");
	line->Add(character);

	Long j;
	Long i = 0;
	while (i < note->GetLength()) {
		line = note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			cout << character->GetContent();
			j++;
		}
		cout << endl;
		i++;
	}
	


	Long index = note->First();
	line = note->GetAt(index);
	string content = line->GetContent();
	cout << content << endl;

	index = line->First();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Previous();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = note->Previous();
	line = note->GetAt(index);
	content = line->GetContent();
	cout << content << endl;

	index = line->MoveNextWord();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Next();
	cout << index << endl;

	index = line->Next();
	cout << index << endl;

	index = note->Next();
	line = note->GetAt(index);
	content = line->GetContent();
	cout << content << endl;

	index = line->First();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Next();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	Long current = line->GetCurrent();
	index = note->Previous();
	line = note->GetAt(index);

	index = line->Move(current);
	character = line->GetAt(index);
	cout << character->GetContent() << endl;

	//19
	index = line->Last();
	cout << index << endl;
	cout << "줄의 길이" << line->GetLength() << endl;

	//20
	index = note->MoveNextWord();
	line = note->GetAt(index);
	index = line->GetCurrent();
	character = line->GetAt(index);
	cout << character->GetContent() << endl;

	//21
	index = line->MoveNextWord();
	cout << line->GetCurrent() << endl;

	//22
	index = line->First();
	character = line->GetAt(index);
	cout << character->GetContent() << endl;

	//23
	index = note->MovePreviousWord();
	line = note->GetAt(index);
	cout << line->GetCurrent() << endl; // 4가 나와야 정상

	//24
	index = line->MovePreviousWord();
	character = line->GetAt(index);
	cout << character->GetContent() << endl;

	//25
	index = note->Move(1);
	line = note->GetAt(index);
	cout << line->GetContent() << endl;

	//26
	index = line->Move(0);
	character = line->GetAt(index);
	cout << character->GetContent() << endl;


	if (note != 0) {
		delete note;
	}

	return 0;
}

#endif