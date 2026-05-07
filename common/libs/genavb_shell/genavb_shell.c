/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>

#include "rtos_apps/shell/port_stats.h"
#include "rtos_apps/shell/shell.h"
#include "shell_config.h"
#include "genavb_shell.h"

SHELL_SUBCMD_SET_CREATE(genavb_cmds, (genavb));

int cmd_genavb_default(const struct shell *sh, size_t argc, char *argv[])
{
    if (argc == 1) {
        shell_help(sh);
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s: unknown parameter: %s", argv[0], argv[1]);
    return -EINVAL;
}

SHELL_SUBCMD_ADD((net), genavb, &genavb_cmds,
                 "GenAVB/TSN stack configuration and management commands",
                 cmd_genavb_default, 1, 0);

SHELL_SUBCMD_ADD((genavb), port_stats, NULL,
                 CMD_PORT_STATS_HELP,
                 cmd_port_stats, 2, 0);

SHELL_SUBCMD_ADD((genavb), log, NULL,
                 CMD_SHELL_LOG_HELP,
                 cmd_shell_log, 3, 0);
