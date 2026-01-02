CC = cc
CFLAGS = -Wall -Werror
DEBUG_FLAGS = -g3 -Og
RELEASE_FLAGS = -O3
TARGET = ./build/tetris
SRCS = $(wildcard src/*.c)

all: CFLAGS += $(RELEASE_FLAGS)
all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p ./build
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

clean:
	rm -rf ./build

.PHONY: all debug clean
