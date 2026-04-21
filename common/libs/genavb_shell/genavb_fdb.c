/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "shell_config.h"
#include "genavb_shell.h"
#include "rtos_apps/shell/fdb.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_fdb,
    SHELL_CMD(update, NULL, CMD_FDB_UPDATE_HELP, &cmd_fdb_update),
    SHELL_CMD(delete, NULL, CMD_FDB_DELETE_HELP, &cmd_fdb_delete),
    SHELL_CMD(read, NULL, CMD_FDB_READ_HELP, &cmd_fdb_read),
    SHELL_CMD(dump, NULL, CMD_FDB_DUMP_HELP, &cmd_fdb_dump),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), fdb, &genavb_cmd_fdb,
                 "Forwarding database configuration and management commands",
                 cmd_genavb_default, 1, 0);
