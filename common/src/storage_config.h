/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _STORAGE_CONFIG_H_
#define _STORAGE_CONFIG_H_

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>

/* Mount configuration */
#define PARTITION_NODE DT_NODELABEL(lfs1)

#if DT_NODE_EXISTS(PARTITION_NODE)
#define CONFIG_STORAGE_ROOT FSTAB_ENTRY_DT_MOUNT_POINT(PARTITION_NODE)
#else
#define CONFIG_STORAGE_ROOT ""
#endif

#endif /* _STORAGE_CONFIG_H_ */
