#pragma once

#include "stack.h"

typedef struct
{
	byte game_memory[0x1000];
	byte registers[16];
	word address_register;
	word program_counter;
	byte scr_data[64][32];
	stack* stack;
} emu_ctx;

emu_ctx* init_emu(char* path);
void run_emu(emu_ctx* emu);
word get_op_code(emu_ctx* ctx);