# ---------------------------------------------------
# Copyright (c) 2013 Pablo Caro. All Rights Reserved.
# Pablo Caro <me@pcaro.es> - http://pcaro.es/
# Makefile
# ---------------------------------------------------

CC=mpicc
CFLAGS=-Wall
EXAMPLES=01-hello_world 02-master_workers 03-send_recv_01 04-send_recv_02

build_all: $(EXAMPLES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXAMPLES)

.PHONY: clean
