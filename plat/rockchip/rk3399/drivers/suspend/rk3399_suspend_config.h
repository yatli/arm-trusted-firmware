#ifndef __RK3399_SUSPEND_CONFIG_H
#define __RK3399_SUSPEND_CONFIG_H

typedef enum {
  RK_SM_SLP_ARMPD     = 0x2,
  RK_SM_DDR_RET       = 0x8,
  RK_SM_SLP_PLLPD     = 0x10,
  RK_SM_OSC_DIS       = 0x20,
  RK_SM_SLP_CENTER_PD = 0x40,
  RK_SM_AP_PWROFF     = 0x80
} rk_suspendmode_t;

typedef enum {
  RK_WS_CLUSTER_L_INTERRUPT = 0x01,
  RK_WS_CLUSTER_B_INTERRUPT = 0x02,
  RK_WS_GPIO_INTERRUPT      = 0x04,
  RK_WS_SDIO_INTERRUPT      = 0x08,
  RK_WS_SDMMC_INTERRUPT     = 0x10,
  RK_WS_TIMER_INTERRUPT     = 0x40,
  RK_WS_USBDEV_INTERRUPT    = 0x80,
  RK_WS_M0_SOFT_INTERRUPT   = 0x100,
  RK_WS_M0_WDT_INTERRUPT    = 0x200,
  RK_WS_TIMER_OUT_INTERRUPT = 0x400,
  RK_WS_PWM_INTERRUPT       = 0x800,
  RK_WS_PCIE_INTERRUPT      = 0x2000,
} rk_wakeupsource_t;


extern rk_suspendmode_t rk_suspendmode_config;
extern rk_wakeupsource_t rk_wakeupsource_config;
extern int rk_suspend_debug_config;
extern int rk_pwm_config;
extern int rk_virtualpoweroff_config;
extern int rk_gpio_power_config[0xa];
extern int rk_apios_config;

#endif
