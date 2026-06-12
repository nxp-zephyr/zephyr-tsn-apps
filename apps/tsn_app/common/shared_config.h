/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * m7 shared configuration
 */
#ifndef _SHARED_CONFIG_H_
#define _SHARED_CONFIG_H_

#include "system_config.h"
#include "gavb_stack.h"
#include "rtos_apps/tsn/tsn_entry.h"

#define CONFIG_APP_LOGICAL_PORTS_M7 1

#if defined(CONFIG_TSN_MULTICORE_SECONDARY) && (CONFIG_APP_LOGICAL_PORTS_M7 != CONFIG_APP_LOGICAL_PORTS)
#error "CONFIG_APP_LOGICAL_PORTS_M7 must match CONFIG_APP_LOGICAL_PORTS in the secondary core build"
#endif

struct system_config_m7 {
    struct net_config net[CONFIG_APP_LOGICAL_PORTS_M7];
    union {
        struct rtos_apps_tsn_config tsn_app_config;
    } app;
};

#if defined(CONFIG_TSN_MULTICORE)
void shared_system_config_set(void);
void shared_system_config_get(void);
#else
static inline void shared_system_config_set(void)
{
    return;
}

static inline void shared_system_config_get(void)
{
    return;
}
#endif

#endif /* _SHARED_CONFIG_H_ */
