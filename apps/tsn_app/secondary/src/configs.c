/*
 * Copyright 2022, 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gavb_stack.h"
#include "system_config.h"

/* The secondary core configuration is retrieved from shared memory
 * prior to stack initialization via shared_system_config_get().
 * Its default settings originate from m7_system_cfg on the primary core,
 * and may be overridden by filesystem attributes configured there (on the
 * primary core).
 */
struct system_config system_cfg = {0};
