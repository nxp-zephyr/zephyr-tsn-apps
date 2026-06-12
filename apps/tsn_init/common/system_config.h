/*
 * Copyright 2018-2026 NXP
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
    uint8_t ip_addr[4];
    uint8_t net_mask[4];
    uint8_t gw_addr[4];
};

struct system_config {
    struct net_config system_net_cfg[CONFIG_APP_LOGICAL_PORTS];
};

extern struct system_config system_cfg;

void __system_config_get_net(const char *prefix, unsigned int port_id, struct net_config *net);
struct net_config *system_config_get_net(unsigned int port_id);

#endif /* _SYSTEM_CONFIG_H_ */
