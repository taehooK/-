#include "Array.h"
#include<iostream>

using namespace std;
int Compare(void *one, void *other);

int main(int argc, char* argv[]) {
	//1. �迭�� 3��ŭ �����ϴ�

	Array<Long> array(3);

	//2. ���� 30�� ��ġ1�� �����ϴ�.
	Long index = array.Store(0, 30);
	cout << array[index] << endl;
	//3. ���� 10�� ��ġ 2�� �����ϴ�.
	index = array.Store(1, 10);
	cout << array[index] << endl;
	//4. ���� 20�� ��ġ 3�� �����ϴ�.
	index = array.Store(2, 20);
	cout << array[index] << endl;
	//5. ���� 20�� ��ġ 2�� ����.
	index = array.Insert(1, 20);
	cout << array[index] << endl;
	cout << "=======�˻�=========" << endl;
	//6. ���� 20�� ���� �˻��ϴ�.
	Long key = 20;
	Long(*indexes);
	Long count;
	array.LinearSearchDuplicate(&key, &indexes, &count, Compare);

	Long i = 0;
	while (i < count) {
		cout << array[indexes[i]] << endl;
		i++;
	}
	
	if (indexes != 0) {
		delete[] indexes;
	}

	// 7. �� ���� �迭 ��Ҹ� �����ϴ�.
	index = array.DeleteFromRear();

	if (index == -1)
	{
		cout << "�����." << endl;
	}


	// 8.�� �տ� ���� 30�� �߰��ϴ�.
	index = array.AppendFromFront(30);
	cout << array[index] << endl;


	// 9. 10�� �����˻��ϴ�.
	key = 20;
	index = array.LinearSearchUnique(&key, Compare);
	
	if (index > -1) {
		cout << array[index] << endl;
	}

	// 10. ��ǰ�����ϴ�.
	array.BubbleSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}
	cout << endl;

	//11. Ŭ�����ϴ�.
	array.Clear();
	if (array.GetLength() == 0) {
		cout << "Ŭ����" << endl;
	}

	//12. one�迭�� 2��ŭ �����ϴ�
	Array<Long> one(2);
	//13. 50�� one�� ��ġ1�� �����Ѵ�.
	index = one.Store(0, 50);
	cout << one[index] << endl;
	//14. 90�� one�� ��ġ 2�� �����ϴ�.
	index = one.Store(1, 90);
	cout << one[index] << endl;

	//15. other �迭�� 3��ŭ �����ϴ�.
	Array<Long>other(3);
	//16. ���� 60�� other�� ��ġ 1�� �����ϴ�.
	index = other.Store(0, 60);
	cout << other[index] << endl;
	//17. 2��°�� 70����
	index = other.Store(1, 70);
	cout << other[index] << endl;
	//18. 3��°�� 80����
	index = other.Store(2, 80);
	cout << other[index] << endl;
	//19. ����
	array.Merge(one, other, Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}

	//20. 4��° ����
	index = array.Delete(3);
	if (index == -1) {
		cout << "���� �Ǿ����ϴ�. " << endl;
	}
	//21. 70�� �̺а˻�
	key = 70;
	index = array.BinarySearchUnique(&key, Compare);

	if (index > -1) {
		cout << array[index] << endl;
	}

	//22. 90�� �� �ڿ� ����
	index = array.AppendFromRear(90);
	cout << array[index] << endl;

	//23. ���������ϴ�.

	array.SelectionSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}
	cout << endl;
	cout << "�ִ��ε���: " << array.Max(Compare) << endl;
	cout << "�ּ��ε���: " << array.Min(Compare) << endl;

	//24. �� ���� �迭��Ҹ� �����ϴ�.
	index = array.DeleteFromFront();

	if (index == 01) {
		cout << "���������ϴ�." << endl;
	}
	
	//25. ��ġ 1�� �迭��Ҹ� ���� 90���� �����ϴ�
	index = array.Modify(0, 90);
	cout << array[index] << endl;

	//26. ���������ϴ�.
	array.InsertionSort(Compare);

	Long object;
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << " ";
		i++;
	}
	cout << endl;

	//27. 90�� ã��.
	key = 90;
	array.BinarySearchDuplicate(&key, &indexes, &count, Compare);

	i = 0;
	while(i < count) {
		object = array.GetAt(indexes[i]);
		cout << object << endl;
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}
	
	Long *ret;
	i = 0;
	while (i < array.GetLength()) {
		ret = array + i;
		cout << *ret << " ";
		i++;
	}

	
	cout << "�ִ��ε���: " << array.Max(Compare) << endl;
	cout << "�ּ��ε���: " << array.Min(Compare) << endl;


	cout << endl;
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