CC = cc
MINGW = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Werror
DEBUG_FLAGS = -g3 -Og
RELEASE_FLAGS = -O3
TARGET = ./build/tetris
SRCS = $(wildcard src/*.c)

windows: TARGET := ./build/tetris.exe

all: CFLAGS += $(RELEASE_FLAGS)
all: $(TARGET)

windows: CC = $(MINGW)
windows: CFLAGS += $(RELEASE_FLAGS)
windows: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p ./build
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

clean:
	rm -rf ./build

.PHONY: all debug clean
