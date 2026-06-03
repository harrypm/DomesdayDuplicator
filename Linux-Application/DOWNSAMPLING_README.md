# Real-Time Downsampling Support

This version of DomesdayDuplicator includes real-time downsampling support for LaserDisc RF capture, allowing you to capture at reduced sample rates (20 MSPS and 10 MSPS) while maintaining high quality through proper anti-aliasing.

## Dependencies

The capture and downsampling path requires:
- FFmpeg's `libswresample`/`libavutil` for high-quality real-time resampling
- `libFLAC` (v1.5.0 or newer) for native multithreaded FLAC encoding

### Ubuntu/Debian
```bash
sudo apt install libswresample-dev libavutil-dev libflac-dev
```

### Red Hat/CentOS/Fedora
```bash
sudo yum install ffmpeg-devel
# or on newer versions:
sudo dnf install ffmpeg-devel
```

### Windows
FFmpeg libraries should be installed and available in the system PATH.

## Building

Use the provided build script to ensure system libraries are used:

```bash
./build_with_system_ffmpeg.sh
```

Or manually:
```bash
# Set PKG_CONFIG_PATH to prefer system libraries
export PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/share/pkgconfig"

cmake -B build -S .
cmake --build build -j$(nproc)
```

## Downsampling Modes

The application now supports three new capture formats:

1. **16-bit Signed (40 MSPS)** - Original full rate capture
2. **16-bit Signed Half Rate (20 MSPS)** - Real-time 2:1 downsampling 
3. **16-bit Signed Quarter Rate (10 MSPS)** - Real-time 4:1 downsampling

## Technical Details

- **Real-time processing**: Downsampling occurs during capture, not post-processing
- **High quality**: Uses FFmpeg's libswresample with proper anti-aliasing filters
- **Native FLAC encode**: Uses libFLAC multithreaded encoder (default thread count: 80% of detected CPU cores)
- **Memory efficient**: Processes data in chunks without excessive buffering
- **Cross-platform**: Compatible with system FFmpeg/libFLAC libraries on Linux, Windows, and macOS

## Verification

You can verify the resampling is working correctly by checking the test output:
```bash
cd build/DomesdayDuplicator
ldd DomesdayDuplicator | grep -E "swresample|FLAC"
```

Should show system library linkage like:
```
libswresample.so.3 => /lib/x86_64-linux-gnu/libswresample.so.3
libFLAC.so.12 => /lib/x86_64-linux-gnu/libFLAC.so.12
```

## Workflow Testing
This version includes comprehensive GitHub Actions workflows for automated testing and building across all supported platforms. Testing workflow validation now.
