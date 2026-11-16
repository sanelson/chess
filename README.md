# chess
Baby's first chess game in C

## Build

Requires a C compiler, GNU Make, `pkg-config` (or `pkgconf`), SDL2, and SDL2_ttf.

```sh
make
```

On macOS with Homebrew, install the dependencies with `brew install sdl2 sdl2_ttf pkgconf`.
On Linux (Debian/Ubuntu), install the required build dependencies with:

```sh
sudo apt update
sudo apt install build-essential pkg-config libsdl2-dev libsdl2-ttf-dev
```

On Windows, use an MSYS2 MinGW environment with the corresponding SDL2, SDL2_ttf,
`pkgconf`, and Make packages; the output is `chess.exe`.

Run the demo with a font file that includes chess-piece glyphs:

```sh
./chess
./chess /path/to/font.ttf
```

When no font path is supplied, the program tries a sensible default for the current platform. A custom font can still be passed explicitly to override the default. On Windows, run `chess.exe` instead of `./chess`.
