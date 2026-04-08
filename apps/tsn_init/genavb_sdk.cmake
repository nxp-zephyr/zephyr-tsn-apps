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
  # Move the Switch GenAVB/TSN driver to TCM. This guarantees that the switch and endpoint desciptors are in non cacheable memory.
  # Exclude the relatively large netc_sw_drivers variable to save DTCM space for the system heap.
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_netc_sw.c FILTER "^\\.bss\\.(?!netc_sw_drivers$).*" LOCATION DTCM_BSS_DATA)
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_netc_sw.c LOCATION ITCM_TEXT_RODATA)

  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_enetc_ep.c LOCATION DTCM_BSS_DATA)
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_enetc_ep.c LOCATION ITCM_TEXT_RODATA)
endif()
else()
  message(ERROR "unsupported")
endif()

set(RTOS_APPS "${CMAKE_CURRENT_LIST_DIR}/src")
set(APP_GENAVB_SDK_INCLUDE ${RTOS_APPS}/boards/${BoardDir})

# Define an RTOS network buffer heap and place it in DTCM non-cacheable memory
add_compile_definitions(CONFIG_RTOS_NET_HEAP_SIZE=40960)
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

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE genavb_sdk)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

if(CONFIG_CODE_DATA_RELOCATION)
  get_target_property(genavbtsn_libs genavb_sdk GENAVB_SDK_LIBRARIES)
  if (NOT genavbtsn_libs)
    message(FATAL_ERROR "No GENAVB_SDK_LIBRARIES property found for genavb_sdk target ")
  endif()

  foreach(genavbtsn_lib IN LISTS genavbtsn_libs)
    zephyr_code_relocate(LIBRARY ${genavbtsn_lib} LOCATION RAM NOKEEP)
  endforeach()
endif()
