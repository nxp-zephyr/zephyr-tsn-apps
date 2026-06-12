# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

set(AppRootPath ${CMAKE_CURRENT_SOURCE_DIR})
set(AppCommonPath "${AppRootPath}/common")
set(CommonPath "${AppRootPath}/../../common")

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE
  "${CommonPath}/configs/base.conf"
  "${CommonPath}/configs/logging.conf"
  "${CommonPath}/configs/fs.conf"
  "${CommonPath}/configs/net.conf"
  "${CommonPath}/configs/runtime-stats.conf"
  "${CommonPath}/configs/shell.conf"
)

# Load zephyr extensions to get BOARD for sysbuild and regular builds
find_package(Zephyr COMPONENTS extensions basic_settings REQUIRED HINTS $ENV{ZEPHYR_BASE})
zephyr_get(_TARGET_BOARD VAR BOARD)
# Common device tree overlays
if("${_TARGET_BOARD}" STREQUAL "mimxrt1180_evk/mimxrt1189/cm33")
  list(APPEND EXTRA_DTC_OVERLAY_FILE
    "${CommonPath}/overlays/nxp_rt118x_cm33_memory.overlay"
    "${CommonPath}/overlays/nxp_rt118x_tune_interrupt_priorities.overlay"
  )
endif()

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tsn_app)
set(MCUX_SDK_PROJECT_NAME app)

SET(HAL_NXP_PATH "${ZEPHYR_BASE}/../modules/hal/nxp")
SET(SdkRootDirPath "${HAL_NXP_PATH}/mcux/mcux-sdk-ng")

include(${CommonPath}/libs/sys_heap_stats/sys_heap_stats.cmake)
include(${CommonPath}/libs/genavb_shell/genavb_shell.cmake)

function(include_mcux_driver driver)
  set(CONFIG_MCUX_COMPONENT_driver.${driver} ON)
  include(${SdkRootDirPath}/drivers/${driver}/CMakeLists.txt)
endfunction()

if(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33)
  include_mcux_driver(rgpio)
  include_mcux_driver(gpt)
  set(CONFIG_MCUX_COMPONENT_driver.netc_rt1180 ON)
  set(CONFIG_MCUX_COMPONENT_driver.netc_switch ON)
  include_mcux_driver(netc)
  include_mcux_driver(msgintr)

  set(CONFIG_MCUX_COMPONENT_component.eth_phy_common ON)
  set(CONFIG_MCUX_COMPONENT_component.phyrtl8201 ON)
  set(CONFIG_MCUX_COMPONENT_component.phyrtl8211f ON)
  include(${SdkRootDirPath}/components/phy/CMakeLists.txt)

  set(BoardDir "evkmimxrt1180/cm33")
endif()

include (${CommonPath}/cmake/relocation.cmake)

include(genavb_sdk.cmake)

if(CONFIG_CODE_DATA_RELOCATION)

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
zephyr_linker_sources(ITCM_SECTION ${CommonPath}/linker/sw_isr_table.ld)

zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_FP)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_QBV)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_TSN_BRIDGE)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_NUM_PORTS=2)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_LOGICAL_PORT_LIST={0,1})

set(RTOS_ABSTRACTION_LAYER_DIR "${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr")
set(RTOS_ABSTRACTION_LAYER_TARGET app)
include(${RTOS_ABSTRACTION_LAYER_DIR}/rtos_abstraction_layer.cmake)

set(RtosAppsPath "${ZEPHYR_BASE}/../rtos-apps")
set(RTOS_APPS_TARGET app)
include(${RtosAppsPath}/rtos_apps_log.cmake)
include(${RtosAppsPath}/rtos_apps_shell.cmake)
include(${RtosAppsPath}/rtos_apps_storage.cmake)

target_sources(app PRIVATE
  ${CommonPath}/src/boards/${BoardDir}/clock_config.c
  ${CommonPath}/src/boards/${BoardDir}/pin_mux.c
  ${CommonPath}/src/boards/${BoardDir}/board.c
  ${CommonPath}/src/networking_config.c
  ${CommonPath}/src/genavbtsn_eth.c
  ${CommonPath}/src/gavb_stack.c
  ${CommonPath}/src/early_mem.c
  ${AppCommonPath}/system_config.c
  src/configs.c
  src/main.c
)

if (CONFIG_TSN_MULTICORE)
  target_sources(app PRIVATE
    ${CommonPath}/src/init_sync.c
    ${AppCommonPath}/shared_config.c
  )
endif()

if (CONFIG_APP_STORAGE)
  target_sources(app PRIVATE
    ${CommonPath}/src/storage_app.c
  )
endif()

zephyr_include_directories(
  ${CommonPath}/src/boards/${BoardDir}
  src/boards/${BoardDir}
  ${CommonPath}/src
  ${AppCommonPath}
)
