//LinekdList.cpp
#include "LinkedList.h"
#include <iostream>
using namespace std;

int Compare(void *one, void *other);
int main(int argc, char *argv[]) {
	LinkedList<Long> linkedList;
	LinkedList<Long>::Node *index;
	
	index = linkedList.AppendFromTail(100);
	cout<<index->GetObject() << endl;

	index = linkedList.AppendFromTail(200);
	cout<<index->GetObject() << endl;

	index = linkedList.Next();
	cout << index->GetObject() << endl;

	index = linkedList.InsertBefore(index, 300);
	cout << index->GetObject() << endl;

	index = linkedList.First();
	cout << index->GetObject() << endl;

	index = linkedList.InsertAfter(index, 400);
	cout << index->GetObject() << endl;

	Long key = 300;
	index = linkedList.LinearSearchUnique(&key, Compare);
	cout << index->GetObject() << endl;

	index = linkedList.Delete(index);
	if (index == 0) {
		cout << "»èÁ¦ µÊ" << endl;
	}
	index = linkedList.Previous();
	cout << index->GetObject() << endl;

	index = linkedList.AppendFromTail(200);
	cout << index->GetObject() << endl;

	LinkedList<Long>::Node* (*indexes);
	Long count;
	key = 200;
	linkedList.LinearSearchDuplicate(&key, &indexes, &count, Compare);
	Long object;
	Long i = 0;
	while (i < count) {
		object = indexes[i]->GetObject();
		cout << object << endl;
		i++;
	}

	index = linkedList.Move(indexes[0]);
	cout << index->GetObject() << endl;
	
	if (indexes != 0) {
		delete[] indexes;
	}

	index = linkedList.DeleteFromTail();
	if (index == 0) {
		cout << "Áö¿öÁü. " << endl;
	}

	index = linkedList.DeleteFromHead();
	if (index == 0) {
		cout << "Áö¿öÁü. " << endl;
	}

	index = linkedList.Last();
	cout << index->GetObject() << endl;

	linkedList.DeleteAllItems();
	if (linkedList.GetLength() == 0) {
		cout << "¸ðµÎ Áö¿öÁü. " << endl;
	}

	index = linkedList.AppendFromTail(500);
	cout << index->GetObject() << endl;

	index = linkedList.InsertBefore(index, 100);
	cout << index->GetObject() << endl;

	LinkedList<Long> newLinkedList(linkedList);
	i = 0;
	while (i < newLinkedList.GetLength()) {
		cout << newLinkedList[i] << endl;
		i++;
	}
	index = newLinkedList.DeleteFromTail();
	linkedList = newLinkedList;
	i = 0;
	while (i < linkedList.GetLength()) {
		cout << linkedList[i] << endl;
		i++;
	}

	return 0;
}
int Compare(void *one, void *other) {
	Long *one_ = static_cast<Long*>(one);
	Long *other_ = static_cast<Long*>(other);
	int ret;

	if (*one_ < *other_) {
		ret = -1;
	}
	else if (*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > *other_) {
		ret = 1;
	}

	return ret;
}