# XCPlite
# cmake_minimum_required(VERSION 3.5)

set(MACOS FALSE) # Build for macOS Darwin ARM
set(WINDOWS FALSE) # Build for Windows x64
set(LINUX TRUE) # Build for Linus x64

if (MACOS) # Not working yet

  set(CMAKE_TOOLCHAIN_FILE "darwin-arm-toolchain.cmake")

endif ()




# project(XCPlite VERSION 6.0 LANGUAGES C)
include_directories(.)
include_directories(./../src)

set(XCPlite_SOURCES xcp_main.c xcpAppl.c ../src/xcpLite.c ../src/xcpEthTl.c ../src/A2L.c ../src/platform.c )
set_source_files_properties(${XCPlite_SOURCES} PROPERTIES LANGUAGE C)
add_library(XCP STATIC ${XCPlite_SOURCES})




# compile*.c as C++
# file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/../*.c")
# set_source_files_properties(${CFILES} PROPERTIES LANGUAGE CXX)

if (WINDOWS)

else ()

  # set(THREADS_PREFER_PTHREAD_FLAG ON)
  # find_package(Threads REQUIRED)
  # target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

  # set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".out")
  # target_link_libraries(${PROJECT_NAME} PRIVATE m)

endif ()
