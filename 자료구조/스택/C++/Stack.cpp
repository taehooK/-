#if 0//Stack.cpp
#include "Stack.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	Stack<Long> stack(2);
	Long index;
	Long ret;
	bool isEmpty;

	//1. ������ �����
	//2. 100�� �ִ�
	stack.Push(100);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//3. 200�� �ִ�
	stack.Push(200);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//4. 300�� �ִ�
	stack.Push(300);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//5. ������
	index = stack.Pop();
	if (index == -1) {
		cout << "���� ����" << endl;
		cout << stack.GetLength() << endl;
		cout << stack.GetCapacity() << endl;
	}


	//5.1. ������
	cout << stack.Top() << endl;
	stack.Pop();

	cout << stack.Top() << endl;
	stack.Pop();

	if (stack.IsEmpty() == true) {
		cout << "�� ����" << endl;
	}

	//6. 100�� �ִ�
	stack.Push(100);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//7. 200�� �ִ�
	stack.Push(200);
	cout << stack.Top() << endl;
	cout << stack.GetLength() << endl;
	cout << stack.GetCapacity() << endl;

	//8. ��������ϴ�.
	Stack<Long> other(stack);
	Long i = 0;
	while (i <= other.GetLength()) {
		cout << other.Top() << endl;
		other.Pop();
		i++;
	}

	//9. ġȯ�����ϴ�.
	Stack<Long> another = stack;
	i = 0;
	while (i <= another.GetLength()) {
		cout << another.Top() << endl;
		another.Pop();
		i++;
	}

	//8. ����
	stack.Empty();
	//9. ��������� Ȯ���Ѵ�.
	if (stack.IsEmpty() == true) {
		cout << "�� ����" << endl;
	}
	
	return 0;
}

#endif