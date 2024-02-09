#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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

void process_key(emu_ctx* emu, SDL_KeyCode key, bool state) {
	byte res;
	switch (key) {
		case SDLK_1:
			res = 1;
			break;
		case SDLK_2:
			res = 2;
			break;
		case SDLK_3:
			res = 3;
			break;
		case SDLK_4:
			res = 0xC;
			break;

		case SDLK_q:
			res = 4;
			break;
		case SDLK_w:
			res = 5;
			break;
		case SDLK_e:
			res = 6;
			break;
		case SDLK_r:
			res = 0xD;
			break;

		case SDLK_a:
			res = 7;
			break;
		case SDLK_s:
			res = 8;
			break;
		case SDLK_d:
			res = 9;
			break;
		case SDLK_f:
			res = 0xE;
			break;

		case SDLK_z:
			res = 0xA;
			break;
		case SDLK_x:
			res = 0;
			break;
		case SDLK_c:
			res = 0xB;
			break;
		case SDLK_v:
			res = 0xF;
			break;

		default:
			break;
	}

	if (state && emu->waiting) {
		emu->registers[emu->waiting_reg] = res;
		emu->waiting = false;
		incr_pc(emu); // probably not the exact way this should behave but idk
	}
	emu->keys[res] = state;
}

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

	printf("emu initialized!\n");

	return ctx;
}

void run_emu(emu_ctx* emu, SDL_Window* window, SDL_Renderer* renderer) {
	bool running = true;
	while (running) {
		clock_t start = clock();

		SDL_RenderClear(renderer);

		// this works for now dunno how bad this really is tbh
		for (size_t y = 0; y < 32; y++) {
			for (size_t x = 0; x < 64; x++) {
				if (emu->scr_data[y][x])
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_Rect rect = {.x = x * 20, .y = y * 20, .w = 20, .h = 20};
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
				break;
			} else if (e.type == SDL_KEYDOWN) {
				process_key(emu, e.key.keysym.sym, true);
			} else if (e.type == SDL_KEYUP) {
				process_key(emu, e.key.keysym.sym, false);
			}
		}

		if (!emu->waiting) {
			for (size_t x = 0; x < 15; x++) {
				word opcode = get_op_code(emu);
				process_opcode(emu, opcode);
			}
		}

		clock_t end = clock();
		SDL_RenderPresent(renderer);
		SDL_Delay(15 - (end - start) / CLOCKS_PER_SEC * 1000); // 60FPS (maybe)
		if (!emu->waiting && emu->delay_timer != 0)
			emu->delay_timer -= 1;
		if (!emu->waiting && emu->sound_timer != 0)
			emu->delay_timer -= 1;
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
