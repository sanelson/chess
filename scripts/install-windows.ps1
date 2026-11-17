$ErrorActionPreference = "Stop"

Write-Host "Installing MSYS2..."
winget install --id MSYS2.MSYS2 -e --accept-source-agreements --accept-package-agreements

Write-Host ""
Write-Host "Now open the MSYS2 MinGW64 shell and run:"
Write-Host "  pacman -Syu --noconfirm"
Write-Host "  pacman -S --needed --noconfirm base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-pkgconf mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf"
Write-Host ""
Write-Host "Then build from the repo root with:"
Write-Host "  make CC=gcc"
