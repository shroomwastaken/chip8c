#include "opcodes.h"
#include "stack.h"
#include "common.h"
#include "emu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for that one time i use memset()

// i couldnt figure this out myself so i copied it from https://github.com/dmatlack/chip8/blob/master/chip8.c
// and adapted it for my code
void draw_sprite(emu_ctx* emu, byte x, byte y, byte n) {
	printf("trying to draw sprite at %d, %d; n = %d\n", x, y, n);
    unsigned row = y, col = x;
    unsigned byte_index;
    unsigned bit_index;

    // set the collision flag to 0
    emu->registers[0xF] = 0;
    for (byte_index = 0; byte_index < n; byte_index++) {
        byte b = emu->game_memory[emu->address_register + byte_index];

        for (bit_index = 0; bit_index < 8; bit_index++) {
            // the value of the bit in the sprite
            byte bit = (b >> bit_index) & 0x1;
            // the value of the current pixel on the screen
            byte *pixelp = &emu->scr_data[(row + byte_index) % 32][(col + (7 - bit_index)) % 64];

            // if drawing to the screen would cause any pixel to be erased,
            // set the collision flag to 1
            if (bit == 1 && *pixelp ==1) emu->registers[0xF] = 1;

            // draw this pixel by XOR
            *pixelp = *pixelp ^ bit;
        }
    }
}

