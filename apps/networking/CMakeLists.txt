cmake_minimum_required(VERSION 3.20.0)

# Use NEWLIB as libc to make it work with ARM GNU Toolchain out of the box.
set(TOOLCHAIN_HAS_NEWLIB ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(networking)

target_sources(app PRIVATE src/main.c)
