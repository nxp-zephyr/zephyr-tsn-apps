/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_pkt.h>

#include "genavb/error.h"
#include "genavb/socket.h"
#include "genavb/port.h"

#include "gavb_stack.h"
#include "system_config.h"

#define DT_DRV_COMPAT nxp_genavbtsn_eth

struct ethernetif_config {
    unsigned int port_id;
    struct net_config *cfg;
};

#define PKT_MAX_LEN (NET_ETH_MAX_FRAME_SIZE)

struct ethernetif_ctx {
    struct net_if *iface;
    struct genavb_socket_tx *tx_sock;
    struct genavb_socket_rx *rx_sock;
    bool started;
    uint8_t buf[PKT_MAX_LEN];
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

    rc = net_if_set_link_addr(iface, eth_cfg->cfg->hw_addr,
                              sizeof(eth_cfg->cfg->hw_addr),
                              NET_LINK_ETHERNET);
    if (rc < 0)
        goto exit;

    ethernet_init(iface);
    net_if_flag_set(iface, NET_IF_NO_AUTO_START);

exit:
    return;
}

static int genavbtsn_eth_start(const struct device *dev)
{
    struct ethernetif_config *eth_cfg = (struct ethernetif_config *)dev->config;
    struct ethernetif_ctx *ctx = (struct ethernetif_ctx *)dev->data;
    struct genavb_socket_tx_params tx_params = {
        .addr = {
            .ptype = PTYPE_L2,
            .port = eth_cfg->port_id,
            .vlan_id = 0xffff,
            .priority = 0,
        },
    };
    struct genavb_socket_rx_params rx_params = {
        .addr = {
            .ptype = PTYPE_OTHER,
            .port = eth_cfg->port_id,
        },
    };
    bool up, duplex;
    uint64_t rate;
    int rc = 0;

    if (ctx->started)
        goto exit;

    if (gavb_stack_handle() == NULL) {
        rc = -EAGAIN;
        goto exit;
    }

    rc = genavb_socket_tx_open(&ctx->tx_sock, GENAVB_SOCKF_RAW, &tx_params);
    if (rc != GENAVB_SUCCESS) {
        rc = -EIO;
        goto exit;
    }

    rc = genavb_socket_rx_open(&ctx->rx_sock, GENAVB_SOCKF_RAW, &rx_params);
    if (rc != GENAVB_SUCCESS) {
        rc = -EIO;
        goto err_rx_open;
    }

    ctx->started = true;
    rc = genavb_port_status_get(eth_cfg->port_id, &up, &duplex, &rate);
    if (rc == GENAVB_SUCCESS && up)
        net_eth_carrier_on(ctx->iface);
    else
        net_eth_carrier_off(ctx->iface);

    goto exit;

err_rx_open:
    genavb_socket_tx_close(ctx->tx_sock);
    ctx->tx_sock = NULL;
exit:
    return rc;
}

static int genavbtsn_eth_stop(const struct device *dev)
{
    struct ethernetif_ctx *ctx = (struct ethernetif_ctx *)dev->data;
    int rc = 0;

    if (!ctx->started)
        goto exit;

    net_eth_carrier_off(ctx->iface);

    if (ctx->rx_sock) {
        genavb_socket_rx_close(ctx->rx_sock);
        ctx->rx_sock = NULL;
    }

    if (ctx->tx_sock) {
        genavb_socket_tx_close(ctx->tx_sock);
        ctx->tx_sock = NULL;
    }

    ctx->started = false;

exit:
    return rc;
}

static enum ethernet_hw_caps genavbtsn_eth_get_capabilities(const struct device *dev)
{
    ARG_UNUSED(dev);

    return 0;
}

static int genavbtsn_eth_send(const struct device *dev, struct net_pkt *pkt)
{
    struct ethernetif_ctx *ctx = (struct ethernetif_ctx *)dev->data;
    size_t len;
    int rc = 0;

    len = net_pkt_get_len(pkt);
    if (len == 0) {
        rc = -EINVAL;
        goto exit;
    }

    if (len > PKT_MAX_LEN) {
        rc = -EMSGSIZE;
        goto exit;
    }

    rc = net_pkt_read(pkt, ctx->buf, len);
    if (rc)
        goto exit;

    rc = genavb_socket_tx(ctx->tx_sock, (void *)ctx->buf, len);
    if (rc != GENAVB_SUCCESS)
        rc = -EIO;
    else
        rc = 0;

exit:
    return rc;
}

static const struct ethernet_api genavbtsn_eth_api = {
    .iface_api.init = genavbtsn_eth_iface_init,
    .start = genavbtsn_eth_start,
    .stop = genavbtsn_eth_stop,
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
