if (CONFIG_SYS_HEAP_STATS_PERIODIC_LOG)
  target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/sys_heap_stats.c)
endif()
