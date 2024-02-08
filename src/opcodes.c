#include "opcodes.h"
#include "stack.h"
#include "common.h"
#include <stdlib.h>

void process_opcode(emu_ctx* emu, word opcode) {
	byte first_digit = (opcode & 0xF000) >> 24;
	byte second_digit = (opcode & 0x0F00) >> 16;
	byte third_digit = (opcode & 0x00F0) >> 8;
	byte last_digit = opcode & 0x000F;
	switch (first_digit) {
		case 0:
			switch (opcode & 0xF) {
				case 0:
					// clear screen (CLS)
					not_implemented;
					break;
				case 0xE:
					// opcode 00EE: RET
					emu->program_counter = pop(emu->stack);
					break;
			}
			break;
		case 1:
			// opcode 1NNN: jump to address NNN
			emu->program_counter = opcode & 0xFFF;
			break;
		case 2:
			// opcode 2NNN: call subroutine at NNN
			push(emu->stack, emu->program_counter);
			emu->program_counter = opcode & 0xFFF;
			break;
		case 3:
			// opcode 3XKK: skip next instruction if register X == KK
			if (emu->registers[second_digit] == (opcode & 0x00FF))
				emu->program_counter += 2;
			break;
		case 4:
			// opcode 4XKK: skip next instruction if register X != KK
			if (emu->registers[second_digit] != (opcode & 0x00FF))
				emu->program_counter += 2;
			break;
		case 5:
			// opcode 5XY0: skip next instruction if register X == register Y
			if (emu->registers[second_digit] == emu->registers[third_digit])
				emu->program_counter += 2;
			break;
		case 6:
			// opcode 6XKK: put value KK into register X
			emu->registers[second_digit] = opcode & 0x00FF;
			break;
		case 7:
			// opcode 7XKK: register X += KK
			emu->registers[second_digit] += opcode & 0x00FF;
			break;
		case 8:
			switch (last_digit) {
				case 0:
					// opcode 8XY0: set register X to be equal to register Y
					emu->registers[second_digit] = emu->registers[third_digit];
					break;
				case 1:
					// opcode 8XY1: register X |= register Y (bitwise OR)
					emu->registers[second_digit] |= emu->registers[third_digit];
					break;
				case 2:
					// opcode 8XY2: register X &= register Y (bitwise AND)
					emu->registers[second_digit] &= emu->registers[third_digit];
					break;
				case 3:
					// opcode 8XY3: register X ^= register Y (bitwise XOR)
					emu->registers[second_digit] ^= emu->registers[third_digit];
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
					break;
				case 5:
					// opcode 8XY5: register X -= register Y, register F = 0 if there is an underflow else 1
					if (emu->registers[second_digit] >= emu->registers[third_digit]) 
						emu->registers[0xF] = 1;
					else
						emu->registers[0xF] = 0;
					emu->registers[second_digit] -= emu->registers[third_digit];
					break;
				case 6:
					// opcode 8XY6: register F = register X & 1, then divide register X by two
					emu->registers[0xF] = emu->registers[second_digit] & 1;
					emu->registers[second_digit] >>= 1;
					break;
				case 7:
					// opcode 8XY7: register X = register Y - register X, register F = 0 if there is an underflow else 1
					if (emu->registers[second_digit] >= emu->registers[third_digit])
						emu->registers[0xF] = 1;
					else
						emu->registers[0xF] = 0;
					emu->registers[second_digit] = emu->registers[third_digit] - emu->registers[second_digit];
					break;
				case 0xE:
					// opcode 8XYE: register F = (register X >> 7) & 1, then register X *= 2
					emu->registers[0xF] = (emu->registers[second_digit] >> 7) & 1;
					emu->registers[second_digit] <<= 1;
					break;
			}
			break;
		case 9:
			// opcode 9XY0: skip next instruction if register X != register Y
			if (emu->registers[second_digit] != emu->registers[third_digit]) 
				emu->program_counter += 2;
			break;
		case 0xA:
			// opcode ANNN: register I (the address register) = to NNN
			emu->address_register = opcode & 0xFFF;
			break;
		case 0xB:
			// opcode BNNN: jump to address NNN + register 0
			emu->program_counter = (opcode & 0xFFF) + emu->registers[0];
			break;
		case 0xC:
			// opcode CXKK: register X = random unsigned byte & KK
			emu->registers[second_digit] = (rand() % 256) & (opcode & 0x00FF);
			break;
		case 0xD:
			not_implemented;
		case 0xE:
			not_implemented;
		case 0xF:
			switch (opcode & 0x00FF) {
				case 0x07:
					not_implemented;
				case 0x0A:
					not_implemented;
				case 0x15:
					not_implemented;
				case 0x18:
					not_implemented;
				case 0x1E:
					// opcode FX1E: register I += register X
					emu->address_register += emu->registers[second_digit];
				case 0x29:
					not_implemented;
				case 0x33:
					// opcode FX33: store binary-coded decimals of register X in the address currently in register I
					// register I = hundreds digit, register I + 1 = tens digit, register I + 2 = ones digit
					emu->game_memory[emu->address_register] = (byte)(emu->registers[second_digit] / 100 % 10);
					emu->game_memory[emu->address_register + 1] = (byte)(emu->registers[second_digit] / 10 % 10);
					emu->game_memory[emu->address_register + 2] = (byte)(emu->registers[second_digit] % 10);
					break;
				case 0x55:
					// opcode FX55: store registers 0 through X in memory starting from address currently in register I
					for (byte i = 0; i <= second_digit; i++)
						emu->game_memory[emu->address_register + i] = emu->registers[i];
					break;
				case 0x65:
					// opcode FX65: set registers 0 through x to values from memory starting from address currently in register I
					for (byte i = 0; i <= second_digit; i++)
						emu->registers[i] = emu->game_memory[emu->address_register + i];
					break;
			}
			break;
	}
}