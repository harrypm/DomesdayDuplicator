#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Build script for DomesdayDuplicator using system FFmpeg libraries
# This ensures we don't accidentally use brew or other non-system dependencies
# Preflight dependency check for Debian/Ubuntu systems
if command -v dpkg >/dev/null 2>&1; then
    REQUIRED_PACKAGES=(
        build-essential
        cmake
        pkg-config
        libusb-1.0-0-dev
        libswresample-dev
        libavutil-dev
        libflac-dev
        qt6-base-dev
        libqt6serialport6-dev
    )

    MISSING_PACKAGES=()
    for package in "${REQUIRED_PACKAGES[@]}"; do
        if ! dpkg -s "$package" >/dev/null 2>&1; then
            MISSING_PACKAGES+=("$package")
        fi
    done

    if [ ${#MISSING_PACKAGES[@]} -gt 0 ]; then
        echo "Missing build dependencies: ${MISSING_PACKAGES[*]}"
        echo "Install with: sudo apt update && sudo apt install -y ${MISSING_PACKAGES[*]}"
        exit 1
    fi
fi

# Set PKG_CONFIG_PATH to prioritize system libraries
export PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/share/pkgconfig"

# Print which versions we're using
echo "=== Using System FFmpeg Libraries ==="
pkg-config --modversion libswresample libavutil
pkg-config --libs libswresample libavutil
echo "======================================"

# Clean previous build
rm -rf build

# Configure with cmake
echo "Configuring build..."
cmake -B build -S .

# Build
echo "Building..."
cmake --build build -j$(nproc)

# Verify system library linkage
echo "=== Verifying Library Linkage ==="
ldd build/DomesdayDuplicator/DomesdayDuplicator | grep -E "(swresample|avutil|FLAC)"
echo "================================="

echo "Build complete! Executable: build/DomesdayDuplicator/DomesdayDuplicator"
