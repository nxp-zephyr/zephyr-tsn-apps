/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TSN_APP_H_
#define _TSN_APP_H_

#include "rtos_apps/tsn/tsn_entry.h"

#if defined(CONFIG_APP_TSN_ENDPOINT)
int tsn_app_init(struct rtos_apps_tsn_config *app_cfg);
void tsn_app_exit(void);

#else
inline int tsn_app_init(struct rtos_apps_tsn_config *app_cfg)
{
    return 0;
}

inline void tsn_app_exit(void)
{
    return;
}
#endif

#endif /* _TSN_APP_H_ */
