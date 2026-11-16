CC ?= cc
PKG_CONFIG ?= pkg-config
CFLAGS ?= -std=c99 -Wall -Wextra -O2

SDL_CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_ttf 2>/dev/null)
SDL_LIBS := $(shell $(PKG_CONFIG) --libs sdl2 SDL2_ttf 2>/dev/null)

ifeq ($(strip $(SDL_LIBS)),)
$(error SDL2 and SDL2_ttf were not found via pkg-config; install their development packages)
endif

ifeq ($(OS),Windows_NT)
TARGET ?= chess.exe
else
TARGET ?= chess
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.c Makefile
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $< -o $@ $(SDL_LIBS)

clean:
	$(RM) $(TARGET)
