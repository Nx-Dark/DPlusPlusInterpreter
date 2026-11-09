
#!/bin/bash

set -e

cd "$(dirname "$0")/.."

if ! command -v premake5 &> /dev/null; then
    echo "ERROR: Premake5 is not installed."
    exit 1
fi

if ! command -v gmake &> /dev/null; then
    echo "ERROR: GNU Make is not installed."
    exit 1
fi

if ! command -v clang++ &> /dev/null; then
    echo "ERROR: Clang is not installed."
    exit 1
fi

echo ""
echo "[1/2] Generating macOS Makefiles..."

premake5 gmake

echo ""
echo "[2/2] Compiling D++ in Release mode..."

gmake config=release -j"$(sysctl -n hw.ncpu)"
