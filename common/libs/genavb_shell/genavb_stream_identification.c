/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/shell/shell.h>
#include "genavb_shell.h"
#include "rtos_apps/shell/stream_identification.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_si,
    SHELL_CMD(update, NULL, CMD_SI_UPDATE_HELP, &cmd_si_update),
    SHELL_CMD(read, NULL, CMD_SI_READ_HELP, &cmd_si_read),
    SHELL_CMD(delete, NULL, CMD_SI_DELETE_HELP, &cmd_si_delete),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), si, &genavb_cmd_si,
                 "Stream Identification table configuration and management commands",
                 cmd_genavb_default, 1, 0);
