# COMP2521 24T3 - Assignment 1

# !!! DO NOT MODIFY THIS FILE !!!

CC = clang
CFLAGS = -Wall -Wvla -Werror -gdwarf-4

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testMset

testMset: testMset.c Mset.c Mset.h MsetStructs.h
	$(CC) $(CFLAGS) -o testMset testMset.c Mset.c

.PHONY: clean
clean:
	rm -f testMset

