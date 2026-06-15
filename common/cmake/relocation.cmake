# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE ZEPHYR_KERNEL_SOURCES
  ${ZEPHYR_BASE}/kernel/*.c
)

set(ZEPHYR_ARCH_SOURCES
  ${ZEPHYR_BASE}/arch/arm/core/cortex_m/cpu_idle.c
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

file(GLOB_RECURSE ZEPHYR_DRIVER_SOURCES
  ${ZEPHYR_BASE}/drivers/*.c
)

file(GLOB_RECURSE ZEPHYR_SUBSYS_SOURCES
  ${ZEPHYR_BASE}/subsys/logging/*.c
  ${ZEPHYR_BASE}/subsys/net/*.c
  ${ZEPHYR_BASE}/subsys/random/*.c
  ${ZEPHYR_BASE}/subsys/shell/*.c
)

file(GLOB_RECURSE MCU_SDK_SOURCES
  ${ZEPHYR_BASE}/../modules/hal/nxp/mcux/mcux-sdk-ng/*.c
)

file(GLOB_RECURSE RTOS_ABSTRACTION_SOURCES
  ${ZEPHYR_BASE}/../rtos-abstraction-layer/zephyr/*.c
)

# rtos_net_heap.c contains, and handle, RTOS network buffer heap: should be handled/relocated separately
list(FILTER RTOS_ABSTRACTION_SOURCES EXCLUDE REGEX ".*/rtos_net_heap.c$")

# Do not relocate system init code
list(FILTER MCU_SDK_SOURCES EXCLUDE REGEX ".*/system_.*\.c")

# Select all sections except .text.init and .text.exit
set(SECTION_F_N_INIT_EXIT "^(?!\\.text\\.(init|exit)$).*")

# Select all .bss sections except .bss.netc_sw_drivers
set(SECTION_F_N_NETC_SW "^\\.bss\\.(?!netc_sw_drivers$).*")
