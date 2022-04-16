#include "RectangleGraph.h"
#include "Visitor.h"

RectangleGraph::RectangleGraph() {
	this->rect.left = 0;
	this->rect.top = 0;
	this->rect.right = 0;
	this->rect.bottom = 0;
}

RectangleGraph::RectangleGraph(RECT rect, COLORREF color) 
	: Graph(color) {
	this->rect = rect;
}

RectangleGraph::RectangleGraph(const RectangleGraph& source) 
	: Graph(source) {
	this->rect = source.rect;
}

RectangleGraph::~RectangleGraph() {

}

RectangleGraph& RectangleGraph::operator=(const RectangleGraph& source) {
	Graph::operator=(source);
	this->rect = source.rect;

	return *this;
}

void RectangleGraph::Accept(Visitor *visitor) {
	visitor->Visit(this);
}

Graph* RectangleGraph::Clone() {
	return new RectangleGraph(*this);
}
