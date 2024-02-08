#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "emu.h"
#include "opcodes.h"

unsigned char chip8_fontset[80] = 
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F 
};

emu_ctx* init_emu(char* path) {
	// initialize stack
	stack* stk = malloc(sizeof(stack));
	stk->index = -1;

	// initialize context
	emu_ctx* ctx = malloc(sizeof(emu_ctx));
	ctx->stack = stk;
	ctx->program_counter = 0x200;
	ctx->delay_timer = 0;
	ctx->sound_timer = 0;

	// load in the basic font
	for (int i = 0; i < 80; i++) {
        ctx->game_memory[i] = chip8_fontset[i];
    }

	// load in file
	FILE* file = fopen(path, "rb");
	fread(&ctx->game_memory[0x200], 1, 0x1000 - 0x200, file);
	fclose(file);

	srand(time(NULL)); // this is for that one opcode that needs a random byte

	printf("emu inited!\n");

	return ctx;
}

void run_emu(emu_ctx* emu, SDL_Window* window, SDL_Renderer* renderer, SDL_Event* event) {
	bool running = true;
	while (running) {
		// this works for now dunno how bad this really is tbh
		for (size_t y = 0; y < 32; y++) {
			for (size_t x = 0; x < 64; x++) {
				if (emu->scr_data[y][x] == 1)
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_Rect rect = {x * 8, y * 8, 8, 8};
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
		
        while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

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
