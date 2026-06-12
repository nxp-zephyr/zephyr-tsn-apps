/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#if defined(CONFIG_APP_STORAGE)

/**
 * @brief Initialize the storage module.
 *
 * Prepares the file system by either mouting and formatting it or check
 * it has properly been mounted by the OS and sets up the internal
 * storage context. Must be called before any other storage function.
 *
 * @retval 0 on success;
 * @retval -1 on error.
 */
int storage_init(void);

/**
 * @brief De-initialize the storage module.
 *
 * Unmounts the file system and releases the internal storage context.
 * No storage function should be called after this.
 */
void storage_exit(void);

#else

static inline int storage_init(void)
{
    return -1;
}
static inline void storage_exit(void)
{
    return;
}

#endif /* CONFIG_APP_STORAGE */

#endif /* _STORAGE_H_ */

