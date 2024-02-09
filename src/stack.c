#include <stdio.h>
#include "stack.h"

int push(stack* s, int element) {
	if (s->index >= MAX_STACK_SIZE - 1) {
		printf("ERROR IN STACK: stack overflow, index %d\n", s->index);
		return -1;
	}
	s->index++;
	s->elements[s->index] = element;
	return 0;
}

word pop(stack* s) {
	if (s->index < 0) {
		printf("ERROR IN STACK: stack underflow, index %d\n", s->index);
		return -1;
	}
	word element = s->elements[s->index];
	s->index--;
	return element;
}
