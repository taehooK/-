#ifndef _STACK_H
#define _STACK_H

typedef signed long int Long;

template <typename T>
class Stack {
public:
	Stack(Long capacity = 256);
	Stack(const Stack& source);
	~Stack();
	Long Push(T object);
	Long Pop();
	T Top();
	void Empty();
	bool IsEmpty();
	T GetAt(Long index);

	Stack& operator=(const Stack& source);


	Long GetTop() const;
	Long GetCapacity() const;
	Long GetLength() const;


private:
	T (*front);
	Long top;
	Long capacity;
	Long length;
};

template <typename T>
inline Long Stack<T>::GetTop() const {
	return this->top;
}

template <typename T>
inline Long Stack<T>::GetCapacity() const {
	return this->capacity;
}

template <typename T>
inline Long Stack<T>::GetLength() const {
	return this->length;
}

template <typename T>
Stack<T>::Stack(Long capacity) {
	this->front = new T[capacity];
	this->top = 0;
	this->capacity = capacity;
	this->length = 0;
}

template <typename T>
Stack<T>::Stack(const Stack& source) {
	this->front = new T[source.capacity];
	this->capacity = source.capacity;

	Long i = 0;
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->length = source.length;
	this->top = source.top;
}

template <typename T>
Stack<T>::~Stack() {
	Long i = 0;
	if (this->front != 0) {
		delete[] this->front;
	}
}

template <typename T>
Long Stack<T>::Push(T object) {
	Long i;
	T(*temp);

	if (this->length >= this->capacity) {
		temp = new T[this->capacity + 1];
		i = 0;
		while (i < this->length) {
			temp[i] = this->front[i];
			i++;
		}
		if (this->front != 0) {
			delete[] this->front;
		}
		this->front = temp;
		this->capacity++;
	}
	this->front[this->top++] = object;
	this->length++;

	return this->top;
}

template <typename T>
Long Stack<T>::Pop() {
	this->length--;
	this->top--;

	return -1;
}

template <typename T>
T Stack<T>::Top() {
	return this->front[this->top - 1];
}

template <typename T>
void Stack<T>::Empty() {
	this->length = 0;
	this->top = 0;
}

template <typename T>
bool Stack<T>::IsEmpty() {
	bool isEmpty = false;
	if (this->length == 0) {
		isEmpty = true;
	}
	return isEmpty;
}

template <typename T>
T Stack<T>::GetAt(Long index) {
	return this->front[index];
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& source) {
	if (this->front != 0) {
		delete[] this->front;
	}
	
	this->front = new T[source.capacity];
	Long i = 0;
	while (i < source.length) {
		this->front[i] = source->front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;

	return *this;
}



#endif // !_STACK_H
