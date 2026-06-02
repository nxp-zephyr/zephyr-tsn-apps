cmake_minimum_required(VERSION 3.20.0)

set(AppRootPath ${CMAKE_CURRENT_SOURCE_DIR})
set(CommonPath "${AppRootPath}/../../common")

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE
  "${CommonPath}/configs/base.conf"
  "${CommonPath}/configs/logging.conf"
  "${CommonPath}/configs/net.conf"
  "${CommonPath}/configs/runtime-stats.conf"
  "${CommonPath}/configs/shell.conf"
)

# Common device tree overlays
if("${BOARD}" STREQUAL "mimxrt1180_evk/mimxrt1189/cm33")
  list(APPEND EXTRA_DTC_OVERLAY_FILE
    "${CommonPath}/overlays/nxp_rt118x_cm33_memory.overlay"
    "${CommonPath}/overlays/nxp_rt118x_tune_interrupt_priorities.overlay"
  )
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(networking)

include(${CommonPath}/libs/sys_heap_stats/sys_heap_stats.cmake)

if(CONFIG_CODE_DATA_RELOCATION)
  include (${CommonPath}/cmake/relocation.cmake)

  file(GLOB_RECURSE APPS_SOURCES
    ${AppRootPath}/src/*.c
    ${CommonPath}/*.c
  )

  # Do not relocate early_mem.c file used for early memcpy and memset
  list(FILTER APPS_SOURCES EXCLUDE REGEX ".*/early_mem.c$")

  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM1_DATA_BSS NOKEEP)
  # NOINIT catches the system heap memory pool
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION DTCM_NOINIT NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_DRIVER_SOURCES} ${ZEPHYR_SUBSYS_SOURCES} ${APPS_SOURCES} ${MCU_SDK_SOURCES} LOCATION RAM NOKEEP)

endif()

zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/libc_itcm.ld)
zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/fsl_mem_ops.ld)

target_sources(app PRIVATE
  src/main.c
  ${CommonPath}/src/early_mem.c
)
