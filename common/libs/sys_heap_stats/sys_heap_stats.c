/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#if defined(CONFIG_RTOS_NET_HEAP_SIZE) && (CONFIG_RTOS_NET_HEAP_SIZE > 0)
#include "rtos_abstraction_layer.h"
#endif

LOG_MODULE_REGISTER(sys_heap_stats);

#if defined(K_HEAP_MEM_POOL_SIZE) && (K_HEAP_MEM_POOL_SIZE > 0)
extern struct k_heap _system_heap;
#else
#error "CONFIG_SYS_HEAP_STATS_PERIODIC_LOG enabled but system heap not available"
#endif

static void sys_heap_stats_fn(struct k_work *work)
{
    struct sys_memory_stats stats;
    int ret;

    ret = sys_heap_runtime_stats_get(&_system_heap.heap, &stats);

    if (ret == 0) {
        LOG_INF("System Heap          - Free Bytes: %u, Allocated Bytes: %u, Max Allocated Bytes: %u",
                 stats.free_bytes, stats.allocated_bytes, stats.max_allocated_bytes);
    } else {
        LOG_ERR("sys_heap_runtime_stats_get() for system heap failed: %d", ret);
    }

#if defined(CONFIG_RTOS_NET_HEAP_SIZE) && (CONFIG_RTOS_NET_HEAP_SIZE > 0)
    ret = sys_heap_runtime_stats_get(&rtos_net_heap_handle->kheap->heap, &stats);

    if (ret == 0) {
        LOG_INF("Network Buffers Heap - Free Bytes: %u, Allocated Bytes: %u, Max Allocated Bytes: %u",
                 stats.free_bytes, stats.allocated_bytes, stats.max_allocated_bytes);
    } else {
        LOG_ERR("sys_heap_runtime_stats_get() for rtos net heap failed: %d", ret);
    }
#endif
}

static K_WORK_DEFINE(sys_heap_stats_work, sys_heap_stats_fn);

static void sys_heap_timer_expiry_fn(struct k_timer *dummy)
{
    k_work_submit(&sys_heap_stats_work);
}

K_TIMER_DEFINE(sys_heap_timer, sys_heap_timer_expiry_fn, NULL);

static int sys_heap_stats_init(void)
{
#if CONFIG_SYS_HEAP_STATS_PERIODIC_LOG_PERIOD > 0
    k_timer_start(&sys_heap_timer, K_MSEC(CONFIG_SYS_HEAP_STATS_PERIODIC_LOG_PERIOD),
                    K_MSEC(CONFIG_SYS_HEAP_STATS_PERIODIC_LOG_PERIOD));
#endif

    return 0;
}

/* Initialize last as there is no dependency */
SYS_INIT(sys_heap_stats_init, POST_KERNEL, 99);
