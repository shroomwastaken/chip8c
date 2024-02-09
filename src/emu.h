#pragma once

#include "SDL2/SDL.h"
#include "common.h"
#include "stack.h"
#include "stdbool.h"

typedef struct {
	byte game_memory[0x1000];
	byte registers[16];
	word address_register;
	word program_counter;
	stack* stack;

	bool scr_data[32][64];
	bool keys[16];

	byte delay_timer;
	byte sound_timer;

	bool waiting;
	byte waiting_reg;
} emu_ctx;

void process_key(emu_ctx* emu, SDL_KeyCode key, bool state);
emu_ctx* init_emu(char* path);
void run_emu(emu_ctx* emu, SDL_Window* window, SDL_Renderer* renderer);
word get_op_code(emu_ctx* ctx);
void incr_pc(emu_ctx* emu);
void set_pc(emu_ctx* emu, word addr);
