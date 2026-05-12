/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>

#include "rtos_apps/shell/port_stats.h"
#include "rtos_apps/shell/shell.h"
#include "genavb_shell.h"

#include "rtos_apps/shell/fdb.h"
#include "rtos_apps/shell/fp.h"
#include "rtos_apps/shell/frer.h"
#include "rtos_apps/shell/psfp.h"
#include "rtos_apps/shell/qbv.h"
#include "rtos_apps/shell/stream_identification.h"
#include "rtos_apps/shell/vlan.h"

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

void genavb_shell_init(void)
{
    void *shell = (void *)shell_backend_uart_get_ptr();

    if (shell == NULL) {
        printk("shell_backend_uart_get_ptr() failed\n");
        return;
    }

    cmd_fdb_init(shell);
#ifdef CONFIG_APP_FP
    cmd_fp_init(shell);
#endif
    cmd_frer_init(shell);
    cmd_psfp_init(shell);
#ifdef CONFIG_APP_QBV
    cmd_qbv_init(shell);
#endif
    cmd_stream_identification_init(shell);
    cmd_vlan_init(shell);
}
