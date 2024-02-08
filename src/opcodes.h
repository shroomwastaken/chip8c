#pragma once
#include "emu.h"
#include "common.h"

void process_opcode(emu_ctx* emu, word opcode);
void draw_sprite(emu_ctx* emu, byte x, byte y, byte n);