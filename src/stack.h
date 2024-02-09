#pragma once

#include "common.h"

#define MAX_STACK_SIZE 256

typedef struct
{
	word elements[MAX_STACK_SIZE];
	short int index; // this is an int for it to start out at -1
} stack;

int push(stack* s, int element);
word pop(stack* s);
