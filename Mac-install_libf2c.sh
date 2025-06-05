#!/usr/bin/env bash
set -euo pipefail

PREFIX="${PREFIX:-/usr/local}"
if [[ $(uname -m) == "arm64" && ! -d /usr/local ]]; then
  PREFIX="/opt/homebrew"
fi

echo "[libf2c] Target prefix: $PREFIX"
echo "[libf2c] (override: PREFIX=/your/path ./install_libf2c.sh)"

if [[ ! -w "$PREFIX" ]]; then
  echo "[libf2c] Need write access to $PREFIX (try: sudo "$0")"
  exit 1
fi

mkdir -p "$PREFIX/include" "$PREFIX/lib"

command -v curl  >/dev/null || { echo "curl not found";  exit 1; }
command -v make  >/dev/null || { echo "make not found";  exit 1; }
command -v cc    >/dev/null || { echo "C compiler not found (clang?)"; exit 1; }

TMPDIR=$(mktemp -d)
cleanup() { rm -rf "$TMPDIR"; }
trap cleanup EXIT

echo "[libf2c] Downloading source …"
curl -L https://www.netlib.org/f2c/libf2c.zip -o "$TMPDIR/libf2c.zip"

echo "[libf2c] Unpacking …"
unzip -q "$TMPDIR/libf2c.zip" -d "$TMPDIR"

cd "$TMPDIR/libf2c"

echo "[libf2c] Building …"
make -f $TMPDIR/libf2c/makefile.u   # makefile.u = Unix Makefile in archive

if command -v $TMPDIR/libf2c/ranlib >/dev/null; then
  ranlib libf2c.a || true
fi

echo "[libf2c] Installing …"
cp -f $TMPDIR/libf2c/libf2c.a           "$PREFIX/lib/"
cp -f $TMPDIR/libf2c/f2c.h               "$PREFIX/include/"
echo "[libf2c] installed."

