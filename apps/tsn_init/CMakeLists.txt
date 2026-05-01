# SPDX-License-Identifier: BSD-3-Clause

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

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tsn_init)
set(MCUX_SDK_PROJECT_NAME app)

SET(HAL_NXP_PATH "${ZEPHYR_BASE}/../modules/hal/nxp")
SET(SdkRootDirPath "${HAL_NXP_PATH}/mcux/mcux-sdk-ng")

include(../../common/libs/sys_heap_stats/sys_heap_stats.cmake)
include(../../common/libs/genavb_shell/genavb_shell.cmake)

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

include(genavb_sdk.cmake)

if(CONFIG_CODE_DATA_RELOCATION)
  file(GLOB_RECURSE ZEPHYR_KERNEL_SOURCES
    ${ZEPHYR_BASE}/kernel/*.c
  )

  set(ZEPHYR_ARCH_SOURCES
    ${ZEPHYR_BASE}/arch/arm/core/cortex_m/irq_manage.c
    ${ZEPHYR_BASE}/arch/arm/core/cortex_m/swap_helper.S
    ${ZEPHYR_BASE}/arch/arm/core/cortex_m/thread.c
    ${ZEPHYR_BASE}/arch/common/sw_isr_common.c
  )

  if(CONFIG_CPU_CORTEX_M_HAS_VTOR)
    list(APPEND ZEPHYR_ARCH_SOURCES
      ${ZEPHYR_BASE}/arch/arm/core/cortex_m/exc_exit.c
      ${ZEPHYR_BASE}/arch/arm/core/cortex_m/isr_wrapper.c
    )
  endif()

  file(GLOB_RECURSE ZEPHYR_LIB_SOURCES
    ${ZEPHYR_BASE}/lib/*.c
  )

  file(GLOB_RECURSE ZEPHYR_NON_KERNEL_SOURCES
    ${ZEPHYR_BASE}/drivers/*.c
    ${ZEPHYR_BASE}/subsys/logging/*.c
    ${ZEPHYR_BASE}/subsys/shell/*.c
  )

  file(GLOB_RECURSE ZEPHYR_NET_SOURCES
    ${ZEPHYR_BASE}/subsys/net/*.c
  )

  file(GLOB_RECURSE APPS_SOURCES
    ${ZEPHYR_BASE}/../zephyr-tsn-apps/apps/tsn_init/src/*.c
    ${ZEPHYR_BASE}/../zephyr-tsn-apps/common/*.c
    ${ZEPHYR_BASE}/../zephyr-tsn-apps/rtos-apps/*.c
  )

  # Do not relocate early_mem.c file used for early memcpy and memset
  list(FILTER APPS_SOURCES EXCLUDE REGEX ".*/early_mem.c$")

  file(GLOB_RECURSE MCU_SDK_SOURCES
    ${ZEPHYR_BASE}/../modules/hal/nxp/mcux/mcux-sdk-ng/*.c
  )

  # Do not relocate system init code
  list(FILTER MCU_SDK_SOURCES EXCLUDE REGEX ".*/system_.*\.c")

  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION ITCM1_DATA_BSS NOKEEP)
  # NOINIT catches the system heap memory pool
  zephyr_code_relocate(FILES ${ZEPHYR_KERNEL_SOURCES} ${ZEPHYR_ARCH_SOURCES} ${ZEPHYR_LIB_SOURCES} LOCATION DTCM_NOINIT NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_NON_KERNEL_SOURCES} ${APPS_SOURCES} ${MCU_SDK_SOURCES} LOCATION RAM NOKEEP)
  zephyr_code_relocate(FILES ${ZEPHYR_NET_SOURCES} LOCATION hyperram0 NOKEEP)

endif()

zephyr_linker_sources(ITCM_SECTION ../../common/linker/libc_itcm.ld)

zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_FP)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_QBV)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_TSN_BRIDGE)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_NUM_PORTS=2)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_LOGICAL_PORT_LIST={0,1})

set(RTOS_ABSTRACTION_LAYER_TARGET app)
include(${RTOS_ABSTRACTION_LAYER_DIR}/rtos_abstraction_layer.cmake)

set(RtosAppsPath "${ZEPHYR_BASE}/../rtos-apps")
set(RTOS_APPS_TARGET app)
include(${RtosAppsPath}/rtos_apps_log.cmake)
include(${RtosAppsPath}/rtos_apps_shell.cmake)

target_sources(app PRIVATE
  src/boards/${BoardDir}/board.c
  src/boards/${BoardDir}/clock_config.c
  src/boards/${BoardDir}/pin_mux.c
  src/common/configs.c
  src/common/gavb_stack.c
  src/common/genavbtsn_eth.c
  src/common/system_config.c
  src/main.c
  ../../common/src/early_mem.c
)

zephyr_include_directories(
  src/common
  src/boards/${BoardDir}
)
