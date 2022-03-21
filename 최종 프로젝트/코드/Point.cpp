#include "Point.h"
#include "Visitor.h"

Point::Point() {
	this->x = 0;
	this->y = 0;
}

Point::Point(Long x, Long y, COLORREF color)
	: Graph(color) {
	this->x = x;
	this->y = y;
}

Point::Point(const Point& source) 
	: Graph(source) {
	this->x = source.x;
	this->y = source.y;
}

Point::~Point() {

}

void Point::Accept(Visitor *visitor) {
	visitor->Visit(this);
}

Point& Point::operator =(const Point& source) {
	Graph::operator=(source);
	this->x = source.x;
	this->y = source.y;

	return *this;
}

Graph* Point::Clone() {
	return new Point(*this);
}