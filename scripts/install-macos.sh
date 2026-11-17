#!/usr/bin/env bash
set -eu

if ! command -v brew >/dev/null 2>&1; then
  echo "Homebrew is required. Install it from https://brew.sh/" >&2
  exit 1
fi

brew install sdl2 sdl2_ttf pkgconf

echo "Dependencies installed. Build with:"
echo "  make"
