/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/logging/log.h>

#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>

#include "genavbtsn_eth.h"

#include "storage_config.h"
#include "system_config.h"

LOG_MODULE_REGISTER(networking_config);

static void __system_config_update_iface(unsigned int port_id)
{
    struct net_if *iface = genavbtsn_eth_get_by_port_id(port_id);
    int rc;

    if (!iface)
        goto exit;

    __system_config_get_net(CONFIG_STORAGE_ROOT "/port", port_id, system_net_cfg);

    /* Bring interface down to set the Link Address */
    if (net_if_is_up(iface)) {
        rc = net_if_down(iface);
        if (rc < 0) {
            LOG_ERR("net_if_down(%u) failed (%d)", port_id, rc);
            goto exit;
        }
    }

    rc = net_if_set_link_addr(iface, system_net_cfg[port_id].hw_addr, sizeof(system_net_cfg[port_id].hw_addr),
                              NET_LINK_ETHERNET);
    if (rc < 0) {
        LOG_ERR("net_if_set_link_addr(%u) failed (%d)", port_id, rc);
        goto exit;
    }

    if (!net_if_ipv4_addr_add(iface, (struct net_in_addr *)&system_net_cfg[port_id].ip_addr, NET_ADDR_MANUAL, 0)) {
        LOG_ERR("net_if_ipv4_addr_add(%u) failed", port_id);
        goto exit;
    }

    if (!net_if_ipv4_set_netmask_by_addr(iface, (struct net_in_addr *)&system_net_cfg[port_id].ip_addr,
                                         (struct net_in_addr *)&system_net_cfg[port_id].net_mask)) {
        LOG_ERR("net_if_ipv4_set_netmask_by_addr(%u) failed", port_id);
        goto exit;
    }

    net_if_ipv4_set_gw(iface, (struct net_in_addr *)&system_net_cfg[port_id].gw_addr);

    rc = net_if_up(iface);
    if (rc < 0) {
        LOG_ERR("net_if_up(%u) failed (%d)", port_id, rc);
        goto exit;
    }

exit:
    return;
}

void network_start(void)
{
    int i;

    for (i = 0; i < CONFIG_APP_EP_NUM_PORTS; i++) {
        __system_config_update_iface(i);
    }
}
