#if 0//Stack.cpp
#include "Stack.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	Stack<Long> stack(2);
	Long index;
	Long ret;
	bool isEmpty;

	//1. 스택을 만들다
	//2. 100을 넣다
	stack.Push(100);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//3. 200을 넣다
	stack.Push(200);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//4. 300을 넣다
	stack.Push(300);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//5. 꺼내다
	index = stack.Pop();
	if (index == -1) {
		cout << "지워 졌음" << endl;
		cout << stack.GetLength() << endl;
		cout << stack.GetCapacity() << endl;
	}


	//5.1. 꺼내다
	cout << stack.Top() << endl;
	stack.Pop();

	cout << stack.Top() << endl;
	stack.Pop();

	if (stack.IsEmpty() == true) {
		cout << "빈 상태" << endl;
	}

	//6. 100을 넣다
	stack.Push(100);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//7. 200을 넣다
	stack.Push(200);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//8. 복사생성하다.
	Stack<Long> other(stack);
	Long i = 0;
	while (i <= other.GetLength()) {
		cout << other.Top() << endl;
		other.Pop();
		i++;
	}

	//9. 치환연산하다.
	Stack<Long> another = stack;
	i = 0;
	while (i <= another.GetLength()) {
		cout << another.Top() << endl;
		another.Pop();
		i++;
	}

	//8. 비우다
	stack.Empty();
	//9. 비워졌는지 확인한다.
	if (stack.IsEmpty() == true) {
		cout << "빈 상태" << endl;
	}
	
	return 0;
}

#endif