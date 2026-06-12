/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "networking_config.h"
#include "system_config.h"
#include "shared_config.h"
#include "genavb_shell.h"
#include "gavb_stack.h"
#include "init_sync.h"
#include "tsn_app.h"
#include "storage.h"
#include "board.h"

#include "rtos_apps/tsn/tsn_tasks_config.h"

#define TSN_INIT_STACK_SIZE 4096
#define TSN_INIT_STACK_PRIO (K_LOWEST_THREAD_PRIO - 4)

K_THREAD_STACK_DEFINE(tsn_init_stack, TSN_INIT_STACK_SIZE);
struct k_thread tsn_init_thread;
static struct gavb_pps pps;

static void tsn_init_main(void *p1, void *p2, void *p3)
{
    struct rtos_apps_tsn_config *app_cfg;
    struct cyclic_task_config *c_cfg;
    int rc;

    printk("Starting TSN app: enter\n");

    storage_init();

    app_cfg = system_config_get_tsn_app();
    if (!app_cfg) {
        printk("system_config_get_tsn_app() failed\n");
        goto exit;
    }

    if (gavb_stack_init()) {
        printk("gavb_stack_init() failed\n");
        goto exit;
    }

    c_cfg = tsn_conf_get_cyclic_task(app_cfg->role);
    if (!c_cfg) {
        printk("tsn_conf_get_cyclic_task() failed\n");
        goto err_stack_init;
    }

    if (gavb_pps_init(&pps, c_cfg->params.clk_id) < 0)
        printk("gavb_pps_init() failed: pps timer could not be started\n");

    rc = tsn_app_init(app_cfg);
    if (rc < 0)
        goto err_tsn_init;

    printk("Starting TSN app: success\n");

    genavb_shell_init();

    network_start();

    shared_system_config_set();

#if defined(CONFIG_TSN_MULTICORE_PRIMARY)
    if (init_sync_signal_stack_ready() < 0) {
        printk("init_sync_signal_stack_ready() failed\n");
    }
#endif

    return;

err_tsn_init:
    gavb_pps_exit(&pps);

err_stack_init:
    if (gavb_stack_exit() < 0)
        printk("gavb_stack_exit() failed\n");

exit:
    printk("Starting TSN app: failed\n");

    return;
}

int main(void)
{
    k_tid_t thread_ret;

    BOARD_Init();
    BOARD_InitNVIC();

    printk("Main: started\n");

    thread_ret = k_thread_create(&tsn_init_thread, tsn_init_stack, TSN_INIT_STACK_SIZE,
                &tsn_init_main, NULL, NULL, NULL,
                TSN_INIT_STACK_PRIO, 0, K_NO_WAIT);

    return 0;
}
