/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef CONFIG_APP_QBV
#include "shell_config.h"
#include "genavb_shell.h"
#include "rtos_apps/shell/qbv.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_qbv,
    SHELL_CMD(set, NULL, CMD_QBV_SET_HELP, &cmd_qbv_set),
    SHELL_CMD(get, NULL, CMD_QBV_GET_HELP, &cmd_qbv_get),
    SHELL_CMD(disable, NULL, CMD_QBV_DISABLE_HELP, &cmd_qbv_disable),
    SHELL_CMD(set_max_sdu, NULL, CMD_QBV_SET_MAX_SDU_HELP, &cmd_qbv_set_max_sdu),
    SHELL_CMD(get_max_sdu, NULL, CMD_QBV_GET_MAX_SDU_HELP, &cmd_qbv_get_max_sdu),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), qbv, &genavb_cmd_qbv,
                 "Time-Aware Shaper (Qbv) configuration and management commands",
                 cmd_genavb_default, 1, 0);
#endif
