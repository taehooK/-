#include "Array.h"
#include <stddef.h> // size_t
#include <stdlib.h> // malloc, calloc
#include <memory.h> // memcpy



void Create(Array *array, Long capacity, size_t size) {
	
	array->front = calloc(capacity, size); //배열 생성
	array->capacity = capacity;
	array->length = 0;

}


Long Store(Array *array, Long index, void *object, size_t size) {
	// 1. object와 index를 읽는다.
	// 2. 배열에 object를 적는다.
	memcpy(((char*)array->front) + (index*size), object, size);
	array->length++;

	return index;
}


Long Insert(Array *array, Long index, void *object, size_t size) {
	void(*objects);
	Long i = 0;
	Long j = 0;

	// 1. 위치, 객체를 입력받는다.
	// 2. 새 배열을 만든다.
	objects = calloc(array->capacity + 1, size);
	// 3. 위치보다 작은동안 반복한다.
	while ( i < index){ // 3.1. 새배열에 옮겨적는다
		memcpy(((char*)objects) + (j*size), ((char*)array->front) + (i*size), size);
		i++;
		j++;
	}
	j++; // 4. 위치 다음부터 사용량까지 반복한다.
	while (i < array->length) {
		// 4.1. 새 배열에 옮겨적는다.
		memcpy((char*)objects + (j *size), ((char*)array->front) + (i*size), size);
		j++;
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
	}
	array->front = objects;
	array->capacity++;
	memcpy(((char*)array->front) + (index*size), object, size);
	array->length++;

	return index;
}


Long AppendFromFront(Array *array, void *object, size_t size) {
	Long index = 0;
	void(*objects);
	Long i = 0;
	//1. 객체를 입력받는다.
	//2. 새 배열을 만든다.
	objects = calloc(array->capacity + 1, size);

	//3. 새 배열에 두 번째 부터 옮겨적는다.
	while (i < array->length) {
		memcpy((char*)objects + ((i + 1) *size), ((char*)array->front) + (i*size), size);
		i++;
	}
	//4. 기존 배열을 지운다.
	if (array->front != NULL) {
		free(array->front);
	}
	array->front = objects;
	array->capacity++;
	//5. 첫 번째에 객체를 적는다.
	memcpy(((char*)array->front) + (index*size), object, size);
	array->length++;

	return index;
}


