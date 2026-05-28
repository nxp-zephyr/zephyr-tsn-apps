/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SHELL_CONFIG_H_
#define _SHELL_CONFIG_H_

#include <zephyr/shell/shell.h>

#include <zephyr/posix/unistd.h>

#include "rtos_apps/log.h"

#include "gavb_stack.h"

#if CONFIG_APP_STORAGE
#include "storage_config.h"
#endif

#define shell_printf(sh, ...)                             \
    do {                                                  \
        if (sh)                                           \
            shell_fprintf(sh, SHELL_NORMAL, __VA_ARGS__); \
    } while(0)

#endif /* _SHELL_CONFIG_H_ */
