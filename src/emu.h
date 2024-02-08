#pragma once

#include "SDL2/SDL.h"
#include "common.h"
#include "stack.h"

typedef struct
{
	byte game_memory[0x1000];
	byte registers[16];
	word address_register;
	word program_counter;
	byte scr_data[32][64];
	stack* stack;
	byte delay_timer;
	byte sound_timer;
} emu_ctx;

emu_ctx* init_emu(char* path);
void run_emu(emu_ctx* emu, SDL_Window* window, SDL_Renderer* renderer, SDL_Event* event);
word get_op_code(emu_ctx* ctx);
void incr_pc(emu_ctx* emu);
void set_pc(emu_ctx* emu, word addr);
