#!/usr/bin/env sh
set -e

# Build the binary.
if command -v make >/dev/null 2>&1 && [ -f "Makefile" ]; then
  make
else
  CC=${CC:-cc}
  CFLAGS=${CFLAGS:--Wall -Wextra -O2}
  "$CC" $CFLAGS -o org org.c
fi

# Install the binary.
if [ -n "${NO_INSTALL:-}" ]; then
  echo "Built ./org"
  exit 0
fi

PREFIX=${PREFIX:-/usr/local}
BINDIR=${BINDIR:-"$PREFIX/bin"}

if [ ! -w "$BINDIR" ]; then
  if command -v sudo >/dev/null 2>&1; then
    sudo mkdir -p "$BINDIR"
    sudo install -m 0755 org "$BINDIR/org"
    echo "Installed to $BINDIR/org"
    exit 0
  fi
  BINDIR="${HOME}/.local/bin"
fi

mkdir -p "$BINDIR"
install -m 0755 org "$BINDIR/org"

if ! command -v org >/dev/null 2>&1; then
  echo "Installed to $BINDIR/org"
  echo "Add $BINDIR to PATH to use 'org' from the terminal."
else
  echo "Installed to $BINDIR/org"
fi
