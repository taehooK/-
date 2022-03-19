#include "DummyLine.h"


DummyLine::DummyLine(Long capacity)
:Line(capacity){
}

DummyLine::DummyLine(const DummyLine& source)
:Line(source){
}

DummyLine::DummyLine(Line& line)
:Line(line){

}
DummyLine::~DummyLine(){
}

DummyLine& DummyLine::operator=(const DummyLine& source){
	Line::operator=(source);
	return *this;
}
