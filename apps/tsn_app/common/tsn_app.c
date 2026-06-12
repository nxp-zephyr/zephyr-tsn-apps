/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "system_config.h"
#include "gavb_stack.h"
#include "tsn_app.h"

#include "rtos_abstraction_layer.h"

#include "rtos_apps/types.h"
#include "rtos_apps/async.h"
#include "rtos_apps/log.h"

#include <stdio.h>

#define STATS_TASK_NAME         "TSN app stats"
#define STATS_TASK_STACK_SIZE   (RTOS_MINIMAL_STACK_SIZE + 185)
#define STATS_TASK_PRIORITY     5
#define STATS_PERIOD_MS         2000

static struct tsn_app_ctx *tsn_ctx;

static void app_log_update_time(genavb_clock_id_t clk_id)
{
    uint64_t log_time;

    if (genavb_clock_gettime64(clk_id, &log_time) < 0)
        return;

#if defined(CONFIG_RTOS_APPS_LOG_TIMESTAMP)
    rtos_apps_log_timestamp_update(log_time / NSECS_PER_SEC);
#endif
}

int tsn_app_init(struct rtos_apps_tsn_config *app_cfg)
{
    struct rtos_apps_async_config async_cfg = {
        .name = STATS_TASK_NAME,
        .stack_size = STATS_TASK_STACK_SIZE,
        .priority = STATS_TASK_PRIORITY,
        .func = NULL,
        .data = NULL,
        .period_ms = STATS_PERIOD_MS,
    };

    app_cfg->log_update_time = &app_log_update_time;
    app_cfg->async = rtos_apps_async_init(&async_cfg);

    if (rtos_apps_tsn_init(app_cfg, &tsn_ctx) < 0) {
        log_err("rtos_apps_tsn_init() failed\n");
        goto exit;
    }

    return 0;

exit:
    return -1;
}

void tsn_app_exit(void)
{
    rtos_apps_tsn_exit(tsn_ctx);

    log_info("TSN app: exited\n");
}
