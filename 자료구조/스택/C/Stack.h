//Stack.h
#ifndef _STACK_H
#define _STACK_H
#include <stddef.h>

typedef enum boolean{
	TRUE = 1,
	FALSE = 0
}Boolean;
typedef signed long int Long;
typedef struct _stack {
	void(*front);
	Long capacity;
	Long length;
	Long top;
}Stack;

void Stack_Create(Stack *stack, Long capacity, size_t size);
Long Push(Stack *stack, void *object, size_t size);
Long Pop(Stack *stack);
void Top(Stack *stack, void *object, size_t size);
void Stack_Empty(Stack *stack);
void Stack_Destroy(Stack *stack);
Boolean Stack_IsEmpty(Stack *stack);






#endif // _STACK_H
