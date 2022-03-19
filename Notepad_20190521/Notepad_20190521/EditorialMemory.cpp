#include "EditorialMemory.h"
#include "EditWorks.h"


EditorialMemory::EditorialMemory(Long capacity)
:editWorks(capacity){
	this->capacity = capacity;
	this->length = 0;
}

EditorialMemory::EditorialMemory(const EditorialMemory& source)
:editWorks(source.editWorks){
	this->capacity = source.capacity;
	this->length = source.length;
}

EditorialMemory::~EditorialMemory() {
	Long i = 0;
	while (i < this->length) {
		EditWork *editWork = this->editWorks.Top();
		if (editWork != 0) {
			delete editWork;
		}
		this->editWorks.Pop();
		i++;
	}

}

Long EditorialMemory::Push(WorkType workType, Glyph *glyph, Long row, Long column) {
	Long index;
	EditWork *editWork = 0;

	if (workType == ADD) {
		editWork = new Adding(glyph, row, column);
	}
	else if (workType == REMOVE) {
		editWork = new Removing(glyph, row, column);
	}

	index = this->editWorks.Push(editWork);

	this->capacity = this->editWorks.GetCapacity();
	this->length++;

	return this->length - 1;
}

Long EditorialMemory::Pop(){
	Long index;

	index = this->editWorks.Pop();

	this->length--;

	return index;
}

EditWork* EditorialMemory::Top(){
	return this->editWorks.Top();
}

void EditorialMemory::Empty(){
	this->editWorks.Empty();
}

bool EditorialMemory::IsEmpty(){
	return this->editWorks.IsEmpty();
}


