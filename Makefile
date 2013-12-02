# ---------------------------------------------------
# Copyright (c) 2013 Pablo Caro. All Rights Reserved.
# Pablo Caro <me@pcaro.es> - http://pcaro.es/
# Makefile
# ---------------------------------------------------

CC=mpicc
CFLAGS=-Wall
EXAMPLES=01-hello_world

build_all: $(EXAMPLES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXAMPLES)

.PHONY: clean
