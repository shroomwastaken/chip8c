#include <stdio.h>
#include <stdlib.h>
#include "emu.h"
#include "opcodes.h"

emu_ctx* init_emu(char* path) {
	// initialize stack
	stack* stk = malloc(sizeof(stack));
	stk->index = -1;

	// initialize context
	emu_ctx* ctx = malloc(sizeof(emu_ctx));
	ctx->stack = stk;

	// load in file
	FILE* file = fopen(path, "rb");
	fread(&ctx->game_memory[0x200], 0xFFF, 1, file);
	fclose(file);

	return ctx;
}

void run_emu(emu_ctx* emu) {
	while (1)
	{
		word opcode = get_op_code(emu);
		process_opcode(emu, opcode);
	}
}

word get_op_code(emu_ctx* ctx) {
	word res = ctx->game_memory[ctx->program_counter] << 8;
	res |= ctx->game_memory[ctx->program_counter + 1];
	return res;
}

void incr_pc(emu_ctx *emu) {
	emu->program_counter += 2;
}

void set_pc(emu_ctx *emu, word addr) {
	emu->program_counter = addr;
}
