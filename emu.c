#include <stdio.h>
#include <stdlib.h>
#include "emu.h"

void run_emu(char* path) {
	// initialize stack
	stack* stk = malloc(sizeof(stack));

	// initialize context
	emu_ctx* ctx = malloc(sizeof(emu_ctx));
	ctx->stack = stk;

	// load in file
	FILE* file = fopen(path, "rb");
	fread(&ctx->game_memory[0x200], 0xFFF, 1, file);
	fclose(file);
}

word get_next_op_code(emu_ctx* ctx) {
	word res = ctx->game_memory[ctx->program_counter] << 8;
	res |= ctx->game_memory[ctx->program_counter + 1];
	ctx->program_counter += 2;
	return res;
}