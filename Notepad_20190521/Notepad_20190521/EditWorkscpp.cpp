#include "EditWorks.h"
#include "Glyph.h"



EditWork::EditWork(){
	this->glyph = 0;
	this->row = -1;
	this->column = -1;
}

EditWork::EditWork(Glyph* glyph, Long row, Long column){
	this->glyph = glyph;
	this->row = row;
	this->column = column;
}

EditWork::EditWork(const EditWork& source){
	this->glyph = source.glyph->Clone();
	this->row = source.row;
	this->column = source.column;
}

EditWork::~EditWork(){
	if (this->glyph != 0) {
		delete this->glyph;
	}
}

EditWork& EditWork::operator=(const EditWork& other){
	this->glyph = other.glyph->Clone();
	this->row = other.row;
	this->column = other.column;

	return *this;
}

Glyph* EditWork::GetGlyph(){
	return this->glyph;
}
	
Adding::Adding(){
}

Adding::Adding(Glyph* glyph, Long row, Long column)
:EditWork(glyph, row, column){
}

Adding::Adding(const Adding& source)
:EditWork(source){
}

Adding::~Adding(){
}

Adding& Adding::operator=(const Adding& other){
	EditWork::operator=(other);
	return *this;
}

Removing::Removing(){
}

Removing::Removing(Glyph* glyph, Long row, Long column)
:EditWork(glyph, row, column) {

}

Removing::Removing(const Removing& source)
:EditWork(source){
}

Removing::~Removing(){}

Removing& Removing::operator=(const Removing& other){
	EditWork::operator=(other);
	return *this;
}