void process_opcode(emu_ctx* emu, word opcode) {
	if (opcode == 0xFF03)
		exit(42);
	byte first_digit = (opcode & 0xF000) >> 12;
	byte second_digit = (opcode & 0x0F00) >> 8;
	byte third_digit = (opcode & 0x00F0) >> 4;
	byte last_digit = opcode & 0x000F;

	switch (first_digit) {
		case 0:
			switch (opcode & 0xF) {
				case 0:
					// opcode 00E0: CLS
					memset(emu->scr_data, 0, sizeof(emu->scr_data));
					break;
				case 0xE:
					// opcode 00EE: RET
					set_pc(emu, pop(emu->stack));
					break;
			}
			break;
		case 1:
			// opcode 1NNN: jump to address NNN
			set_pc(emu, opcode & 0xFFF);
			break;
		case 2:
			// opcode 2NNN: call subroutine at NNN
			push(emu->stack, emu->program_counter);
			set_pc(emu, opcode & 0xFFF);
			break;
		case 3:
			// opcode 3XKK: skip next instruction if register X == KK
			if (emu->registers[second_digit] == (opcode & 0x00FF))
				incr_pc(emu);
			incr_pc(emu);
			break;
		case 4:
			// opcode 4XKK: skip next instruction if register X != KK
			if (emu->registers[second_digit] != (opcode & 0x00FF))
				incr_pc(emu);
			incr_pc(emu);
			break;
		case 5:
			// opcode 5XY0: skip next instruction if register X == register Y
			if (emu->registers[second_digit] == emu->registers[third_digit])
				incr_pc(emu);
			incr_pc(emu);
			break;
		case 6:
			// opcode 6XKK: put value KK into register X
			emu->registers[second_digit] = opcode & 0x00FF;
			incr_pc(emu);
			break;
		case 7:
			// opcode 7XKK: register X += KK
			emu->registers[second_digit] += opcode & 0x00FF;
			incr_pc(emu);
			break;
		case 8:
			switch (last_digit) {
				case 0:
					// opcode 8XY0: set register X to be equal to register Y
					emu->registers[second_digit] = emu->registers[third_digit];
					incr_pc(emu);
					break;
				case 1:
					// opcode 8XY1: register X |= register Y (bitwise OR)
					emu->registers[second_digit] |= emu->registers[third_digit];
					incr_pc(emu);
					break;
				case 2:
					// opcode 8XY2: register X &= register Y (bitwise AND)
					emu->registers[second_digit] &= emu->registers[third_digit];
					incr_pc(emu);
					break;
				case 3:
					// opcode 8XY3: register X ^= register Y (bitwise XOR)
					emu->registers[second_digit] ^= emu->registers[third_digit];
					incr_pc(emu);
					break;
				case 4:
					// opcode 8XY4: register X += register Y, if result is > 255 then store
					// only the 8 lowest bits and set register F to 1 else set register F to 0
					if (emu->registers[second_digit] + emu->registers[third_digit] > 255) {
						emu->registers[0xF] = 1;
						emu->registers[second_digit] = (emu->registers[second_digit] + emu->registers[third_digit]) & 0xF;
					} else {
						emu->registers[0xF] = 0;
						emu->registers[second_digit] = emu->registers[second_digit] + emu->registers[third_digit];
					}
					incr_pc(emu);
					break;
				case 5:
					// opcode 8XY5: register X -= register Y, register F = 0 if there is an underflow else 1
					if (emu->registers[second_digit] >= emu->registers[third_digit])
						emu->registers[0xF] = 1;
					else
						emu->registers[0xF] = 0;
					emu->registers[second_digit] -= emu->registers[third_digit];
					incr_pc(emu);
					break;
				case 6:
					// opcode 8XY6: register F = register X & 1, then divide register X by two
					emu->registers[0xF] = emu->registers[second_digit] & 1;
					emu->registers[second_digit] >>= 1;
					incr_pc(emu);
					break;
				case 7:
					// opcode 8XY7: register X = register Y - register X, register F = 0 if there is an underflow else 1
					if (emu->registers[second_digit] >= emu->registers[third_digit])
						emu->registers[0xF] = 1;
					else
						emu->registers[0xF] = 0;
					emu->registers[second_digit] = emu->registers[third_digit] - emu->registers[second_digit];
					incr_pc(emu);
					break;
				case 0xE:
					// opcode 8XYE: register F = (register X >> 7) & 1, then register X *= 2
					emu->registers[0xF] = (emu->registers[second_digit] >> 7) & 1;
					emu->registers[second_digit] <<= 1;
					incr_pc(emu);
					break;
			}
			break;
		case 9:
			// opcode 9XY0: skip next instruction if register X != register Y
			if (emu->registers[second_digit] != emu->registers[third_digit])
				incr_pc(emu);
			incr_pc(emu);
			break;
		case 0xA:
			// opcode ANNN: register I (the address register) = to NNN
			emu->address_register = opcode & 0xFFF;
			incr_pc(emu);
			break;
		case 0xB:
			// opcode BNNN: jump to address NNN + register 0
			emu->program_counter = (opcode & 0xFFF) + emu->registers[0];
			break;
		case 0xC:
			// opcode CXKK: register X = random unsigned byte & KK
			emu->registers[second_digit] = (rand() % 256) & (opcode & 0x00FF);
			incr_pc(emu);
			break;
		case 0xD:
			// opcode DXYN: display sprite
			draw_sprite(emu, second_digit, third_digit, last_digit);
			incr_pc(emu);
			break;
		case 0xE:
			// opcode EX9E: skip next instruction if key with value of register X is pressed
			printf("unimplemented 0xE\n");
			incr_pc(emu);
			break;
		case 0xF:
			switch (opcode & 0x00FF) {
				case 0x07:
					// opcode FX07: register X = delay timer value
					emu->registers[second_digit] = emu->delay_timer;
					incr_pc(emu);
					break;
				case 0x0A:
					printf("unimplemented fx0a");
					incr_pc(emu);
					break;
				case 0x15:
					// opcode FX15: delay timer = register X
					emu->delay_timer = emu->registers[second_digit];
					incr_pc(emu);
					break;
				case 0x18:
					// opcode DX18: sound timer = register X
					emu->sound_timer = emu->registers[second_digit];
					incr_pc(emu);
					break;
				case 0x1E:
					// opcode FX1E: register I += register X
					emu->address_register += emu->registers[second_digit];
					incr_pc(emu);
					break;
				case 0x29:
					// opcode FX29: register I = location of sprite info for symbol in register X
					emu->address_register = 5 * emu->registers[second_digit];
					incr_pc(emu);
					break;
				case 0x33:
					// opcode FX33: store binary-coded decimals of register X in the address currently in register I
					// register I = hundreds digit, register I + 1 = tens digit, register I + 2 = ones digit
					emu->game_memory[emu->address_register] = (byte)(emu->registers[second_digit] / 100 % 10);
					emu->game_memory[emu->address_register + 1] = (byte)(emu->registers[second_digit] / 10 % 10);
					emu->game_memory[emu->address_register + 2] = (byte)(emu->registers[second_digit] % 10);
					incr_pc(emu);
					break;
				case 0x55:
					// opcode FX55: store registers 0 through X in memory starting from address currently in register I
					for (byte i = 0; i <= second_digit; i++)
						emu->game_memory[emu->address_register + i] = emu->registers[i];
					incr_pc(emu);
					break;
				case 0x65:
					// opcode FX65: set registers 0 through x to values from memory starting from address currently in register I
					for (byte i = 0; i <= second_digit; i++)
						emu->registers[i] = emu->game_memory[emu->address_register + i];
					incr_pc(emu);
					break;
			}
			break;
	}
}
