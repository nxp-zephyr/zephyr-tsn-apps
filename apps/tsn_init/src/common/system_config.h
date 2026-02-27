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

struct net_config {
    uint8_t hw_addr[6];
    uint8_t ip_addr[4];
    uint8_t net_mask[4];
    uint8_t gw_addr[4];
};

struct net_config *system_config_get_net(unsigned int port_id);


#endif /* _SYSTEM_CONFIG_H_ */