#ifndef NN_CUSTOM_DRIVER_H
#define NN_CUSTOM_DRIVER_H

#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/internal/syscall_handler.h>

#ifdef __cplusplus
extern "C" {
#endif

enum nn_driver_led_t {
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

typedef int (*custom_set_id)(const struct device *dev,
			     const uint32_t id);

typedef int (*custom_select_led)(const struct device *dev,
				 const enum nn_driver_led_t led);

__subsystem struct nn_driver_custom_api {
	custom_channel_get channel_get;
	custom_sample_fetch sample_fetch;
	custom_show_settings show_settings;
	custom_set_id set_id;
	custom_select_led select_led;
};

__syscall int channel_get(const struct device *dev,
                                        enum sensor_channel chan,
                                        struct sensor_value *val,
                                        const struct gpio_dt_spec *led);

static inline int z_impl_channel_get(const struct device *dev,
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

static inline int z_impl_sample_fetch(const struct device *dev,
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

static inline int z_impl_show_settings(const struct device *dev)
{
	const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
        if (api->show_settings == NULL) {
                return -ENOSYS;
        }
	return api->show_settings(dev);
}

__syscall int set_id(const struct device *dev,
		     const uint32_t id);

static inline int z_impl_set_id(const struct device *dev,
				const uint32_t id)
{
	const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
	if (api->set_id == NULL) {
                return -ENOSYS;
        }
	return api->set_id(dev, id);
}

__syscall int select_led(const struct device *dev,
			 const enum nn_driver_led_t led);

static inline int z_impl_select_led(const struct device *dev,
				    const enum nn_driver_led_t led)
{
	const struct nn_driver_custom_api *api = (const struct nn_driver_custom_api *)dev->api;
	if (api->select_led == NULL) {
                return -ENOSYS;
        }
	return api->select_led(dev, led);
}

#include <zephyr/syscalls/nn-custom-driver.h>

#ifdef __cplusplus
}
#endif

#endif // NN_CUSTOM_DRIVER_H
