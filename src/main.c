#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "emu.h"
#include "SDL2/SDL.h"

static SDL_Window* window;
static SDL_Renderer* renderer;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: chip8c.exe <game file>\npress any key to exit");
		getchar();
		return 1;
	}

	// begin
	emu_ctx* emu = init_emu(argv[1]);

	SDL_Init(SDL_INIT_EVERYTHING); // should add error checking lmao
	SDL_CreateWindowAndRenderer(1280, 640, SDL_WINDOW_SHOWN, &window, &renderer);

	// "game loop"
	run_emu(emu, window, renderer);

	// end
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(emu->stack);
	free(emu);
	return 0;
}
