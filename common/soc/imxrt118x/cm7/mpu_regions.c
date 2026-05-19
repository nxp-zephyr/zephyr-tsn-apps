/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/devicetree.h>
#include <zephyr/arch/arm/mpu/arm_mpu_mem_cfg.h>

#define MEMORY_REGION_SIZE_KB(SIZE)    (SIZE / 1024)

#define ITCM_SIZE                       DT_REG_SIZE_BY_IDX(DT_NODELABEL(itcm), 0)
#define DTCM_SIZE                       DT_REG_SIZE_BY_IDX(DT_NODELABEL(dtcm), 0)
#define OCRAM2_SIZE                     DT_REG_SIZE_BY_IDX(DT_NODELABEL(ocram2), 0)
#define PERIPHERAL_SIZE                 DT_REG_SIZE_BY_IDX(DT_NODELABEL(peripheral), 0)

#define REGION_ITCM_BASE_ADDRESS         DT_REG_ADDR_BY_IDX(DT_NODELABEL(itcm), 0)
#define REGION_ITCM_SIZE                 \
			REGION_CUSTOMED_MEMORY_SIZE(MEMORY_REGION_SIZE_KB(ITCM_SIZE))
#define REGION_DTCM_BASE_ADDRESS         DT_REG_ADDR_BY_IDX(DT_NODELABEL(dtcm), 0)
#define REGION_DTCM_SIZE                 \
			REGION_CUSTOMED_MEMORY_SIZE(MEMORY_REGION_SIZE_KB(DTCM_SIZE))
#define REGION_OCRAM2_BASE_ADDRESS   DT_REG_ADDR_BY_IDX(DT_NODELABEL(ocram2), 0)
#define REGION_OCRAM2_SIZE           \
			REGION_CUSTOMED_MEMORY_SIZE(MEMORY_REGION_SIZE_KB(OCRAM2_SIZE))
#define REGION_PERIPHERAL_BASE_ADDRESS   DT_REG_ADDR_BY_IDX(DT_NODELABEL(peripheral), 0)
#define REGION_PERIPHERAL_SIZE           \
			REGION_CUSTOMED_MEMORY_SIZE(MEMORY_REGION_SIZE_KB(PERIPHERAL_SIZE))

static const struct arm_mpu_region mpu_regions[] = {
	/*
	 * Add "UNMAPPED" region to deny access to whole address space to
	 * workaround speculative prefetch.
	 * Refer to Arm errata 1013783-B for more details.
	 */
	MPU_REGION_ENTRY("UNMAPPED", 0,
			{REGION_4G | MPU_RASR_XN_Msk | P_NA_U_NA_Msk}),

#ifdef CONFIG_XIP
	MPU_REGION_ENTRY("FLASH", CONFIG_FLASH_BASE_ADDRESS,
			 REGION_FLASH_ATTR(REGION_CUSTOMED_MEMORY_SIZE(CONFIG_FLASH_SIZE))),
#endif

#if DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(itcm))
	MPU_REGION_ENTRY("ITCM", REGION_ITCM_BASE_ADDRESS,
			 REGION_FLASH_ATTR(REGION_ITCM_SIZE)),
#endif

#if DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(dtcm))
	MPU_REGION_ENTRY("DTCM", REGION_DTCM_BASE_ADDRESS,
			 REGION_RAM_NOCACHE_ATTR(REGION_DTCM_SIZE)),
#endif

#if DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(ocram2))
	MPU_REGION_ENTRY("OCRAM2", REGION_OCRAM2_BASE_ADDRESS,
			 REGION_RAM_ATTR(REGION_OCRAM2_SIZE)),
#endif

	MPU_REGION_ENTRY("PERIPHERAL", REGION_PERIPHERAL_BASE_ADDRESS,
			 REGION_PPB_ATTR(REGION_PERIPHERAL_SIZE)),
};

const struct arm_mpu_config mpu_config = {
	.num_regions = ARRAY_SIZE(mpu_regions),
	.mpu_regions = mpu_regions,
};
