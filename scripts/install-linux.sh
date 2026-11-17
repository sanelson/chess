#!/usr/bin/env bash
set -eu

if command -v sudo >/dev/null 2>&1; then
  SUDO=sudo
else
  SUDO=""
fi

if [ -f /etc/debian_version ]; then
  if [ -n "$SUDO" ]; then
    "$SUDO" apt update
    "$SUDO" apt install -y build-essential pkg-config libsdl2-dev libsdl2-ttf-dev
  else
    apt update
    apt install -y build-essential pkg-config libsdl2-dev libsdl2-ttf-dev
  fi
else
  echo "This helper script currently targets Debian/Ubuntu-style Linux systems." >&2
  echo "Install build-essential, pkg-config, libsdl2-dev, and libsdl2-ttf-dev manually." >&2
  exit 1
fi

echo "Dependencies installed. Build with:"
echo "  make"
