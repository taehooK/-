//Subject.h
#ifndef _SUBJECT_H
#define _SUBJECT_H

#include "Array.h"
typedef signed long int Long;

class Observer;
class Subject {
public:
	virtual ~Subject();
	virtual Long AttachObserver(Observer *obserever);
	virtual Long AttachObserver(Long index, Observer *observer);
	virtual Long DettachObserver(Observer *obserever);
	virtual void Notify();
	
	Subject(Long capacity = 256);
	Subject(const Subject& source);
	

	Observer* GetAt(Long index);
	Subject& operator=(const Subject& source);
	Long GetCapacity() const;
	Long GetLength() const;

private:
	Array<Observer*> observers;
	Long capacity;
	Long length;
};

inline Long Subject::GetCapacity() const {
	return this->capacity;
}

inline Long Subject::GetLength() const {
	return this->length;
}

int CompareObserverLinks(void *one, void *other);
#endif // !_SUBJECT_H

