# chess
Baby's first chess game in C

## Build

Requires a C compiler, GNU Make, `pkg-config` (or `pkgconf`), SDL2, and SDL2_ttf.

```sh
make
```

### Platform install helpers

The project includes simple install scripts under `scripts/` to keep the Unix and Windows setup paths small and easy to maintain:

```sh
./scripts/install-macos.sh
./scripts/install-linux.sh
powershell -ExecutionPolicy Bypass -File ./scripts/install-windows.ps1
```

### macOS

With Homebrew:

```sh
brew install sdl2 sdl2_ttf pkgconf
```

### Linux (Debian/Ubuntu)

```sh
sudo apt update
sudo apt install build-essential pkg-config libsdl2-dev libsdl2-ttf-dev
```

### Windows

The simplest supported setup is to install MSYS2 with `winget`, then add the MinGW build toolchain and SDL2 dev packages from the MSYS2 package manager:

```powershell
winget install --id MSYS2.MSYS2 -e --accept-source-agreements --accept-package-agreements
```

Then open the MSYS2 shell and install the required packages:

```sh
pacman -Syu --noconfirm
pacman -S --needed --noconfirm \
  base-devel \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-pkgconf \
  mingw-w64-x86_64-SDL2 \
  mingw-w64-x86_64-SDL2_ttf
```

Build from the MSYS2 MinGW shell (for example, `mingw64.exe`):

```sh
cd /c/path/to/chess
make CC=gcc
./chess.exe
```

This produces a Windows executable named `chess.exe`. If the default font lookup fails, pass a font path explicitly, for example:

```sh
./chess.exe C:/Windows/Fonts/seguisym.ttf
```

### Packaging

The project also keeps a minimal platform packaging flow separate from the game code:

```sh
./scripts/package-macos.sh
./scripts/package-linux.sh
powershell -ExecutionPolicy Bypass -File ./scripts/package-windows.ps1
```

These scripts package the native build for each OS without introducing a new runtime stack or .NET layer.

#### Windows packaging notes

A native MinGW build is still a Windows executable, but it depends on a few runtime DLLs from the MSYS2 toolchain when it is launched outside the build environment. For a clean end-user package, include these alongside `chess.exe` in the same folder:

- `SDL2.dll`
- `SDL2_ttf.dll`
- `libgcc_s_seh-1.dll`
- `libstdc++-6.dll`

The included `scripts/package-windows.ps1` helper copies these into a bundled folder and creates a ZIP archive. This keeps the game easy to run without making the user install MSYS2.

Run the demo with a font file that includes chess-piece glyphs:

```sh
./chess
./chess /path/to/font.ttf
```

When no font path is supplied, the program tries a sensible default for the current platform. A custom font can still be passed explicitly to override the default. On Windows, run `chess.exe` instead of `./chess`.
