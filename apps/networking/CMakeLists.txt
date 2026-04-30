cmake_minimum_required(VERSION 3.20.0)

# Use NEWLIB as libc to make it work with ARM GNU Toolchain out of the box.
set(TOOLCHAIN_HAS_NEWLIB ON)

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE
  "../../common/configs/base.conf"
  "../../common/configs/logging.conf"
  "../../common/configs/net.conf"
  "../../common/configs/runtime-stats.conf"
  "../../common/configs/shell.conf"
)

# Common device tree overlays
if("${BOARD}" STREQUAL "mimxrt1180_evk/mimxrt1189/cm33")
  list(APPEND EXTRA_DTC_OVERLAY_FILE
    "../../common/overlays/mimxrt1180_evk_mimxrt1189_cm33_ocram1_ram.overlay"
    "../../common/overlays/nxp_rt118x_tune_interrupt_priorities.overlay"
  )
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(networking)

include(../../common/libs/sys_heap_stats/sys_heap_stats.cmake)

target_sources(app PRIVATE src/main.c)
