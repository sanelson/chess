# chess
Baby's first chess game in C

## Build

Requires a C compiler, GNU Make, `pkg-config` (or `pkgconf`), SDL2, and SDL2_ttf.

```sh
make
```

On macOS with Homebrew, install the dependencies with `brew install sdl2 sdl2_ttf pkgconf`.
On Linux, install the SDL2 and SDL2_ttf development packages for your distribution.
On Windows, use an MSYS2 MinGW environment with the corresponding SDL2, SDL2_ttf,
`pkgconf`, and Make packages; the output is `chess.exe`.

Run the demo with a font file that includes chess-piece glyphs:

```sh
./chess /path/to/font.ttf
```

On Windows, run `chess.exe` instead of `./chess`.
