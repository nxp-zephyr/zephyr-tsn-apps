/*
 * Copyright 2021-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_rgpio.h"
#include "fsl_clock.h"
#include "fsl_gpt.h"
#include "fsl_phy.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_Init(void);
void BOARD_InitNVIC(void);
void BOARD_NETC_Init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
