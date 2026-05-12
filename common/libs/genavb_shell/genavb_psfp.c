/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/shell/shell.h>
#include "genavb_shell.h"
#include "rtos_apps/shell/psfp.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_sf,
    SHELL_CMD(update, NULL, CMD_SF_UPDATE_HELP, &cmd_sf_update),
    SHELL_CMD(delete, NULL, CMD_SF_DELETE_HELP, &cmd_sf_delete),
    SHELL_CMD(read, NULL, CMD_SF_READ_HELP, &cmd_sf_read),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_sg,
    SHELL_CMD(update, NULL, CMD_SG_UPDATE_HELP, &cmd_sg_update),
    SHELL_CMD(delete, NULL, CMD_SG_DELETE_HELP, &cmd_sg_delete),
    SHELL_CMD(read, NULL, CMD_SG_READ_HELP, &cmd_sg_read),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_fm,
    SHELL_CMD(update, NULL, CMD_FM_UPDATE_HELP, &cmd_fm_update),
    SHELL_CMD(delete, NULL, CMD_FM_DELETE_HELP, &cmd_fm_delete),
    SHELL_CMD(read, NULL, CMD_FM_READ_HELP, &cmd_fm_read),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), sf, &genavb_cmd_sf,
                 "Stream Filter, Per-Stream Filtering and Policing (PSFP) configuration commands",
                 cmd_genavb_default, 1, 0);

SHELL_SUBCMD_ADD((genavb), sg, &genavb_cmd_sg,
                 "Stream Gate, Per-Stream Filtering and Policing (PSFP) configuration commands",
                 cmd_genavb_default, 1, 0);

SHELL_SUBCMD_ADD((genavb), fm, &genavb_cmd_fm,
                 "Flow Meter, Per-Stream Filtering and Policing (PSFP) configuration commands",
                 cmd_genavb_default, 1, 0);
