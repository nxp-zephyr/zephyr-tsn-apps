# RTOS abstraction layer configuration
set(RTOS_ABSTRACTION_LAYER_DIR "${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr")

set(GenAVBPath "${ZEPHYR_BASE}/../gen_avb")
set(GenAVBBuildPath "${CMAKE_CURRENT_BINARY_DIR}/gen_avb_build")
set(MCUX_SDK "${ZEPHYR_BASE}/../modules/hal/nxp/mcux/mcux-sdk-ng")
set(RTOS_DIR "${RTOS_ABSTRACTION_LAYER_DIR}")

if(CONFIG_BOARD_MIMXRT1180_EVK_MIMXRT1189_CM7)
  set(TARGET "zephyr_rt1189_cm7")
  set(CONFIG "endpoint_tsn_no_gptp")

else()
  message(ERROR "Unsupported board")
endif()

set(RTOS_APPS "${CMAKE_CURRENT_LIST_DIR}/src")
set(APP_GENAVB_SDK_INCLUDE ${RTOS_APPS}/boards/${BoardDir})

# Define an RTOS network buffer heap.
add_compile_definitions(CONFIG_RTOS_NET_HEAP_SIZE=40960)

add_subdirectory(${GenAVBPath} "${GenAVBBuildPath}")

zephyr_include_directories(
  ${GenAVBPath}/include
  ${GenAVBPath}/include/rtos
)

target_sources(app PRIVATE
  src/boards/${BoardDir}/genavb_sdk.c
)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE genavb_sdk)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

if(CONFIG_CODE_DATA_RELOCATION)
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_enetc_ep.c FILTER ${SECTION_F_N_INIT_EXIT} LOCATION ITCM_TEXT_RODATA)
  zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../gen_avb/rtos/net_port_enetc_ep.c LOCATION DTCM_BSS_DATA)

  get_target_property(genavbtsn_libs genavb_sdk GENAVB_SDK_LIBRARIES)
  if (NOT genavbtsn_libs)
    message(FATAL_ERROR "No GENAVB_SDK_LIBRARIES property found for genavb_sdk target ")
  endif()

  foreach(genavbtsn_lib IN LISTS genavbtsn_libs)
    # Place GenAVB/TSN in ocram except for init and exit functions
    zephyr_code_relocate(LIBRARY ${genavbtsn_lib} FILTER ${SECTION_F_N_INIT_EXIT} LOCATION DTCM_DATA_BSS_NOINIT NOKEEP)
    zephyr_code_relocate(LIBRARY ${genavbtsn_lib} FILTER ${SECTION_F_N_INIT_EXIT} LOCATION ITCM_TEXT_RODATA NOKEEP)
  endforeach()

  # Place RTOS network buffer heap (NOINIT section) in DTCM non-cacheable memory.
  zephyr_code_relocate(FILES ${RTOS_ABSTRACTION_SOURCES} LOCATION DTCM_DATA_BSS_NOINIT NOKEEP)
  zephyr_code_relocate(FILES ${RTOS_ABSTRACTION_SOURCES} LOCATION ITCM_TEXT_RODATA NOKEEP)

endif()
