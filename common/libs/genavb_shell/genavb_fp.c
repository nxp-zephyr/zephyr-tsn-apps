/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef CONFIG_APP_FP
#include "shell_config.h"
#include "genavb_shell.h"
#include "rtos_apps/shell/fp.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_fp,
    SHELL_CMD(fp_set, NULL, CMD_FP_SET_HELP, &cmd_fp_set),
    SHELL_CMD(fp_get, NULL, CMD_FP_GET_HELP, &cmd_fp_get),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), fp, &genavb_cmd_fp,
                 "Frame Preemption configuration and management commands",
                 cmd_genavb_default, 1, 0);

#endif
