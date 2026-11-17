#!/usr/bin/env bash
set -eu

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
DIST="$ROOT/dist/linux"
mkdir -p "$DIST"

if [ ! -f "$ROOT/chess" ]; then
  echo "Build the project first with: make" >&2
  exit 1
fi

cp "$ROOT/chess" "$DIST/chess"
cp "$ROOT/README.md" "$DIST/README.txt"

( cd "$DIST" && tar -czf "chess-linux.tar.gz" chess README.txt )

echo "Package created at $DIST/chess-linux.tar.gz"
