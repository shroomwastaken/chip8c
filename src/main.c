#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "emu.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: chip8c.exe <game file>\n");
		return 1;
	}

	// begin
	emu_ctx* emu = init_emu(argv[1]);
	run_emu(emu);

	// end
	free(emu->stack);
	free(emu);
	return 0;
}