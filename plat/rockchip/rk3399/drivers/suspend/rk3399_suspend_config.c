#include "rk3399_suspend_config.h"

#include <assert.h>
#include <errno.h>
#include <platform_def.h>
#include <plat_private.h>

rk_suspendmode_t  rk_suspendmode_config = 0;
rk_wakeupsource_t rk_wakeupsource_config = 0;
int rk_suspend_debug_config = 0;
int rk_pwm_config = 0;
int rk_apios_config = 0;
int rk_virtualpoweroff_config = 0;
int rk_gpio_power_config[0xa] = {0};


