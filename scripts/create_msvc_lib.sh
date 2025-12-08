#!/bin/bash
# Script to create MSVC import library from .def file
# Note: This requires MSVC's lib.exe, so it only works on Windows

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEF_FILE="${1:-$ROOT/build-phase4/openmfc.def}"
OUTPUT="${2:-$ROOT/build-phase4/openmfc.lib}"

echo "Creating MSVC import library from: $DEF_FILE"
echo "Output: $OUTPUT"

# Check if we're on Windows with MSVC
if [[ "$OS" != "Windows_NT" ]]; then
    echo "Error: This script requires Windows with MSVC"
    echo "On Linux, we can't create MSVC .lib files"
    echo "The .def file is valid and can be used with:"
    echo "  lib /DEF:$DEF_FILE /OUT:$OUTPUT /MACHINE:X64"
    exit 1
fi

# On Windows with MSVC
lib /DEF:"$DEF_FILE" /OUT:"$OUTPUT" /MACHINE:X64
if [ $? -eq 0 ]; then
    echo "Successfully created MSVC import library: $OUTPUT"
else
    echo "Failed to create MSVC import library"
    exit 1
fi