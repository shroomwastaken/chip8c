#pragma once

#include "stack.h"

typedef struct
{
	byte game_memory[0xFFF];
	byte registers[16];
	word address_register;
	word program_counter;
	byte scr_data[64][32];
	stack* stack;
} emu_ctx;

void run_emu(char* path);
word get_next_op_code(emu_ctx* ctx);