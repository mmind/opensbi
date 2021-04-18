/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2022 Heiko Stuebner <heiko@sntech.de>
 *
 * Authors:
 *   Heiko Stuebner <heiko@sntech.de>
 */

#include <platform_override.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_trap.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/fdt/fdt_fixup.h>

#define CSR_MCOUNTERWEN  0x7c9
static void thead_c9xx_pmu_init(void)
{
	unsigned long interrupts;

	interrupts = csr_read(CSR_MIDELEG) | (1 << 17);
	csr_write(CSR_MIDELEG, interrupts);

	/* CSR_MCOUNTEREN has already been set in mstatus_init() */
	csr_write(CSR_MCOUNTERWEN, 0xffffffff);
	csr_write(CSR_MHPMEVENT3, 1);
	csr_write(CSR_MHPMEVENT4, 2);
	csr_write(CSR_MHPMEVENT5, 3);
	csr_write(CSR_MHPMEVENT6, 4);
	csr_write(CSR_MHPMEVENT7, 5);
	csr_write(CSR_MHPMEVENT8, 6);
	csr_write(CSR_MHPMEVENT9, 7);
	csr_write(CSR_MHPMEVENT10, 8);
	csr_write(CSR_MHPMEVENT11, 9);
	csr_write(CSR_MHPMEVENT12, 10);
	csr_write(CSR_MHPMEVENT13, 11);
	csr_write(CSR_MHPMEVENT14, 12);
	csr_write(CSR_MHPMEVENT15, 13);
	csr_write(CSR_MHPMEVENT16, 14);
	csr_write(CSR_MHPMEVENT17, 15);
	csr_write(CSR_MHPMEVENT18, 16);
	csr_write(CSR_MHPMEVENT19, 17);
	csr_write(CSR_MHPMEVENT20, 18);
	csr_write(CSR_MHPMEVENT21, 19);
	csr_write(CSR_MHPMEVENT22, 20);
	csr_write(CSR_MHPMEVENT23, 21);
	csr_write(CSR_MHPMEVENT24, 22);
	csr_write(CSR_MHPMEVENT25, 23);
	csr_write(CSR_MHPMEVENT26, 24);
	csr_write(CSR_MHPMEVENT27, 25);
	csr_write(CSR_MHPMEVENT28, 26);
}

static void thead_c9xx_pmu_map(unsigned long idx, unsigned long event_id)
{
	if (idx >= 3 && idx <= 31)
		csr_write_num(CSR_MHPMEVENT3 + idx - 3, event_id);
}

static void thead_c9xx_pmu_set(unsigned long type, unsigned long idx,
			       unsigned long event_id)
{
	switch (type) {
	case 2:
		thead_c9xx_pmu_map(idx, event_id);
		break;
	default:
		thead_c9xx_pmu_init();
		break;
	}
}
#include <sbi/sbi_console.h>

static int thead_c9xx_vendor_ext_provider(long extid, long funcid,
					  const struct sbi_trap_regs *regs,
					  unsigned long *out_value,
					  struct sbi_trap_info *out_trap)
{
sbi_printf("%s\n", __func__);
	switch (extid) {
	case 0x09000001:
		thead_c9xx_pmu_set(regs->a0, regs->a1, regs->a2);
		break;
	default:
		while(1);
	}
	return 0;
}

static const struct fdt_match thead_c9xx_match[] = {
	{ .compatible = "allwinner,sun20i-d1" },
	{ },
};

const struct platform_override thead_c9xx = {
	.match_table = thead_c9xx_match,
	.vendor_ext_provider = thead_c9xx_vendor_ext_provider,
};
