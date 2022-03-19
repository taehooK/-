#include "Array.h"
#include<iostream>

using namespace std;
int Compare(void *one, void *other);

int main(int argc, char* argv[]) {
	//1. 배열을 3만큼 생성하다

	Array<Long> array(3);

	//2. 숫자 30을 위치1에 저장하다.
	Long index = array.Store(0, 30);
	cout << array[index] << endl;
	//3. 숫자 10을 위치 2에 저장하다.
	index = array.Store(1, 10);
	cout << array[index] << endl;
	//4. 숫자 20을 위치 3에 저장하다.
	index = array.Store(2, 20);
	cout << array[index] << endl;
	//5. 숫자 20을 위치 2에 적다.
	index = array.Insert(1, 20);
	cout << array[index] << endl;
	cout << "=======검색=========" << endl;
	//6. 숫자 20을 선형 검색하다.
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

	// 7. 맨 뒤의 배열 요소를 삭제하다.
	index = array.DeleteFromRear();

	if (index == -1)
	{
		cout << "지우다." << endl;
	}


	// 8.맨 앞에 숫자 30을 추가하다.
	index = array.AppendFromFront(30);
	cout << array[index] << endl;


	// 9. 10을 선형검색하다.
	key = 20;
	index = array.LinearSearchUnique(&key, Compare);
	
	if (index > -1) {
		cout << array[index] << endl;
	}

	// 10. 거품정렬하다.
	array.BubbleSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}
	cout << endl;

	//11. 클리어하다.
	array.Clear();
	if (array.GetLength() == 0) {
		cout << "클리어" << endl;
	}

	//12. one배열을 2만큼 생성하다
	Array<Long> one(2);
	//13. 50을 one의 위치1에 저장한다.
	index = one.Store(0, 50);
	cout << one[index] << endl;
	//14. 90을 one의 위치 2에 저장하다.
	index = one.Store(1, 90);
	cout << one[index] << endl;

	//15. other 배열을 3만큼 생성하다.
	Array<Long>other(3);
	//16. 숫자 60을 other의 위치 1에 저장하다.
	index = other.Store(0, 60);
	cout << other[index] << endl;
	//17. 2번째에 70저장
	index = other.Store(1, 70);
	cout << other[index] << endl;
	//18. 3번째에 80저장
	index = other.Store(2, 80);
	cout << other[index] << endl;
	//19. 병합
	array.Merge(one, other, Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}

	//20. 4번째 삭제
	index = array.Delete(3);
	if (index == -1) {
		cout << "삭제 되었습니다. " << endl;
	}
	//21. 70을 이분검색
	key = 70;
	index = array.BinarySearchUnique(&key, Compare);

	if (index > -1) {
		cout << array[index] << endl;
	}

	//22. 90을 맨 뒤에 삼입
	index = array.AppendFromRear(90);
	cout << array[index] << endl;

	//23. 선택정렬하다.

	array.SelectionSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << " ";
		i++;
	}
	cout << endl;
	cout << "최대인덱스: " << array.Max(Compare) << endl;
	cout << "최소인덱스: " << array.Min(Compare) << endl;

	//24. 맨 앞의 배열요소를 삭제하다.
	index = array.DeleteFromFront();

	if (index == 01) {
		cout << "지워졌습니다." << endl;
	}
	
	//25. 위치 1의 배열요소를 숫자 90으로 수정하다
	index = array.Modify(0, 90);
	cout << array[index] << endl;

	//26. 삼입정렬하다.
	array.InsertionSort(Compare);

	Long object;
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << " ";
		i++;
	}
	cout << endl;

	//27. 90을 찾다.
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

	
	cout << "최대인덱스: " << array.Max(Compare) << endl;
	cout << "최소인덱스: " << array.Min(Compare) << endl;


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