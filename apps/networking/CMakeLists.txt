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
    "../../common/overlays/nxp_rt118x_cm33_memory.overlay"
    "../../common/overlays/nxp_rt118x_tune_interrupt_priorities.overlay"
  )
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(networking)

include(../../common/libs/sys_heap_stats/sys_heap_stats.cmake)

if(CONFIG_CODE_DATA_RELOCATION)
  include (../../common/relocation.cmake)

  file(GLOB_RECURSE APPS_SOURCES
    ${ZEPHYR_BASE}/../zephyr-tsn-apps/apps/networking/src/*.c
    ${ZEPHYR_BASE}/../zephyr-tsn-apps/common/*.c
  )

  # Do not relocate early_mem.c file used for early memcpy and memset
  list(FILTER APPS_SOURCES EXCLUDE REGEX ".*/early_mem.c$")

  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM1_DATA_BSS NOKEEP)
  # NOINIT catches the system heap memory pool
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION DTCM_NOINIT NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_DRIVER_SOURCES} ${ZEPHYR_SUBSYS_SOURCES} ${APPS_SOURCES} ${MCU_SDK_SOURCES} LOCATION RAM NOKEEP)

endif()

zephyr_linker_sources(ITCM_SECTION ../../common/linker/libc_itcm.ld)

target_sources(app PRIVATE
  src/main.c
  ../../common/src/early_mem.c
)
