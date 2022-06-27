/*
 * Copyright (c) 2017-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include <assert.h>
#include <stdio.h>

#include <common/debug.h>
#include <plat/common/platform.h>

#include <lib/mmio.h>
#include <drivers/delay_timer.h>

/* Set the default maximum log level to the `LOG_LEVEL` build flag */
static unsigned int max_log_level = LOG_LEVEL;

/*
 * The common log function which is invoked by TF-A code.
 * This function should not be directly invoked and is meant to be
 * only used by the log macros defined in debug.h. The function
 * expects the first character in the format string to be one of the
 * LOG_MARKER_* macros defined in debug.h.
 */
void tf_log(const char *fmt, ...)
{
	unsigned int log_level;
	va_list args;
	const char *prefix_str;

	/* We expect the LOG_MARKER_* macro as the first character */
	log_level = fmt[0];

	/* Verify that log_level is one of LOG_MARKER_* macro defined in debug.h */
	assert((log_level > 0U) && (log_level <= LOG_LEVEL_VERBOSE));
	assert((log_level % 10U) == 0U);

	if (log_level > max_log_level)
		return;

	prefix_str = plat_log_get_prefix(log_level);

  /*
#define REG_MSK_SHIFT	16
#define BITS_SHIFT(bits, shift)	(bits << (shift))
#define GRF_GPIO4B_IOMUX	0xe024
#define BITS_WITH_WMASK(bits, msk, shift)\
	(BITS_SHIFT(bits, shift) | BITS_SHIFT(msk, (shift + REG_MSK_SHIFT)))
#define GRF_IOMUX_2BIT_MASK     0x3
#define GRF_GPIO4B0_IOMUX_SHIFT      0
#define GRF_GPIO4B1_IOMUX_SHIFT      2
  // HACK
  // bring back uart2 iomux...
  // 0x2 is uartdbga_sin/sout
  mmio_write_32(GRF_BASE + GRF_GPIO4B_IOMUX,
      BITS_WITH_WMASK(0x2, GRF_IOMUX_2BIT_MASK, GRF_GPIO4B0_IOMUX_SHIFT) |
      BITS_WITH_WMASK(0x2, GRF_IOMUX_2BIT_MASK, GRF_GPIO4B1_IOMUX_SHIFT));
      */

	while (*prefix_str != '\0') {
		(void)putchar(*prefix_str);
		prefix_str++;
	}

	va_start(args, fmt);
	(void)vprintf(fmt + 1, args);
	va_end(args);

  /*
  // HACK
  // bring back sdmmc iomux
  // 0x1 is sdmmc_data0/1
  for(int i = 0; i < 1000000; ++i) {
    // hope it works...
  }

  mmio_write_32(GRF_BASE + GRF_GPIO4B_IOMUX,
      BITS_WITH_WMASK(0x1, GRF_IOMUX_2BIT_MASK, GRF_GPIO4B0_IOMUX_SHIFT) |
      BITS_WITH_WMASK(0x1, GRF_IOMUX_2BIT_MASK, GRF_GPIO4B1_IOMUX_SHIFT));
  */
}

void tf_log_newline(const char log_fmt[2])
{
	unsigned int log_level = log_fmt[0];

	/* Verify that log_level is one of LOG_MARKER_* macro defined in debug.h */
	assert((log_level > 0U) && (log_level <= LOG_LEVEL_VERBOSE));
	assert((log_level % 10U) == 0U);

	if (log_level > max_log_level)
		return;

	putchar('\n');
}

/*
 * The helper function to set the log level dynamically by platform. The
 * maximum log level is determined by `LOG_LEVEL` build flag at compile time
 * and this helper can set a lower (or equal) log level than the one at compile.
 */
void tf_log_set_max_level(unsigned int log_level)
{
	assert(log_level <= LOG_LEVEL_VERBOSE);
	assert((log_level % 10U) == 0U);

	/* Cap log_level to the compile time maximum. */
	if (log_level <= (unsigned int)LOG_LEVEL)
		max_log_level = log_level;
}
