/*
 * Copyright 2018-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gavb_stack.h"
#include "rtos_abstraction_layer.h"

#include "genavb/error.h"
#include "genavb/ether.h"
#include "genavb/config.h"
#include "genavb/genavb.h"
#include "genavb/vlan.h"

const unsigned int br_port_list[CONFIG_APP_BR_NUM_PORTS] = CONFIG_APP_BR_LOGICAL_PORT_LIST;

static struct genavb_handle *s_genavb_handle = NULL;

struct genavb_handle *gavb_stack_handle(void)
{
    return s_genavb_handle;
}

#ifdef CONFIG_APP_TSN_BRIDGE
static void gavb_bridge_vlan_init(void)
{
    struct genavb_vlan_port_map port_map[CONFIG_APP_BR_NUM_PORTS] = {0};
    unsigned int port_id;
    uint16_t vid;
    int i, rc;

    for (i = 0; i < CONFIG_APP_BR_NUM_PORTS; i++) {
        port_id = br_port_list[i];

        vid = VLAN_PVID_DEFAULT;

        port_map[i].port_id = port_id;
        port_map[i].control = GENAVB_VLAN_ADMIN_CONTROL_FIXED;

        rc = genavb_vlan_update(vid, &port_map[i]);
        if (rc < 0)
            rtos_printf("genavb_vlan_update() failed: %s\n", genavb_strerror(rc));
    }
}
#endif

int gavb_stack_init(void)
{
    unsigned int endpoint_logical_port_list[CONFIG_APP_EP_NUM_PORTS] = CONFIG_APP_EP_LOGICAL_PORT_LIST;
    unsigned int bridge_logical_port_list[CONFIG_APP_BR_NUM_PORTS] = CONFIG_APP_BR_LOGICAL_PORT_LIST;
#if CONFIG_APP_BR_NUM_PORTS > 1
    unsigned int bridge_port_max = CONFIG_APP_BR_NUM_PORTS - 1; /* assume last port is host port */
#else
    unsigned int bridge_port_max = CONFIG_APP_BR_NUM_PORTS;
#endif
    unsigned int endpoint_port_max = CONFIG_APP_EP_NUM_PORTS;
    struct genavb_config *genavb_config;
    int rc = 0;
    int i;

    genavb_config = rtos_malloc(sizeof(struct genavb_config));

    if (!genavb_config) {
        rtos_printf("Failed to allocate memory for genavb_config\n");
        rc = -1;
        goto exit;
    }

    if (s_genavb_handle) {
        rc = 0;
        goto exit;
    }

    genavb_get_default_config(genavb_config);

#ifdef CONFIG_APP_TSN_BRIDGE
    genavb_config->management_config.is_bridge = 1;
    genavb_config->fgptp_config.is_bridge = 1;
    genavb_config->srp_config.is_bridge = 1;
#endif /* CONFIG_APP_TSN_BRIDGE */

    if (genavb_config->fgptp_config.is_bridge) {
        genavb_config->fgptp_config.port_max = bridge_port_max;
        memcpy(genavb_config->fgptp_config.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->fgptp_config.port_max = endpoint_port_max;
        memcpy(genavb_config->fgptp_config.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    if (genavb_config->fgptp_config.is_bridge == 1) {
        for (i = 0; i < CFG_MAX_GPTP_DOMAINS; i++) {
            genavb_config->fgptp_config.domain_cfg[i].gmCapable = 1;
            genavb_config->fgptp_config.domain_cfg[i].priority1 = 246;
        }
    }

    if (genavb_config->management_config.is_bridge) {
        genavb_config->management_config.port_max = bridge_port_max;
        memcpy(genavb_config->management_config.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->management_config.port_max = endpoint_port_max;
        memcpy(genavb_config->management_config.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    if (genavb_config->srp_config.is_bridge) {
        genavb_config->srp_config.port_max = bridge_port_max;
        memcpy(genavb_config->srp_config.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->srp_config.port_max = endpoint_port_max;
        memcpy(genavb_config->srp_config.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    genavb_config->srp_config.mvrp_cfg.is_bridge = genavb_config->srp_config.is_bridge;

    if (genavb_config->srp_config.mvrp_cfg.is_bridge) {
        genavb_config->srp_config.mvrp_cfg.port_max = bridge_port_max;
        memcpy(genavb_config->srp_config.mvrp_cfg.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->srp_config.mvrp_cfg.port_max = endpoint_port_max;
        memcpy(genavb_config->srp_config.mvrp_cfg.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    genavb_config->srp_config.msrp_cfg.is_bridge = genavb_config->srp_config.is_bridge;

    if (genavb_config->srp_config.msrp_cfg.is_bridge) {
        genavb_config->srp_config.msrp_cfg.port_max = bridge_port_max;
        memcpy(genavb_config->srp_config.msrp_cfg.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->srp_config.msrp_cfg.port_max = endpoint_port_max;
        memcpy(genavb_config->srp_config.msrp_cfg.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    genavb_set_config(genavb_config);

    if ((rc = genavb_init(&s_genavb_handle, 0)) != GENAVB_SUCCESS) {
        s_genavb_handle = NULL;
        rtos_printf("genavb_init() failed: %s\n", genavb_strerror(rc));
        goto exit;
    }

#ifdef CONFIG_APP_TSN_BRIDGE
    gavb_bridge_vlan_init();
#endif

exit:
    if (genavb_config)
        rtos_free(genavb_config);

    return rc;
}

int gavb_stack_exit(void)
{
    genavb_exit(s_genavb_handle);

    s_genavb_handle = NULL;

    return 0;
}
