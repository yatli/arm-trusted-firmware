/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/mmio.h>
#include <rk3399_suspend_config.h>

#include <cdn_dp.h>
#include <dfs.h>
#include <plat_sip_calls.h>
#include <rockchip_sip_svc.h>

#define RK_SIP_DDR_CFG		  0x82000008
#define DRAM_INIT		0x00
#define DRAM_SET_RATE		0x01
#define DRAM_ROUND_RATE		0x02
#define DRAM_SET_AT_SR		0x03
#define DRAM_GET_BW		0x04
#define DRAM_GET_RATE		0x05
#define DRAM_CLR_IRQ		0x06
#define DRAM_SET_PARAM		0x07
#define DRAM_SET_ODT_PD		0x08

#define RK_SIP_HDCP_CONTROL	0x82000009
#define RK_SIP_HDCP_KEY_DATA64	0xC200000A

#define RK_SIP_SUSPEND_MODE 0x82000003
/* SIP_SUSPEND_MODE32 call types */
#define   SUSPEND_MODE_CONFIG		0x01
#define   WKUP_SOURCE_CONFIG		0x02
#define   PWM_REGULATOR_CONFIG		0x03
#define   GPIO_POWER_CONFIG		0x04
#define   SUSPEND_DEBUG_ENABLE		0x05
#define   APIOS_SUSPEND_CONFIG		0x06
#define   VIRTUAL_POWEROFF		0x07


uint32_t ddr_smc_handler(uint64_t arg0, uint64_t arg1,
			 uint64_t id, uint64_t arg2)
{
	switch (id) {
	case DRAM_SET_RATE:
		return ddr_set_rate((uint32_t)arg0);
	case DRAM_ROUND_RATE:
		return ddr_round_rate((uint32_t)arg0);
	case DRAM_GET_RATE:
		return ddr_get_rate();
	case DRAM_SET_ODT_PD:
		dram_set_odt_pd(arg0, arg1, arg2);
		break;
	default:
		break;
	}

	return 0;
}

uint32_t suspend_mode_handler(uint64_t ctrl, uint64_t config1, uint64_t config2)
{
  switch(ctrl) {
    case SUSPEND_MODE_CONFIG:
      rk_suspendmode_config = config1;
      break;
    case WKUP_SOURCE_CONFIG:
      rk_wakeupsource_config = config1;
      break;
    case PWM_REGULATOR_CONFIG:
      rk_pwm_config = config1;
      break;
    case GPIO_POWER_CONFIG:
      if (config1 < sizeof(rk_gpio_power_config) / sizeof(int)) {
        rk_gpio_power_config[config1] = config2;
      }
      break;
    case SUSPEND_DEBUG_ENABLE:
      rk_suspend_debug_config = config1;
      break;
    case APIOS_SUSPEND_CONFIG:
      rk_apios_config = config1;
      break;
    case VIRTUAL_POWEROFF:
      if (config1 != 0) {
        return 0;
      }
      rk_virtualpoweroff_config = config2;
      break;
    default:
      break;
  }

  return 0;
}

uintptr_t rockchip_plat_sip_handler(uint32_t smc_fid,
				    u_register_t x1,
				    u_register_t x2,
				    u_register_t x3,
				    u_register_t x4,
				    void *cookie,
				    void *handle,
				    u_register_t flags)
{
#ifdef PLAT_RK_DP_HDCP
	uint64_t x5, x6;
#endif

	switch (smc_fid) {
	case RK_SIP_DDR_CFG:
		SMC_RET1(handle, ddr_smc_handler(x1, x2, x3, x4));
#ifdef PLAT_RK_DP_HDCP
	case RK_SIP_HDCP_CONTROL:
		SMC_RET1(handle, dp_hdcp_ctrl(x1));
	case RK_SIP_HDCP_KEY_DATA64:
		x5 = read_ctx_reg(get_gpregs_ctx(handle), CTX_GPREG_X5);
		x6 = read_ctx_reg(get_gpregs_ctx(handle), CTX_GPREG_X6);
		SMC_RET1(handle, dp_hdcp_store_key(x1, x2, x3, x4, x5, x6));
#endif
  case RK_SIP_SUSPEND_MODE:
    SMC_RET1(handle, suspend_mode_handler(x1, x2, x3));
	default:
		ERROR("%s: unhandled SMC (0x%x)\n", __func__, smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}
}
