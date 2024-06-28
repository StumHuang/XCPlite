# XCPlite
cmake_minimum_required(VERSION 3.5)

set(MACOS FALSE) # Build for macOS Darwin ARM
set(WINDOWS FALSE) # Build for Windows x64
set(LINUX TRUE) # Build for Linus x64

include_directories(.)
include_directories(./../src)

file(GLOB srcFile "../src/*.c")
set(XCPlite_SOURCES xcp_main.c ../src/xcpAppl.c ../src/xcpLite.c ../src/xcpEthTl.c ../src/A2L.c ../src/platform.c ../src/xcpEthServer.c)
# set(XCPlite_SOURCES xcp_main.c ${srcFile})
set_source_files_properties(${XCPlite_SOURCES} PROPERTIES LANGUAGE C)
add_library(XCP STATIC ${XCPlite_SOURCES})
