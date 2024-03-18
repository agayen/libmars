cmake_minimum_required(VERSION 3.25)

project(libmars CXX)

include(bare)
add_bare_module(libmars)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GST_BASE REQUIRED gstreamer-base-1.0)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GST_INCLUDE_DIRS} ${GST_BASE_INCLUDE_DIRS})

#glib added
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

# Source files
set(files
  src/callback-sink.c
  src/callback-sink.h
  src/chunker.c
  src/chunker.h
)

target_sources(
  ${libmars}
  PRIVATE
    ${files}
    src/binding.cpp
)

target_link_libraries(${libmars} PRIVATE ${GLIB_LDFLAGS})