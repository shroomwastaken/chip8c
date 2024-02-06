#pragma once

#include "common.h"

#define MAX_STACK_SIZE 256

typedef struct
{
	word elements[MAX_STACK_SIZE];
	unsigned char index;
} stack;

int push(stack* s, int element);
int pop(stack* s);
