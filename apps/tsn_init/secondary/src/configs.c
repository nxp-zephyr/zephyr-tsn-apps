/*
 * Copyright 2022, 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gavb_stack.h"
#include "system_config.h"

/* Use a different default mac address than the primary core. */
#define MAC_ADDR_BYTE_1 0xEF

struct net_config system_net_cfg[CONFIG_APP_LOGICAL_PORTS] = {
    [0] = {
        .hw_addr = {0x00, 0xBB, 0xCC, 0xDD, MAC_ADDR_BYTE_1, 0x00},
    },
#if CONFIG_APP_LOGICAL_PORTS > 1
    [1] = {
        .hw_addr = {0x00, 0xBB, 0xCC, 0xDD, MAC_ADDR_BYTE_1, 0x01},
    },
#endif
};
