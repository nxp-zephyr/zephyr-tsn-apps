/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/shell/shell.h>
#include "genavb_shell.h"
#include "rtos_apps/shell/hsr.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_hsr,
    SHELL_CMD(mode_set, NULL, CMD_HSR_MODE_SET_HELP, &cmd_hsr_mode_set),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), hsr, &genavb_cmd_hsr,
                 "High-availability Seamless Redundancy (HSR) configuration commands",
                 cmd_genavb_default, 1, 0);
