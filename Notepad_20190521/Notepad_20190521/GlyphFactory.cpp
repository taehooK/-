//GlyphFactory.cpp

#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "Character.h"
#include "DummyLine.h"
#include "Scanner.h"

GlyphFactory::GlyphFactory() {

}
GlyphFactory::GlyphFactory(const GlyphFactory& source) {

}
GlyphFactory::~GlyphFactory() {

}
Glyph* GlyphFactory::Make(const char(*content)) {
	Glyph* glyph = 0;
	if (content[0] == '\0') {
		glyph = new Note;
	}
	else if (content[0] == '\r' && content[1] == '\n') {
		glyph = new Line;
	}
	else if (content[0] == '\n' && content[1] == '\n' ) {
		glyph = new DummyLine;
	}
	else if(!(content[0] & 0x80)){
		glyph = new SingleByteCharacter(content[0]);
	}
	else if (content[0] & 0x80) {
		glyph = new DoubleByteCharacter(content);
	}
	return glyph;
}

Glyph* GlyphFactory::MakeLine(const char(*content)) {
	Glyph *line = new Line;
	Scanner scanner(content);
	

	while (!scanner.IsEnd()) {
		string token = scanner.GetToken();
		Glyph *character = 0;
		if (!(token[0] & 0x80)) {
			character = new SingleByteCharacter(token[0]);
		}
		else if (token[0] & 0x80) {
			character = new DoubleByteCharacter(token.c_str());
		}

		line->Add(character);

		scanner.Next();
	}
	
	return line;
}