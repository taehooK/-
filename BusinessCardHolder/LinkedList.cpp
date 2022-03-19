#include "LinkedList.h"
#if 0
#include <iostream>

using namespace std;

int Compare(void *one, void *other);

int main(int argc, char *argv[]) {
	//1. ���Ḯ��Ʈ�� �����.
	LinkedList<Long> linkedList;

	LinkedList<Long>::Node *index;

	//2. 100�� �� �ڿ� �߰��ϴ�.				100
	index = linkedList.AppendFromTail(100);
	cout << index->GetObject() << endl;

	//3. 200�� �� �տ� �߰��ϴ�.				200 100
	index = linkedList.AppendFromHead(200);
	cout << index->GetObject() << endl;

	//4. �������� �̵��ϴ�.						200 100
	index = linkedList.Next();
	cout << index->GetObject() << endl;

	//5. 300�� 100�տ� �����ϴ�.				200 300 100
	index = linkedList.InsertBefore(index, 300);
	cout << index->GetObject() << endl;

	//6. ó������ �̵��ϴ�.						200 300 100
	index = linkedList.First();
	cout << index->GetObject() << endl;
	
	//7. 400�� 200�ڿ� �����ϴ�.				200 400 300 100
	index = linkedList.InsertAfter(index, 400);
	cout << index->GetObject() << endl;

	//8. 300�� �����˻��ϴ�.					200 400 300 100
	Long key = 300;
	index = linkedList.LinearSearchUnique(&key, Compare);
	cout << index->GetObject() << endl;
	
	//9. 300�� �����ϴ�.						200 400 100
	index = linkedList.Delete(index);
	if (index == 0) {
		cout << "���������ϴ�." << endl;
	}

	//10. 100�� �������� �̵��ϴ�.				200 400 100
	index = linkedList.Previous();
	cout << index->GetObject() << endl;

	//11. 200�� �� �ڿ� �߰��ϴ�.				200 400 100 200
	index = linkedList.AppendFromTail(200);
	cout << index->GetObject() << endl;

	//12. 200�� �����˻��ϴ�.					200 400 100 200
	LinkedList<Long>::Node* (*indexes);
	Long count;
	Long object;
	key = 200;
	linkedList.LinearSearchDuplicate(&key, &indexes, &count, Compare);
	Long i = 0;
	while (i < count) {
		object = indexes[i]->GetObject();
		cout << object << endl;
		i++;
	}

	//13. ó�� 200���� �̵��ϴ�.				200 400 100 200
	index = linkedList.Move(indexes[0]);
	cout << index->GetObject() << endl;

	//14. �� �ڸ� �����.						200 400 100
	index = linkedList.DeleteFromTail();
	if (index == 0) {
		cout << "���������ϴ�." << endl;
	}

	//15. �� ���� �����.						400 100
	index = linkedList.DeleteFromHead();
	if (index == 0) {
		cout << "���������ϴ�." << endl;
	}

	//16. ���������� �̵��ϴ�.					400 100
	index = linkedList.Last();
	cout << index->GetObject() << endl;

	//17. ��� �����.
	linkedList.DeleteAllItems();
	if (linkedList.GetLength() == 0) {
		cout << "��� ���������ϴ�." << endl;
	}

	//18. 500�� �� �ڿ� �����ϴ�.				500
	index = linkedList.AppendFromTail(500);
	cout << index->GetObject() << endl;

	//19. 100�� 500 �տ� �����ϴ�.				100 500
	index = linkedList.InsertBefore(index, 100);
	cout << index->GetObject() << endl;

	//�����ϴ�.									`100 500
	LinkedList<Long> newLinkedList(linkedList);
	i = 0;
	while (i < newLinkedList.GetLength()) {
		cout << newLinkedList[i] << endl;
		i++;
	}

	//ġȯ�ϴ�.
	index = newLinkedList.DeleteFromTail();		//`100
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
		ret = -1;
	}

	return ret;
}
#endif