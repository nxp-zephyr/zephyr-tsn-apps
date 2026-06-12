/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include <zephyr/kernel.h>

#include "system_config.h"
#include "shared_config.h"
#include "gavb_stack.h"
#include "init_sync.h"
#include "tsn_app.h"
#include "board.h"

#define TSN_INIT_STACK_SIZE 3048
#define TSN_INIT_STACK_PRIO (K_LOWEST_THREAD_PRIO - 4)

K_THREAD_STACK_DEFINE(tsn_init_stack, TSN_INIT_STACK_SIZE);
struct k_thread tsn_init_thread;

static void tsn_init_main(void *p1, void *p2, void *p3)
{
    struct rtos_apps_tsn_config *app_cfg;
    int rc;

    shared_system_config_get();

    printk("Starting TSN app: enter\n");

    app_cfg = system_config_get_tsn_app();
    if (!app_cfg) {
        printk("system_config_get_tsn_app() failed\n");
        goto exit;
    }

    if (gavb_stack_init()) {
        printk("gavb_stack_init() failed\n");
        goto exit;
    }

    rc = tsn_app_init(app_cfg);
    if (rc < 0)
        goto err_tsn_init;

    printk("Starting TSN app: success\n");
    return;

err_tsn_init:
    if (gavb_stack_exit() < 0)
        printk("gavb_stack_exit() failed\n");

exit:
    printk("Starting TSN app: failed\n");

    return;
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
