main : main.o emu.o stack.o opcodes.o
	gcc -o main main.o emu.o stack.o opcodes.o `sdl2-config --cflags --libs`

main.o : src/main.c src/emu.h src/common.h
	gcc -c src/main.c

stack.o : src/stack.c src/stack.h
	gcc -c src/stack.c

emu.o :	src/emu.c src/emu.h src/stack.h src/opcodes.h
	gcc -c src/emu.c

opcodes.o : src/opcodes.c src/emu.h src/common.h src/stack.h src/opcodes.h
	gcc -c src/opcodes.c
