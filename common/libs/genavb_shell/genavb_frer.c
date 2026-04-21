/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "shell_config.h"
#include "genavb_shell.h"
#include "rtos_apps/shell/frer.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_frer,
    SHELL_CMD(seqg_update, NULL, CMD_SEQG_UPDATE_HELP, &cmd_seqg_update),
    SHELL_CMD(seqg_delete, NULL, CMD_SEQG_DELETE_HELP, &cmd_seqg_delete),
    SHELL_CMD(seqg_read, NULL, CMD_SEQG_READ_HELP, &cmd_seqg_read),
    SHELL_CMD(seqr_update, NULL, CMD_SEQR_UPDATE_HELP, &cmd_seqr_update),
    SHELL_CMD(seqr_delete, NULL, CMD_SEQR_DELETE_HELP, &cmd_seqr_delete),
    SHELL_CMD(seqr_read, NULL, CMD_SEQR_READ_HELP, &cmd_seqr_read),
    SHELL_CMD(seqi_update, NULL, CMD_SEQI_UPDATE_HELP, &cmd_seqi_update),
    SHELL_CMD(seqi_delete, NULL, CMD_SEQI_DELETE_HELP, &cmd_seqi_delete),
    SHELL_CMD(seqi_read, NULL, CMD_SEQI_READ_HELP, &cmd_seqi_read),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), frer, &genavb_cmd_frer,
                 "Frame Replication and Elimination for Reliability (FRER) commands",
                 cmd_genavb_default, 1, 0);
