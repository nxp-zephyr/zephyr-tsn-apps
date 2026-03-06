/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>

#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>

#ifdef CONFIG_NET_DHCPV4
static void start_dhcpv4_client(struct net_if *iface, void *user_data)
{
    ARG_UNUSED(user_data);
    struct ethernet_context *eth_ctx;

    if (net_if_l2(iface) != &NET_L2_GET_NAME(ETHERNET)) {
        return;
    }

#ifdef CONFIG_NET_DSA
    eth_ctx = net_if_l2_data(iface);

    /* Skip if this is a DSA port */
    if (eth_ctx && (eth_ctx->dsa_port != NON_DSA_PORT)) {
        return;
    }
#endif

    printk("%s: start on %s: index=%d\n", __func__,
        net_if_get_device(iface)->name, net_if_get_by_iface(iface));

    net_dhcpv4_start(iface);
}
#else
static void start_dhcpv4_client(struct net_if *iface, void *user_data)
{
    return;
}
#endif

static void ipv4_init(void)
{
    net_if_foreach(start_dhcpv4_client, NULL);
}

int main(void)
{
    ipv4_init();

    printk("main: done\n");

    return 0;
}
