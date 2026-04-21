target_sources(app PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/genavb_fdb.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_fp.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_frer.c
  # ${CMAKE_CURRENT_LIST_DIR}/genavb_hsr.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_psfp.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_qbv.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_shell.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_stream_identification.c
  ${CMAKE_CURRENT_LIST_DIR}/genavb_vlan.c
)

zephyr_include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
)
