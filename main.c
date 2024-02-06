#include <stdio.h>
#include "common.h"
#include "emu.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: chip8c.exe <game file>");
		return 1;
	}
	run_emu(argv[1]);
	return 0;
}