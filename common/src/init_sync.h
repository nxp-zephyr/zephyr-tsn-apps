/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef INIT_SYNC_H
#define INIT_SYNC_H

#if defined(CONFIG_TSN_MULTICORE)
int init_sync_signal_stack_ready(void);
int init_sync_wait_for_stack_ready(void);

#else
static inline int init_sync_wait_for_stack_ready(void)
{
    return -1;
}
static inline int init_sync_signal_stack_ready(void)
{
    return -1;
}
#endif

#endif /* INIT_SYNC_H */
