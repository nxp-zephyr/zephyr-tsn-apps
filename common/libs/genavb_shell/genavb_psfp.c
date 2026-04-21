/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "shell_config.h"
#include "genavb_shell.h"
#include "rtos_apps/shell/psfp.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_psfp,
    SHELL_CMD(sf_update, NULL, CMD_SF_UPDATE_HELP, &cmd_sf_update),
    SHELL_CMD(sf_delete, NULL, CMD_SF_DELETE_HELP, &cmd_sf_delete),
    SHELL_CMD(sf_read, NULL, CMD_SF_READ_HELP, &cmd_sf_read),
    SHELL_CMD(sg_update, NULL, CMD_SG_UPDATE_HELP, &cmd_sg_update),
    SHELL_CMD(sg_delete, NULL, CMD_SG_DELETE_HELP, &cmd_sg_delete),
    SHELL_CMD(sg_read, NULL, CMD_SG_READ_HELP, &cmd_sg_read),
    SHELL_CMD(fm_update, NULL, CMD_FM_UPDATE_HELP, &cmd_fm_update),
    SHELL_CMD(fm_delete, NULL, CMD_FM_DELETE_HELP, &cmd_fm_delete),
    SHELL_CMD(fm_read, NULL, CMD_FM_READ_HELP, &cmd_fm_read),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), psfp, &genavb_cmd_psfp,
                 "Per-Stream Filtering and Policing (PSFP) configuration commands",
                 cmd_genavb_default, 1, 0);
