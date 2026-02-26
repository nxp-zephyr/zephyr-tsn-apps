#!/bin/sh

if [ -z "$ARMGCC_DIR" ]; then
    echo "ARMGCC_DIR not defined."
    exit 1;
fi

export ZEPHYR_TOOLCHAIN_VARIANT=cross-compile
export CROSS_COMPILE=$ARMGCC_DIR/bin/arm-none-eabi-

west build -p always -b mimxrt1180_evk/mimxrt1189/cm33 . -d build_rt1180_cm33
