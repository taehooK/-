#include "Layout.h"
#include "Visitor.h"
#include "DrawingVisitor.h"
#include "Graph.h"

Layout::Layout(Long capacity)
:CompositeGraph(capacity){
}

Layout::Layout(const Layout& source)
:CompositeGraph(source){

}

Layout::~Layout(){
}

void Layout::Accept(Visitor *visitor){
	visitor->Visit(this);
}

Graph* Layout::Clone(){
	return new Layout(*this);
}

Layout& Layout::operator=(const Layout& source){
	CompositeGraph::operator=(source);
	return *this;
}