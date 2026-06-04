# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

set(AppRootPath ${CMAKE_CURRENT_SOURCE_DIR})
set(AppCommonPath "${AppRootPath}/../common")
set(CommonPath "${AppRootPath}/../../../common")

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE
  "${CommonPath}/configs/base_secondary.conf"
  "${CommonPath}/configs/logging.conf"
  "${CommonPath}/configs/runtime-stats.conf"
)

# Load zephyr extensions to get BOARD for sysbuild and regular builds
find_package(Zephyr COMPONENTS extensions basic_settings REQUIRED HINTS $ENV{ZEPHYR_BASE})
zephyr_get(_TARGET_BOARD VAR BOARD)
# Common device tree overlays
if("${_TARGET_BOARD}" STREQUAL "mimxrt1180_evk/mimxrt1189/cm7")
  list(APPEND EXTRA_DTC_OVERLAY_FILE
    "../../../../zephyr/boards/nxp/mimxrt1180_evk/cm7_flash_boot.overlay"
    "${CommonPath}/overlays/nxp_rt118x_tune_interrupt_priorities.overlay"
  )
endif()

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tsn_init_secondary)
set(MCUX_SDK_PROJECT_NAME app)

if(NOT CONFIG_NXP_BOARD_SPECIFIC_MPU_SETTINGS)
  if(CONFIG_SOC_MIMXRT1189_CM7)
    zephyr_sources(${CommonPath}/soc/imxrt118x/cm7/mpu_regions.c)
  endif()
endif()


SET(HAL_NXP_PATH "${ZEPHYR_BASE}/../modules/hal/nxp")
SET(SdkRootDirPath "${HAL_NXP_PATH}/mcux/mcux-sdk-ng")

include(${CommonPath}/libs/sys_heap_stats/sys_heap_stats.cmake)

function(include_mcux_driver driver)
  set(CONFIG_MCUX_COMPONENT_driver.${driver} ON)
  include(${SdkRootDirPath}/drivers/${driver}/CMakeLists.txt)
endfunction()

if(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM7)
  include_mcux_driver(rgpio)
  include_mcux_driver(gpt)
  set(CONFIG_MCUX_COMPONENT_driver.netc_rt1180 ON)
  include_mcux_driver(netc)
  include_mcux_driver(msgintr)
  set(BoardDir "evkmimxrt1180/cm7")
endif()

include (${CommonPath}/cmake/relocation.cmake)

include(genavb_sdk.cmake)

if(CONFIG_CODE_DATA_RELOCATION)

  file(GLOB_RECURSE APPS_SOURCES
    ${AppCommonPath}/*.c
    ${AppRootPath}/src/*.c
    ${CommonPath}/*.c
  )

  # Do not relocate early_mem.c file used for early memcpy and memset
  list(FILTER APPS_SOURCES EXCLUDE REGEX ".*/early_mem.c$")

  # NOINIT catches the system heap memory pool
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION DTCM_DATA_BSS_NOINIT NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)

  zephyr_code_relocate(FILES ${ZEPHYR_DRIVER_SOURCES} ${ZEPHYR_SUBSYS_SOURCES} ${APPS_SOURCES} ${MCU_SDK_SOURCES} LOCATION DTCM_DATA_BSS NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_DRIVER_SOURCES} ${ZEPHYR_SUBSYS_SOURCES} ${APPS_SOURCES} ${MCU_SDK_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)

endif()

zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/libc_itcm.ld)
zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/fsl_mem_ops.ld)
zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/sw_isr_table.ld)

zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM7 CONFIG_APP_TSN_ENDPOINT)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM7 CONFIG_APP_EP_NUM_PORTS=1)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM7 CONFIG_APP_EP_LOGICAL_PORT_LIST={0})

set(RTOS_ABSTRACTION_LAYER_DIR "${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr")
set(RTOS_ABSTRACTION_LAYER_TARGET app)
include(${RTOS_ABSTRACTION_LAYER_DIR}/rtos_abstraction_layer.cmake)

set(RtosAppsPath "${ZEPHYR_BASE}/../rtos-apps")
set(RTOS_APPS_TARGET app)
# Import storage for stubs
include(${RtosAppsPath}/rtos_apps_storage.cmake)

target_sources(app PRIVATE
  ${AppCommonPath}/gavb_stack.c
  ${AppCommonPath}/system_config.c
  ${CommonPath}/src/early_mem.c
  ${CommonPath}/src/init_sync.c
  src/boards/${BoardDir}/board.c
  src/configs.c
  src/main.c
)

zephyr_include_directories(
  ${AppCommonPath}
  ${CommonPath}/src
  src/boards/${BoardDir}
)
