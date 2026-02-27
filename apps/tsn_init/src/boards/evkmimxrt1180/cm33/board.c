/*
 * Copyright 2021-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "genavb_sdk.h"
#include "pin_mux.h"

#include "fsl_netc.h"
#include "fsl_netc_soc.h"
#include "fsl_cache.h"
#include "fsl_netc_ierb.h"

#include "zephyr/irq.h"
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
 
static void BOARD_NetPhysReset(void)
{
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Reset PHY8211 for ETH1/ETH2/ETH3 (Switch port1/2/3) */

    /* REALTEK_RTL8211FD DataSheet v1.4
    - Power-on: v3.3 setup should not exceed 100ms
    - Reset: For a complete PHY reset, this pin must be asserted low for at least 10ms for the internal regulator.
             Wait for at least 50ms (for internal circuits settling time) before accessing the PHY register.
    - Toggling Time: Period between consecutive ON/OFF toggling action must be longer than 100ms.
    */

    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */

    RGPIO_PinInit(RGPIO4, 13, &pinConfig); /* Eth4 (EP), GPIO_AD_13 */
    RGPIO_PinInit(RGPIO4, 25, &pinConfig); /* Eth0 (Switch port0), GPIO_AD_25 */
    RGPIO_PinInit(RGPIO6, 13, &pinConfig); /* Eth1(Switch port1), GPIO_B1_13 */
    RGPIO_PinInit(RGPIO4, 28, &pinConfig); /* Eth2(Switch port2), GPIO_AD_28 */
    RGPIO_PinInit(RGPIO6, 15, &pinConfig); /* Eth3(Switch port3), GPIO_B2_01 */

    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    RGPIO_PinWrite(RGPIO4, 13, 1); /* Eth4 (EP), GPIO_AD_13 */
    RGPIO_PinWrite(RGPIO4, 25, 1); /* Eth0 (Switch port0), GPIO_AD_25 */
    RGPIO_PinWrite(RGPIO6, 13, 1); /* Eth1(Switch port1), GPIO_B1_13 */
    RGPIO_PinWrite(RGPIO4, 28, 1); /* Eth2(Switch port2), GPIO_AD_28 */
    RGPIO_PinWrite(RGPIO6, 15, 1); /* Eth3(Switch port3), GPIO_B2_01 */

    SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_NETC_Init(void)
{
    /* EP and Switch port 0 use RMII interface. */
    NETC_SocSetMiiMode(kNETC_SocLinkEp0, kNETC_RmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort0, kNETC_RmiiMode);

    /* Switch port 1~3 use RGMII interface. */
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort1, kNETC_RgmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort2, kNETC_RgmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort3, kNETC_RgmiiMode);

    /* Output reference clock for RMII interface. */
    NETC_SocSetRmiiRefClk(kNETC_SocLinkEp0, true);
    NETC_SocSetRmiiRefClk(kNETC_SocLinkSwitchPort0, true);

    /* Unlock the IERB. It will warm reset whole NETC. */
    if (NETC_IERBUnlock() == kStatus_Success)
    {
        while (!NETC_IERBIsUnlockOver())
        {
        }
    }

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
#if (BOARD_NUM_LINK > 0)
    NETC_SocSetLinkAddr(BOARD_LINK0_PORT, BOARD_LINK0_ADDRESS);
#endif /* (BOARD_NUM_LINK > 0) */
#if (BOARD_NUM_LINK > 1)
    NETC_SocSetLinkAddr(BOARD_LINK1_PORT, BOARD_LINK1_ADDRESS);
#endif /* (BOARD_NUM_LINK > 1) */
#if (BOARD_NUM_LINK > 2)
    NETC_SocSetLinkAddr(BOARD_LINK2_PORT, BOARD_LINK2_ADDRESS);
#endif /* (BOARD_NUM_LINK > 2) */
#if (BOARD_NUM_LINK > 3)
    NETC_SocSetLinkAddr(BOARD_LINK3_PORT, BOARD_LINK3_ADDRESS);
#endif /* (BOARD_NUM_LINK > 3) */
#if (BOARD_NUM_LINK > 4)
    NETC_SocSetLinkAddr(BOARD_LINK4_PORT, BOARD_LINK4_ADDRESS);
#endif /* (BOARD_NUM_LINK > 4) */

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Lock the IERB. */
    assert(NETC_IERBLock() == kStatus_Success);
    while (!NETC_IERBIsLockOver())
    {
    }

    BOARD_NetPhysReset();
}

/*!
 * @brief   void BOARD_Init(void)
 *           - Initialization of the board low level hardware
 * @param   void
 *
 * @return  none
 */
/*
 */
void BOARD_Init(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_NETC_Init();
}

extern void BOARD_GPT_0_IRQ_HANDLER(void);
extern void MSGINTR1_IRQHandler(void);

void BOARD_InitNVIC(void)
{
    IRQ_CONNECT(MSGINTR1_IRQn, 4, MSGINTR1_IRQHandler, NULL, 0);
    IRQ_CONNECT(BOARD_GPT_0_IRQ, 4, BOARD_GPT_0_IRQ_HANDLER, NULL, 0);
}