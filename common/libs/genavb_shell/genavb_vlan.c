/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/shell/shell.h>
#include "genavb_shell.h"
#include "rtos_apps/shell/vlan.h"

SHELL_STATIC_SUBCMD_SET_CREATE(genavb_cmd_vlan,
    SHELL_CMD(update, NULL, CMD_VLAN_UPDATE_HELP, &cmd_vlan_update),
    SHELL_CMD(delete, NULL, CMD_VLAN_DELETE_HELP, &cmd_vlan_delete),
    SHELL_CMD(read, NULL, CMD_VLAN_READ_HELP, &cmd_vlan_read),
    SHELL_CMD(dump, NULL, CMD_VLAN_DUMP_HELP, &cmd_vlan_dump),
    SHELL_CMD(set_pvid, NULL, CMD_VLAN_SET_PVID_HELP, &cmd_vlan_set_pvid),
    SHELL_CMD(get_pvid, NULL, CMD_VLAN_GET_PVID_HELP, &cmd_vlan_get_pvid),
    SHELL_SUBCMD_SET_END
);

SHELL_SUBCMD_ADD((genavb), vlan, &genavb_cmd_vlan,
                 "VLAN configuration and management commands",
                 cmd_genavb_default, 1, 0);
