main : main.o emu.o stack.o
	gcc -o main main.o emu.o stack.o

main.o : src/main.c src/emu.h src/common.h
	gcc -c src/main.c

stack.o : src/stack.c src/stack.h
	gcc -c src/stack.c

emu.o :	src/emu.c src/emu.h src/stack.h
	gcc -c src/emu.c