Long AppendFromRear(Array *array, void *object, size_t size) {
	Long index = 0;
	void(*objects);
	Long i = 0;

	objects = calloc(array->capacity + 1, size);
	while (i < array->length) {
		memcpy((char*)objects + (i *size), ((char*)array->front) + (i*size), size);
		i++;
	}

	if (array->front != NULL) {
		free(array->front);
	}
	array->front = objects;
	array->capacity++;

	index = array->capacity - 1;
	memcpy(((char*)array->front) + (index*size), object, size);
	array->length++;
	return index;

}
Long Delete(Array *array, Long index, size_t size) {
	void(*objects) = NULL;
	Long i = 0;


	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	while (i < index) {
		memcpy((char*)objects + (i*size), ((char*)array->front) + (i*size), size);
		i++;
	}
	i = index+1;
	while (i < array->length) {
		memcpy((char*)objects + ((i - 1)*size), ((char*)array->front) + (i*size), size);
		i++;
	}

	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	if (array->capacity > 1) {
		array->front = objects;
	}

	array->capacity--;
	array->length--;
	index = -1;

	return index;
}
Long DeleteFromFront(Array *array, size_t size) {
	void(*objects) = NULL;
	Long i = 0;

	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	while (i < array->length) {
		memcpy((char*)objects + (i*size), ((char*)array->front) + ((i + 1)*size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front); 
		array->front = NULL;
	}
	if (array->capacity > 1) {
		array->front = objects;
	}

	array->capacity--;
	array->length--;

	return -1;
}
Long DeleteFromRear(Array *array, size_t size) {
	void(*objects) = NULL;
	Long i = 0;

	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	while (i < array->length-1) {
		memcpy((char*)objects+(i*size), ((char*)array->front) + (i*size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		array->front = objects;
	}

	array->capacity--;
	array->length--;

	return -1;

}
void Clear(Array *array) {
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL; // 클리어후 다시 사용할 예정 이므로 이렇게함
		
	}
	array->capacity = 0;
	array->length = 0;

}
Long Modify(Array *array, Long index, void *object, size_t size) {
	memcpy(((char*)array->front) + (index*size), object, size);

	return index;

}
Long LinearSearchUnique(Array *array, void *key, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long index = -1;

	while (i < array->length && compare(((char*)array->front) + (i*size), key) != 0) {
		i++;
	}
	if (i < array->length) {
		index = i;
	}

	return index;
}
void LinearSearchDuplicate(Array *array, void *key, Long *(*indexes), Long *count, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	*count = 0;
	*indexes = (Long(*))calloc(array->length, sizeof(Long));
	while (i < array->length) {
		if (compare(((char*)array->front) + (i*size), key) == 0) {
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
		i++;
	}

}
Long BinarySearchUnique(Array *array, void *key, size_t size, int(*compare)(void*, void*)) {
	Long start = 0;
	Long index = -1;
	Long end;
	Long searchIndex;

	end = array->length - 1;
	searchIndex = (start + end) / 2;
	while (start <= end && compare(((char*)array->front) + (searchIndex*size), key) != 0) {
		if (compare(((char*)array->front) + (searchIndex*size), key) < 0) {
			start = searchIndex + 1;
		}
		else {
			end = searchIndex - 1;
		}

		searchIndex = (start + end) / 2;
	}

	if (start <= end) {
		index = searchIndex;
	}

	return index;
}
void BinarySearchDuplicate(Array *array, void *key, Long*(*indexes), Long *count, size_t size, int(*compare)(void*, void*)) {
	Long start = 0;
	Long end;
	Long i;
	Long j;
	Long searchIndex;
	*count = 0;

	*indexes = (Long(*))calloc(array->length, sizeof(Long));

	end = array->length - 1;
	searchIndex = (start + end) / 2;
	while (start <= end && compare(((char*)array->front) + (searchIndex*size), key) != 0) {
		if (compare(((char*)array->front) + (searchIndex*size), key) < 0) {
			start = searchIndex + 1;
		}
		else {
			end = searchIndex - 1;
		}

		searchIndex = (start + end) / 2;
	}
	i = searchIndex - 1;

	while (i >= start && compare(((char*)array->front) + (i*size), key) == 0){
		i--;
	}
	start = i + 1;

	j = 0;
	i = start;
	while (i <= end && compare(((char*)array->front) + (i*size), key) == 0) {
		(*indexes)[j] = i;
		j++;
		(*count)++;
		i++;
	}


}
void BubbleSort(Array *array, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j;
	void *temp;
	Long check = 1;
	temp = malloc(size);

	while (i < array->length - 1 && check == 1) {
		check = 0;
		j = 0;
		while (j < array->length - (i + 1)) {
			if (compare(((char*)array->front) + (j*size), ((char*)array->front) + ((j + 1)*size)) > 0) {
				memcpy(temp, ((char*)array->front) + (j*size), size);
				memcpy(((char*)array->front) + (j*size), ((char*)array->front) + ((j + 1)*size), size);
				memcpy(((char*)array->front) + ((j + 1)*size), temp, size);
				check = 1;
			}
			j++;
		}
		i++;
	}

	if (temp != NULL) {
		free(temp); 
	}
}
void SelectionSort(Array *array, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j;
	void *min;
	Long minIndex;
	min= malloc(size);
	
	while (i < array->length - 1) {
		minIndex = i;
		j = i + 1;
		while (j < array->length) {
			if (compare(((char*)array->front) + (minIndex*size), ((char*)array->front) + (j*size)) > 0) {
				minIndex = j;
			}
			j++;
		}
		memcpy(min, ((char*)array->front) + (minIndex*size), size);
		memcpy(((char*)array->front) + (minIndex*size), ((char*)array->front) + (i*size), size);
		memcpy(((char*)array->front) + (i*size), min, size);
		i++;
	}
	if (min != NULL) {
		free(min);
	}

}
void InsertionSort(Array*array, size_t size, int(*compare)(void*, void*)) {
	Long i = 1;
	Long j;
	void *temp;
	temp = malloc(size);
	while (i < array->length) {
		memcpy(temp, ((char*)array->front) + (i*size), size);
		j = i - 1;
		while (j >= 0 && compare(((char*)array->front) + (j*size), temp) > 0) {
			memcpy(((char*)array->front) + ((j + 1)*size), ((char*)array->front) + (j*size), size);
			j--;
		}
		memcpy(((char*)array->front) + ((j + 1)*size), temp, size);
		i++;
	}
	if (temp != NULL) {
		free(temp);
	}

}
void Merge(Array *array, Array one, Array other, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	if (array->front != NULL) {
		free(array->front);
	}

	array->front = calloc(one.length + other.length, size);
	array->capacity = one.length + other.length;
	array->length = 0;

	while (i < one.length && j< other.length) {
		if (compare(((char*)one.front) + (i*size), ((char*)other.front) + (j*size)) < 0) {
			memcpy(((char*)array->front) + (k*size), ((char*)one.front) + (i*size), size);
			k++;
			array->length++;
			i++;
		}
		else {
			memcpy(((char*)array->front) + (k*size), ((char*)other.front) + (j*size), size);
			k++;
			array->length++;
			j++;
		}
	}

	while (i < one.length) {
		memcpy(((char*)array->front) + (k*size), ((char*)one.front) + (i*size), size);
		k++;
		array->length++;
		i++;
	}
	while (j < one.length) {
		memcpy(((char*)array->front) + (k*size), ((char*)other.front) + (j*size), size);
		k++;
		array->length++;
		j++;
	}
}
void Destory(Array *array) {
	if (array->front = NULL) {
		free(array->front);
		array->front = NULL;
	}

}
void GetAt(Array *array, Long index, void *object, size_t size) {
	memcpy(object, ((char*)array->front) + (index*size), size);
}