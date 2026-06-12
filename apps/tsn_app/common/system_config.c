/*
 * Copyright 2018-2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_apps/storage/storage.h"

#include "genavb/helpers.h"

#include "system_config.h"
#include "storage_config.h"

void __system_config_get_net(const char *prefix, unsigned int port_id, struct net_config *net)
{
    char port[10 + sizeof(CONFIG_STORAGE_ROOT)];

    if (h_snprintf_strict(port, 10 + sizeof(CONFIG_STORAGE_ROOT), "%s%u", prefix, port_id) < 0)
        goto out;

    storage_read_mac_address(port, "hw_addr", net[port_id].hw_addr);
    storage_read_ipv4_address(port, "ip_addr", net[port_id].ip_addr);
    storage_read_ipv4_address(port, "net_mask", net[port_id].net_mask);
    storage_read_ipv4_address(port, "gw_addr", net[port_id].gw_addr);

out:
    return;
}

struct net_config *system_config_get_net(unsigned int port_id)
{
    if (port_id >= CONFIG_APP_LOGICAL_PORTS)
        goto err;

    __system_config_get_net(CONFIG_STORAGE_ROOT "/port", port_id, system_net_cfg);

    return &system_net_cfg[port_id];

err:
    return NULL;
}
