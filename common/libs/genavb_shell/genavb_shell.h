/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _GENAVB_SHELL_H_
#define _GENAVB_SHELL_H_

#include "shell_config.h"

int cmd_genavb_default(const struct shell *sh, size_t argc, char *argv[]);

void genavb_shell_init(void);

#endif /* _GENAVB_SHELL_H_ */
