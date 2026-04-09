# RTOS abstraction layer configuration
set(RTOS_ABSTRACTION_LAYER_DIR "${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr")

set(GenAVBPath "${ZEPHYR_BASE}/../gen_avb")
set(GenAVBBuildPath "${CMAKE_CURRENT_BINARY_DIR}/gen_avb_build")
set(MCUX_SDK "${ZEPHYR_BASE}/../modules/hal/nxp/mcux/mcux-sdk-ng")
set(RTOS_DIR "${RTOS_ABSTRACTION_LAYER_DIR}")

if(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33)
  set(TARGET "zephyr_rt1189_cm33")
  set(CONFIG "hybrid_tsn_basic")

if(CONFIG_CODE_DATA_RELOCATION)
  # workaround to have switch descriptors in non cacheable memory
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_netc_sw.c LOCATION DTCM_DATA_RODATA_BSS_NOINIT)
endif()
else()
  message(ERROR "unsupported")
endif()

set(RTOS_APPS "${CMAKE_CURRENT_LIST_DIR}/src")
set(APP_GENAVB_SDK_INCLUDE ${RTOS_APPS}/boards/${BoardDir})

# Define an RTOS network buffer heap and place it in DTCM non-cacheable memory
add_compile_definitions("-DCONFIG_RTOS_NET_HEAP_SIZE=40960")
if(CONFIG_CODE_DATA_RELOCATION)
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr/rtos_net_heap.c LOCATION DTCM_DATA_NOINIT)
endif()

add_subdirectory(${GenAVBPath} "${GenAVBBuildPath}")

zephyr_include_directories(
  ${GenAVBPath}/include
  ${GenAVBPath}/include/rtos
)

target_sources(app PRIVATE
  src/boards/${BoardDir}/genavb_sdk.c
)

zephyr_compile_definitions_ifdef(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM33 FSL_ETH_ENABLE_CACHE_CONTROL)

add_library(avb-core-lib STATIC IMPORTED)
set_target_properties(avb-core-lib PROPERTIES IMPORTED_LOCATION ${GenAVBBuildPath}/libstack-core.a)
add_dependencies(${MCUX_SDK_PROJECT_NAME} avb-core-lib stack-rtos)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE avb-core-lib)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE stack-rtos)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
