C_FLAGS=-std=c99 -pedantic -Wall -W -O2
C_DEPENDENCIES=-lm

default:
	gcc *.c $(C_FLAGS) $(C_DEPENDENCIES) -o neural.out
