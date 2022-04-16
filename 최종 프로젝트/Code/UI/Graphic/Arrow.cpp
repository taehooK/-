#include "Arrow.h"
#include "Visitor.h"
#include "DrawingVisitor.h"

Arrow::Arrow() {
	this->rect = { 0, };
}

Arrow::Arrow(RECT rect, COLORREF color) 
	: Graph(color) {
	this->rect = rect;
}

Arrow::Arrow(const Arrow& source) 
	: Graph(source) {
	this->rect = source.rect;
}

Arrow::~Arrow() {

}

void Arrow::Accept(Visitor *visitor) {
	visitor->Visit(this);
}

Graph* Arrow::Clone() {
	return new Arrow(*this);
}

Arrow& Arrow::operator=(const Arrow& source) {
	Graph::operator=(source);
	this->rect = source.rect;
	return *this;
}