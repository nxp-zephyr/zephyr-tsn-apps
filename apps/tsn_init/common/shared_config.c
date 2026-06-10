/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "genavb/helpers.h"

#include <stdint.h>
#include <string.h>

#include "shared_config.h"
#include "storage_config.h"

#if defined(CONFIG_TSN_MULTICORE_PRIMARY)

#if DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(m7_dtcm_shm))
#define M7_DTCM_SHM_ADDR         DT_REG_ADDR_BY_IDX(DT_NODELABEL(m7_dtcm_shm), 0)
#else
#error "No memory region is available for shared configuration"
#endif

static struct system_config_m7 *shared_system_cfg = (struct system_config_m7 *)((uintptr_t)(M7_DTCM_SHM_ADDR));

extern struct system_config_m7 m7_system_cfg;

static int system_cfg_get_m7_net_params(unsigned int port_id, struct net_config *net)
{
    if (port_id >= CONFIG_APP_LOGICAL_PORTS_M7)
        goto err;

    __system_config_get_net(CONFIG_STORAGE_ROOT "/m7/port", port_id, net);

    return 0;

err:
    return -1;
}

static void system_cfg_get_m7_params(struct system_config_m7 *sys_cfg)
{
    for (int i = 0; i < CONFIG_APP_LOGICAL_PORTS_M7; i++)
        system_cfg_get_m7_net_params(i, sys_cfg->net);
}

void shared_system_config_set(void)
{
    struct system_config_m7 *__sys_cfg = &m7_system_cfg;

    system_cfg_get_m7_params(__sys_cfg);

    memcpy(shared_system_cfg, __sys_cfg, sizeof(*__sys_cfg));
}

#elif defined(CONFIG_TSN_MULTICORE_SECONDARY)

#if DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(dtcm_shm))
#define DTCM_SHM_ADDR         DT_REG_ADDR_BY_IDX(DT_NODELABEL(dtcm_shm), 0)
#else
#error "No memory region is available for shared configuration"
#endif

extern struct net_config system_net_cfg[CONFIG_APP_LOGICAL_PORTS];

void shared_system_config_get(void)
{
    struct system_config_m7 *system_config_m7 = (struct system_config_m7 *)((uintptr_t)(DTCM_SHM_ADDR));
    memcpy(system_net_cfg, (const void *)system_config_m7->net, sizeof(system_config_m7->net));
}
#endif /* CONFIG_TSN_MULTICORE_SECONDARY */
