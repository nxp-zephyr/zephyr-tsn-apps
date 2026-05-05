/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/toolchain.h>

/* Use no-tree-loop-distribute-patterns compilation flag to prevent optimizations resulting in
 * memcpy/memset libc calls during early boot when these functions are targeted to be relocated.
 */
__attribute__((optimize("no-tree-loop-distribute-patterns")))
void *arch_early_memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

__attribute__((optimize("no-tree-loop-distribute-patterns")))
void *arch_early_memset(void *dest, int c, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    unsigned char val = (unsigned char)c;

    while (n--) {
        *d++ = val;
    }

    return dest;
}
