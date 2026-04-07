# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

# Use NEWLIB as libc to make it work with ARM GNU Toolchain out of the box.
set(TOOLCHAIN_HAS_NEWLIB ON)

# Common configuration fragment files
list(APPEND EXTRA_CONF_FILE "../../common/configs/runtime-stats.conf")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tsn_init)
set(MCUX_SDK_PROJECT_NAME app)

SET(HAL_NXP_PATH "${ZEPHYR_BASE}/../modules/hal/nxp")
SET(SdkRootDirPath "${HAL_NXP_PATH}/mcux/mcux-sdk-ng")

include(../../common/libs/sys_heap_stats/sys_heap_stats.cmake)

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
    if(CONFIG_CPU_CORTEX_M_HAS_VTOR)
      zephyr_code_relocate(FILES ${ZEPHYR_BASE}/arch/arm/core/cortex_m/isr_wrapper.c LOCATION RAM)
      zephyr_code_relocate(FILES ${ZEPHYR_BASE}/arch/arm/core/cortex_m/exc_exit.c LOCATION RAM)
    endif()
endif()

zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_TSN_BRIDGE)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_NUM_PORTS=2)
zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 CONFIG_APP_EP_LOGICAL_PORT_LIST={0,1})

set(RTOS_ABSTRACTION_LAYER_TARGET ${MCUX_SDK_PROJECT_NAME})
include(${RTOS_ABSTRACTION_LAYER_DIR}/rtos_abstraction_layer.cmake)

target_sources(app PRIVATE
  src/boards/${BoardDir}/board.c
  src/boards/${BoardDir}/clock_config.c
  src/boards/${BoardDir}/pin_mux.c
  src/common/configs.c
  src/common/gavb_stack.c
  src/common/system_config.c
  src/main.c
)

zephyr_include_directories(
  src/common
  src/boards/${BoardDir}
)
