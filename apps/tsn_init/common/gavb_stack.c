/*
 * Copyright 2018-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gavb_stack.h"
#include "rtos_abstraction_layer.h"
#include "rtos_apps/storage.h"
#include "rtos_apps/storage_common.h"

#include "genavb/error.h"
#include "genavb/ether.h"
#include "genavb/config.h"
#include "genavb/helpers.h"
#include "genavb/genavb.h"
#include "genavb/vlan.h"

#include "storage_config.h"

const unsigned int br_port_list[CONFIG_APP_BR_NUM_PORTS] = CONFIG_APP_BR_LOGICAL_PORT_LIST;

static struct genavb_handle *s_genavb_handle = NULL;

struct genavb_handle *gavb_stack_handle(void)
{
    return s_genavb_handle;
}

static char *domain_cfg_param_file(uint8_t instance, char *param, char *buf, size_t size)
{
    if (!instance)
        h_snprintf(buf, size, "%s", param);
    else
        h_snprintf(buf, size, "domain%u/%s", instance, param);

    return buf;
}

static char *port_cfg_param_file(uint8_t port, char *param, char *buf, size_t size)
{
    h_snprintf(buf, size, "port%u/%s", port, param);

    return buf;
}

static char *domain_port_cfg_param_file(uint8_t instance, uint8_t port, char *param, char *buf, size_t size)
{
    if (!instance)
        h_snprintf(buf, size, "%s", param);
    else
        h_snprintf(buf, size, "domain%u/port%u/%s", instance, port, param);

    return buf;
}

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
    unsigned int logical_port_id;
    struct genavb_config *genavb_config;
    char buf[128];
    int rc = 0;
    int i, j;

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

    storage_read_uint(CONFIG_STORAGE_ROOT "/management", "is_bridge", &genavb_config->management_config.is_bridge);

    if (genavb_config->fgptp_config.is_bridge) {
        genavb_config->fgptp_config.port_max = bridge_port_max;
        memcpy(genavb_config->fgptp_config.logical_port_list, bridge_logical_port_list, bridge_port_max * sizeof(unsigned int));
    } else {
        genavb_config->fgptp_config.port_max = endpoint_port_max;
        memcpy(genavb_config->fgptp_config.logical_port_list, endpoint_logical_port_list, endpoint_port_max * sizeof(unsigned int));
    }

    storage_read_uint(CONFIG_STORAGE_ROOT "/gptp", "is_bridge", &genavb_config->fgptp_config.is_bridge);

    /* Read general parameters */
    storage_read_uint(CONFIG_STORAGE_ROOT "/gptp", "force_2011", &genavb_config->fgptp_config.force_2011);
    storage_read_u64(CONFIG_STORAGE_ROOT "/gptp", "neighborPropDelayThreshold", &genavb_config->fgptp_config.neighborPropDelayThreshold);

    /* Read per-domain parameters */
    for (i = 0; i < CFG_MAX_GPTP_DOMAINS; i++) {

        if (i != 0)
            storage_read_int(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "domain_number", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].domain_number);

        if (genavb_config->fgptp_config.is_bridge == 1) {
            genavb_config->fgptp_config.domain_cfg[i].gmCapable = 1;
            genavb_config->fgptp_config.domain_cfg[i].priority1 = 246;
        }

        storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "gmCapable", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].gmCapable);
        storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "priority1", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].priority1);
        storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "priority2", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].priority2);
        storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "clockClass", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].clockClass);
        storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "clockAccuracy", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].clockAccuracy);
        storage_read_u16(CONFIG_STORAGE_ROOT "/gptp", domain_cfg_param_file(i, "offsetScaledLogVariance", buf, sizeof(buf)), &genavb_config->fgptp_config.domain_cfg[i].offsetScaledLogVariance);

        /* Read per-port parameters */
        for (j = 0; j < genavb_config->fgptp_config.port_max; j++) {
            logical_port_id = genavb_config->fgptp_config.logical_port_list[j];
            if (i == 0) {
                /* Below parameters are only needed for domain 0 */

                storage_read_int(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "rxDelayCompensation", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].rxDelayCompensation);
                storage_read_int(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "txDelayCompensation", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].txDelayCompensation);
                storage_read_s8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "initialLogPdelayReqInterval", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].initialLogPdelayReqInterval);
                storage_read_s8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "initialLogSyncInterval", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].initialLogSyncInterval);
                storage_read_s8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "initialLogAnnounceInterval", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].initialLogAnnounceInterval);
                storage_read_s8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "operLogPdelayReqInterval", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].operLogPdelayReqInterval);
                storage_read_s8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "operLogSyncInterval", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].operLogSyncInterval);
                storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "allowedLostResponses", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].allowedLostResponses);
                storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", port_cfg_param_file(logical_port_id, "delayMechanism", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].delayMechanism[i]);
            } else {
                storage_read_u8(CONFIG_STORAGE_ROOT "/gptp", domain_port_cfg_param_file(i, logical_port_id, "delayMechanism", buf, sizeof(buf)), &genavb_config->fgptp_config.port_cfg[j].delayMechanism[i]);
            }
        }
    }

    storage_read_uint(CONFIG_STORAGE_ROOT "/srp", "is_bridge", &genavb_config->srp_config.is_bridge);

    storage_read_bool(CONFIG_STORAGE_ROOT "/hsr", "hsr_enabled", &genavb_config->hsr_config.hsr_enabled);
    for (i = 0; i < CONFIG_APP_LOGICAL_PORTS; i++) {
        uint8_t _hsr_port_type = 0;

        storage_read_u8(CONFIG_STORAGE_ROOT "/hsr", port_cfg_param_file(i, "type", buf, sizeof(buf)), &_hsr_port_type);

        genavb_config->hsr_config.hsr_port[i].type = (hsr_port_type)_hsr_port_type;
        genavb_config->hsr_config.hsr_port[i].logical_port = i;
    }
    genavb_config->hsr_config.port_max = i;

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
