main : main.o emu.o stack.o
	gcc -o main main.o emu.o stack.o

main.o : main.c emu.h common.h
	gcc -c main.c

emu.o :	emu.c emu.h stack.h
	gcc -c emu.c