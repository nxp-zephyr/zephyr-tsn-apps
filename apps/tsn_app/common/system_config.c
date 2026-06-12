/*
 * Copyright 2018-2023, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_apps/storage/storage.h"
#include "rtos_apps/tsn/tsn_tasks_config.h"

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

void __system_config_get_tsn_app(const char *path, struct rtos_apps_tsn_config *tsn)
{
    storage_read_uint(path, "mode", &tsn->mode);
    storage_read_uint(path, "role", &tsn->role);
    storage_read_uint(path, "num_io_devices", &tsn->num_io_devices);
    storage_read_float(path, "motor_offset", &tsn->motor_offset);
    storage_read_uint(path, "control_strategy", &tsn->control_strategy);
    storage_read_uint(path, "cmd_client", &tsn->cmd_client);

    if (tsn->mode == SERIAL)
        tsn->period_ns = APP_PERIOD_SERIAL_DEFAULT;

    storage_read_uint(path, "period_ns", &tsn->period_ns);
    storage_read_uint(path, "offset", &tsn->offset);
    storage_read_uint(path, "network_budget_ns", &tsn->network_budget_ns);
    storage_read_uint(path, "priority", &tsn->priority);
    storage_read_uint(path, "tx_time_offset_ns", &tsn->tx_time_offset_ns);
    storage_read_bool(path, "tx_time_enabled", &tsn->tx_time_enabled);
    storage_read_uint(path, "port_id", &tsn->port_id);
    storage_read_uint(path, "packets", &tsn->packets);
    storage_read_bool(path, "zero_copy", &tsn->zero_copy);
    storage_read_uint(path, "rx_tc_mask", &tsn->rx_tc_mask);
}

struct net_config *system_config_get_net(unsigned int port_id)
{
    if (port_id >= CONFIG_APP_LOGICAL_PORTS)
        goto err;

    __system_config_get_net(CONFIG_STORAGE_ROOT "/port", port_id, system_cfg.system_net_cfg);

    return &system_cfg.system_net_cfg[port_id];

err:
    return NULL;
}
