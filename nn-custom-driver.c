/**
 * Iomico 2026 Zephyr Course sample driver source file
 */

#define DT_DRV_COMPAT nn_driver

#include "./include/nn-driver/nn-custom-driver.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nn_driver, LOG_LEVEL_INF);

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(driver_led1), gpios);

//----------------------------------------------------------------------
// - SECTION - Zephyr sensor API implemenations
//----------------------------------------------------------------------

static int nn_driver_custom_channel_get(const struct device *dev,
					enum sensor_channel chan,
					struct sensor_value *val,
					const struct gpio_dt_spec *led)
{
	int rc = gpio_pin_set_dt(led, 1);
	return rc;
}

static int nn_driver_custom_sample_fetch(const struct device *dev,
					enum sensor_channel chan,
					const struct gpio_dt_spec *led)
{
	int rc = gpio_pin_set_dt(led, 0);
	return rc;
}

int nn_driver_custom_show_settings(const struct device *dev)
{
	struct nn_device_data *data = dev->data;

 	LOG_INF("nn_driver extended API settings");
 	LOG_INF("active_led: %d", data->active_led);
 	LOG_INF("id: . . . . %d", data->id);

	return 0;
}

#if 0
static DEVICE_API(sensor, api_nn_driver) = {
	.channel_get = nn_driver_channel_get,
	.sample_fetch = nn_driver_sample_fetch,
};
#else
static const struct nn_driver_custom_api nn_driver_api = {
	.channel_get = nn_driver_custom_channel_get;
	.sample_fetch = nn_driver_custom_sample_fetch;
	.show_settings = nn_driver_custom_show_settings;
};
#endif

//----------------------------------------------------------------------
// - SECTION - initialization code
//----------------------------------------------------------------------

static int init(const struct device *dev)
{
	LOG_INF("nn_driver init() function called");

	if (!gpio_is_ready_dt(&led)) {
		return -EIO;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
		return -EIO;
	}

#if 0 // NOT THE CORRECT WAY TO EXTEND A STANDARD DEVICE API:
	const struct nn_device_config *config = dev->config;
	if (config == NULL) {
		LOG_ERR("nn driver device config found NULL!");
	} else {
		LOG_INF("nn driver extended api:");
		LOG_INF("show_settings = 0x%08X", (uint32_t)&config->show_settings);
		LOG_INF("set_active_led = 0x%08X", (uint32_t)&config->set_active_led);
	}
#endif

	return 0;
}

// DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_nn_driver);

#define NN_DEVICE_DATA(inst) \
	static struct nn_device_data nn_data_##inst = { 0 };

#define NN_DEVICE_CONFIG(inst) \
	static struct nn_device_config nn_config_##inst = { 0 };
#if 0
//	static struct nn_device_config nn_config_##inst = { \
//		.show_settings = n n_driver_show_settings, \
//		.set_active_led = NULL, \
//		.set_id = NULL, \
//	};
#endif

#define NN_DEVICE_INIT(inst)                                              \
	NN_DEVICE_DATA(inst)                                              \
	NN_DEVICE_CONFIG(inst)                                            \
	DEVICE_DT_INST_DEFINE(                                            \
			0,                 /* device instance          */ \
			init,              /* driver init function     */ \
			NULL,              /* power management context */ \
			&nn_data_##inst,   /* device data              */ \
			&nn_config_##inst, /* device config            */ \
			POST_KERNEL,       /* initialization level     */ \
			80,                /* initialization priority  */ \
			&api_nn_driver     /* driver API               */ \
		 );

#define NN_DEFINE(inst) \
	NN_DEVICE_INIT(inst)

DT_INST_FOREACH_STATUS_OKAY(NN_DEFINE)
