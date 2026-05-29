/*
 * Copyright 2018-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * System configuration
 */
#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include <stdint.h>

#include "gavb_stack.h"

struct net_config {
    uint8_t hw_addr[6];
};

extern struct net_config system_net_cfg[CONFIG_APP_LOGICAL_PORTS];

struct net_config *system_config_get_net(unsigned int port_id);


#endif /* _SYSTEM_CONFIG_H_ */
