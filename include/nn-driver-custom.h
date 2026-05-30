#ifndef NN_DRIVER_CUSTOM_H
#define NN_DRIVER_CUSTOM_H

#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

enum nn_driver_led {
	NN_DRIVER_LED_NONE,
	NN_DRIVER_LED_1,
	NN_DRIVER_LED_2,
	NN_DRIVER_LED_BOTH,
};

struct nn_device_data {
	uint32_t active_led;
	uint32_t id;
};

typedef int (*custom_channel_get)(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val,
                                  const struct gpio_dt_spec *led);

typedef int (*custom_sample_fetch)(const struct device *dev,
                                   enum sensor_channel chan,
                                   const struct gpio_dt_spec *led);

typedef int (*custom_show_settings)(const struct device *dev);

__subsystem struct nn_driver_custom_api {
	custom_channel_get channel_get;
	custom_sample_fetch sample_fetch;
	custom_show_settings show_settings;
};

__syscall int channel_get(const struct device *dev,
                                        enum sensor_channel chan,
                                        struct sensor_value *val,
                                        const struct gpio_dt_spec *led);

static inline int channel_get(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val,
				const struct gpio_dt_spec *led)
{
        const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
        if (api->channel_get == NULL) {
                return -ENOSYS;
        }
	return api->channel_get(dev, chan, val, led);
}

__syscall int sample_fetch(const struct device *dev,
				enum sensor_channel chan,
				const struct gpio_dt_spec *led);

static inline int sample_fetch(const struct device *dev,
				enum sensor_channel chan,
				const struct gpio_dt_spec *led)
{
	const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
        if (api->sample_fetch == NULL) {
                return -ENOSYS;
        }
	return api->sample_fetch(dev, chan, led);
}

__syscall int show_settings(const struct device *dev);

static inline int show_settings(const struct device *dev)
{
	const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
        if (api->show_settings == NULL) {
                return -ENOSYS;
        }
	return api->show_settings(dev);
}

#include <syscalls/nn-driver-custom.h>

#endif // NN_DRIVER_CUSTOM_H
