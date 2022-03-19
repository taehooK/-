#ifndef _EDITORIALMEMORY_H
#define _EDITORIALMEMORY_H

#include "Stack.h"

typedef signed long int Long;
typedef enum workType{
	ADD,
	REMOVE
}WorkType;

class Glyph;
class EditWork;
class EditorialMemory {
public:
	EditorialMemory(Long capacity = 256);
	EditorialMemory(const EditorialMemory& source);
	~EditorialMemory();
	Long Push(WorkType workType, Glyph *glyph, Long row, Long column);
	Long Pop();
	EditWork* Top();

	void Empty();
	bool IsEmpty();

	Long GetCapacity() const;
	Long GetLength() const;

private:
	Stack<EditWork*> editWorks;
	Long capacity;
	Long length;
};

inline Long EditorialMemory::GetCapacity() const {
	return this->capacity;
}

inline Long EditorialMemory::GetLength() const{
	return this->length;
}



#endif // !_EDITORIALMEMORY_H
