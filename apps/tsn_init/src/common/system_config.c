/*
 * Copyright 2018-2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gavb_stack.h"
#include "system_config.h"

extern struct net_config system_net_cfg[CONFIG_APP_LOGICAL_PORTS];

struct net_config *system_config_get_net(unsigned int port_id)
{
    if (port_id >= CONFIG_APP_LOGICAL_PORTS)
        goto err;

    return &system_net_cfg[port_id];

err:
    return NULL;
}
