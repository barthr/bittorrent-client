CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS=

SRC_DIR := src
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/bittorrent-client

.PHONY: all clean build_docker

all: $(TARGET)

# Linking and Compilation step
$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR)

