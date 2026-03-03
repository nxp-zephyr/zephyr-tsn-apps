cmake_minimum_required(VERSION 3.20.0)

# Use NEWLIB as libc to make it work with ARM GNU Toolchain out of the box.
set(TOOLCHAIN_HAS_NEWLIB ON)

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE "../../common/configs/runtime-stats.conf")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(networking)

include(../../common/libs/sys_heap_stats/sys_heap_stats.cmake)

target_sources(app PRIVATE src/main.c)
