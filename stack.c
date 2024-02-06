#include "stack.h"
#include <stdio.h>

int push(stack* s, int element) {
	if (s->index >= MAX_STACK_SIZE - 1) {
		printf("ERROR IN STACK: stack overflow");
		return -1;
	}
	s->index++;
	s->elements[s->index] = element;
	return 0;
}

int pop(stack* s) {
	if (s->index < 0) {
		printf("ERROR IN STACK: stack underflow");
		return -1;
	}
	int element = s->elements[s->index];
	s->index--;
	return element;
}