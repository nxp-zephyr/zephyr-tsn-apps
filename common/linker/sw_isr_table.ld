	/*
	 * The software ISR table source is generated during build time.
	 * To relocate it into TCM, capture it using a linker script snippet
	 * via zephyr_linker_sources() instead of zephyr_code_relocate().
	 * By default, Zephyr places the original software ISR table section
	 * in RAM through its include/zephyr/linker/common-ram.ld
	 */
	. = ALIGN(CONFIG_ARCH_SW_ISR_TABLE_ALIGN);
	*(_SW_ISR_TABLE_SECTION_SYMS)
