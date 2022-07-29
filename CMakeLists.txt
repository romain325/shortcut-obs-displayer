cmake_minimum_required(VERSION 3.19)
project(keyboard_listener C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(PkgConfig)
find_package(LIBEVDEV REQUIRED)

include_directories(${LIBEVDEV_INCLUDE_DIRS})

#[[pkg_check_modules(PC_LIBEVDEV REQUIRED libevdev>=1.0)

include_directories(${PC_LIBEVDEV_INCLUDE_DIRS})
link_directories(${PC_LIBEVDEV_LIBRARY_DIRS})
add_definitions(${PC_LIBEVDEV_CFLAGS_OTHER})]]

add_executable(keyboard_listener main.c)
target_link_libraries(keyboard_listener ${LIBEVDEV_LIBRARY})