/*
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "genavb_sdk.h"
#include <zephyr/kernel.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_Init(void)
{
}

extern void BOARD_GPT_0_IRQ_HANDLER(void);
extern void MSGINTR1_IRQHandler(void);

void BOARD_InitNVIC(void)
{
    IRQ_CONNECT(MSGINTR1_IRQn, 0, MSGINTR1_IRQHandler, NULL, 0);
    IRQ_CONNECT(BOARD_GPT_0_IRQ, 1, BOARD_GPT_0_IRQ_HANDLER, NULL, 0);
}
