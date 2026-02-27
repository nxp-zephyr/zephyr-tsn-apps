/*
 * Copyright 2022-2023, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * How to setup clock using clock driver functions:
 *
 * 1. Call CLOCK_InitXXXPLL() to configure corresponding PLL clock.
 *
 * 2. Call CLOCK_InitXXXpfd() to configure corresponding PLL pfd clock.
 *
 * 3. Call CLOCK_SetRootClock() to configure corresponding module clock source and divider.
 *
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v13.0
processor: MIMXRT1189xxxxx
package_id: MIMXRT1189CVM8B
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

#include "clock_config.h"
#include "fsl_misc.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
    clock_root_config_t rootCfg = {0};

    /* Configure GPT1 using SYS_PLL3_DIV2_CLK */
    rootCfg.mux = kCLOCK_GPT1_ClockRoot_MuxSysPll3Div2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Gpt1, &rootCfg);

    /* Configure ENET using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_ENET_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Enet, &rootCfg);

    /* Configure TMR_1588 using SYS_PLL3_CLK */
    rootCfg.mux = kCLOCK_TMR_1588_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Tmr_1588, &rootCfg);

    /* Configure NETC using SYS_PLL3_PFD3_CLK */
    rootCfg.mux = kCLOCK_NETC_ClockRoot_MuxSysPll3Pfd3;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Netc, &rootCfg);

    /* Configure MAC0 using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_MAC0_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Mac0, &rootCfg);

    /* Configure MAC1 using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_MAC1_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac1, &rootCfg);

    /* Configure MAC2 using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_MAC2_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac2, &rootCfg);

    /* Configure MAC3 using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_MAC3_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Mac3, &rootCfg);

    /* Configure MAC4 using SYS_PLL1_DIV2_CLK */
    rootCfg.mux = kCLOCK_MAC4_ClockRoot_MuxSysPll1Div2;
    rootCfg.div = 10;
    CLOCK_SetRootClock(kCLOCK_Root_Mac4, &rootCfg);

    /* Set NETC PORT Ref clock source. */
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK;
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT1_RMII_REF_CLK_DIR_MASK;
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT2_RMII_REF_CLK_DIR_MASK;
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT3_RMII_REF_CLK_DIR_MASK;
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK;

    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_TMR_EXT_CLK_SEL_MASK;
}
