//GlyphFactory.h

#ifndef _GLYPHFACTORY_H
#define _GLYPHFACTORY_H


class Glyph;
class GlyphFactory {
public:
	GlyphFactory();
	GlyphFactory(const GlyphFactory& source);
	~GlyphFactory();
	Glyph* Make(const char(*content));
	Glyph* MakeLine(const char(*content));
};

#endif // !_GLYPHFACTORY_H
