//Subject.cpp

#include "Subject.h"
#include "Observer.h"

Subject::~Subject() {
	Long i = 0;
	while (i < this->length) {
		delete this->observers[i];
		i++;
	}
}

Long Subject::AttachObserver(Observer *observer) {
	Long index;
	if (this->length < this->capacity) {
		index = this->observers.Store(this->length, observer);
	}
	else {
		index = this->observers.AppendFromRear(observer);
		this->capacity++;
	}

	this->length++;
	return index;
}

Long Subject::AttachObserver(Long index, Observer *observer) {
	index = this->observers.Insert(index, observer);
	this->length++;
	this->capacity++;

	return index;
}

Long Subject::DettachObserver(Observer *observer) {
	Long index = this->observers.LinearSearchUnique(observer, CompareObserverLinks);
	index = this->observers.Delete(index);
	this->capacity--;
	this->length--;
	
	return index;
}

void Subject::Notify() {
	Long i = 0;

	while (i < this->length) {
		this->observers.GetAt(i)->Update();
		i++;
	}
}

Subject::Subject(Long capacity)
:observers(capacity){
	this->capacity = capacity;
	this->length = 0;
}

Subject::Subject(const Subject& source) 
:observers(source.observers){
	Observer *observer;
	Long i = 0;
	while (i < source.length) {
		observer = const_cast<Subject&>(source).observers.GetAt(i);
		this->observers.Modify(i, observer->GetClone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

Observer* Subject::GetAt(Long index) {
	return this->observers.GetAt(index);
}

Subject& Subject::operator=(const Subject& source) {
	Observer *observer;
	Long i = 0;
	while (i < this->length) {
		delete this->observers[i];
		i++;
	}
	this->observers = source.observers;
	this->capacity = source.capacity;
	i = 0;
	while (i < source.length) {
		observer = (const_cast<Subject&>(source).observers.GetAt(i));
		this->observers.Modify(i, observer);
		i++;
	}
	this->length = source.length;
	return *this;
}

int CompareObserverLinks(void *one, void *other) {
	Observer **one_=static_cast<Observer**>(one);
	int ret;
	if (*one_ < other) {
		ret = -1;
	}
	else if (*one_ == other) {
		ret = 0;
	}
	else if (*one_ > other) {
		ret = 1;
	}

	return ret;
}