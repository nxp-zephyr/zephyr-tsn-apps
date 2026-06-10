/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include <zephyr/kernel.h>

#include "gavb_stack.h"
#include "board.h"
#include "shared_config.h"

#include <zephyr/shell/shell.h>
#include "genavb_shell.h"
#include "init_sync.h"
#include "networking_config.h"
#include "rtos_apps/storage.h"
#include "zephyr/shell/shell_uart.h"

#define TSN_INIT_STACK_SIZE 4096
#define TSN_INIT_STACK_PRIO (K_LOWEST_THREAD_PRIO - 4)

K_THREAD_STACK_DEFINE(tsn_init_stack, TSN_INIT_STACK_SIZE);
struct k_thread tsn_init_thread;

static void tsn_init_main(void *p1, void *p2, void *p3)
{
    void *shell;
    int rc;

    printk("Starting GenAVB/TSN stack: enter\n");

    shell = (void *)shell_backend_uart_get_ptr();

    storage_init();

    storage_set_shell(shell);

    rc = gavb_stack_init();
    if (rc < 0)
        goto exit;

    printk("Starting GenAVB/TSN stack: success\n");

    genavb_shell_init();

    network_start();

    shared_system_config_set();

#if defined(CONFIG_TSN_MULTICORE_PRIMARY)
    if (init_sync_signal_stack_ready() < 0) {
        printk("init_sync_signal_stack_ready() failed\n");
    }
#endif

    return;

exit:
    printk("Starting GenAVB/TSN stack: failed\n");
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
