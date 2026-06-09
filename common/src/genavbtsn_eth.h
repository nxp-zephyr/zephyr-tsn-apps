/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief Public API for GenAVB/TSN Ethernet Interface
 */

#ifndef GENAVBTSN_ETH_INCLUDE_H_
#define GENAVBTSN_ETH_INCLUDE_H_

/**
 * @brief Public API for GenAVB/TSN Ethernet Interface
 * @{
 */

#include <zephyr/net/net_if.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get network interface attached to a certain logical port id
 *
 * @param port_id Logical port id
 *
 * @return Pointer to interface or NULL if not found.
 */
struct net_if *genavbtsn_eth_get_by_port_id(unsigned int port_id);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* GENAVBTSN_ETH_INCLUDE_H_ */
