/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include <zephyr/kernel.h>

#include "gavb_stack.h"
#include "board.h"
#include "init_sync.h"

#define TSN_INIT_STACK_SIZE 4096
#define TSN_INIT_STACK_PRIO (K_LOWEST_THREAD_PRIO - 4)

K_THREAD_STACK_DEFINE(tsn_init_stack, TSN_INIT_STACK_SIZE);
struct k_thread tsn_init_thread;

static void tsn_init_main(void *p1, void *p2, void *p3)
{
    int rc;

    printk("Starting GenAVB/TSN stack: enter\n");

    rc = gavb_stack_init();
    if (rc < 0)
        goto exit;

    printk("Starting GenAVB/TSN stack: success\n");
    return;

exit:
    printk("Starting GenAVB/TSN stack: failed\n");
}

int main(void)
{
    k_tid_t thread_ret;

#if defined(CONFIG_TSN_MULTICORE_SECONDARY)
    if (init_sync_wait_for_stack_ready() < 0) {
        printk("init_sync_wait_for_stack_ready() failed\n");
        return -1;
    }
#else
#error "CONFIG_TSN_MULTICORE_SECONDARY must be defined"
#endif

    BOARD_Init();
    BOARD_InitNVIC();

    printk("Main: started\n");

    thread_ret = k_thread_create(&tsn_init_thread, tsn_init_stack, TSN_INIT_STACK_SIZE,
                &tsn_init_main, NULL, NULL, NULL,
                TSN_INIT_STACK_PRIO, 0, K_NO_WAIT);

    return 0;
}
