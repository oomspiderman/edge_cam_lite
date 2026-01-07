# Simple Makefile for edge_cam_lite

CC      := gcc
CFLAGS  := -O2 -Wall -Wextra -std=c11
LDFLAGS :=

BIN_DIR := bin
IN_DIR  := img_samples/in
OUT_DIR := img_samples/out

HOST_SRC := edge_host/main.c
HOST_BIN := $(BIN_DIR)/edge_host

.PHONY: all run clean dirs

all: dirs $(HOST_BIN)

dirs:
	mkdir -p $(BIN_DIR) $(IN_DIR) $(OUT_DIR)

$(HOST_BIN): $(HOST_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: all
	$(HOST_BIN)

clean:
	# remove build artifacts
	rm -rf $(BIN_DIR)
	# empty OUT_DIR but keep the directory
	mkdir -p $(OUT_DIR)
	find $(OUT_DIR) -mindepth 1 -maxdepth 1 -exec rm -rf {} +

