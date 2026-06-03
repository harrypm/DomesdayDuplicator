cmake_minimum_required(VERSION 3.13)
project(DomesdayDuplicator)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for dependencies

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets SerialPort)
find_package(LibUSB REQUIRED)
find_package(PkgConfig REQUIRED)

# Prioritize system libraries over homebrew/linuxbrew for cross-platform compatibility
if(NOT DEFINED ENV{PKG_CONFIG_PATH})
    if(EXISTS "/usr/lib/x86_64-linux-gnu/pkgconfig")
        set(ENV{PKG_CONFIG_PATH} "/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/share/pkgconfig")
    endif()
endif()

pkg_check_modules(SWRESAMPLE REQUIRED libswresample)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(FLAC REQUIRED flac>=1.5.0)

if(NOT CMAKE_CROSSCOMPILING)
    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_INCLUDES ${FLAC_INCLUDE_DIRS})
    set(CMAKE_REQUIRED_LIBRARIES ${FLAC_LIBRARIES})
    check_cxx_source_runs("
        #include <FLAC/stream_encoder.h>
        int main() {
            FLAC__StreamEncoder* encoder = FLAC__stream_encoder_new();
            if (!encoder) return 1;
            unsigned int status = FLAC__stream_encoder_set_num_threads(encoder, 2);
            FLAC__stream_encoder_delete(encoder);
            return status == FLAC__STREAM_ENCODER_SET_NUM_THREADS_NOT_COMPILED_WITH_MULTITHREADING_ENABLED ? 1 : 0;
        }"
        FLAC_MULTITHREADING_AVAILABLE)
    unset(CMAKE_REQUIRED_INCLUDES)
    unset(CMAKE_REQUIRED_LIBRARIES)
    if(NOT FLAC_MULTITHREADING_AVAILABLE)
        message(FATAL_ERROR "libFLAC must be compiled with multithreading support enabled")
    endif()
endif()

add_subdirectory(DomesdayDuplicator)
add_subdirectory(dddconv)
add_subdirectory(dddutil)
