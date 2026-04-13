/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3
 */

#include <errno.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_pkt.h>

#include "system_config.h"

#define DT_DRV_COMPAT nxp_genavbtsn_eth

struct ethernetif_config {
    unsigned int port_id;
    struct net_config *cfg;
};

struct ethernetif_ctx {
    struct net_if *iface;
};

static int genavbtsn_eth_dev_init(const struct device *dev)
{
    ARG_UNUSED(dev);

    return 0;
}

static void genavbtsn_eth_iface_init(struct net_if *iface)
{
    const struct device *dev = net_if_get_device(iface);
    const struct ethernetif_config *eth_cfg = (const struct ethernetif_config *)dev->config;
    struct ethernetif_ctx *ctx = (struct ethernetif_ctx *)dev->data;
    int rc;

    ctx->iface = iface;
    if (eth_cfg->cfg == NULL) {
        printk("genavbtsn_eth_iface_init: missing cfg for port %u\n", eth_cfg->port_id);
        return;
    }

    rc = net_if_set_link_addr(iface, eth_cfg->cfg->hw_addr,
                              sizeof(eth_cfg->cfg->hw_addr),
                              NET_LINK_ETHERNET);
    if (rc < 0) {
        printk("net_if_set_link_addr() failed: %d\n", rc);
        return;
    }

    ethernet_init(iface);
}

static enum ethernet_hw_caps genavbtsn_eth_get_capabilities(const struct device *dev)
{
    ARG_UNUSED(dev);

    return 0;
}

static int genavbtsn_eth_send(const struct device *dev, struct net_pkt *pkt)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(pkt);

    /* TODO */
    return -ENOTSUP;
}

static const struct ethernet_api genavbtsn_eth_api = {
    .iface_api.init = genavbtsn_eth_iface_init,
    .get_capabilities = genavbtsn_eth_get_capabilities,
    .get_phy = NULL,
    .send = genavbtsn_eth_send,
};


#define IP_ETHERNET_INIT(n) \
    BUILD_ASSERT(DT_INST_PROP(n, port_id) < CONFIG_APP_LOGICAL_PORTS, \
                 "genavbtsn_eth port-id out of range"); \
    static struct ethernetif_ctx genavbtsn_eth_ctx_##n; \
    static struct ethernetif_config genavbtsn_eth_cfg_##n = { \
        .port_id = DT_INST_PROP(n, port_id), \
        .cfg = &system_net_cfg[DT_INST_PROP(n, port_id)], \
    }; \
    ETH_NET_DEVICE_DT_INST_DEFINE(n, genavbtsn_eth_dev_init, NULL, \
                    &genavbtsn_eth_ctx_##n, &genavbtsn_eth_cfg_##n, \
                    CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
                    &genavbtsn_eth_api, NET_ETH_MTU);

DT_INST_FOREACH_STATUS_OKAY(IP_ETHERNET_INIT)
