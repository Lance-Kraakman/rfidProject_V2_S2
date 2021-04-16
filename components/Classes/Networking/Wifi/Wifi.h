/*
 * Wifi.h
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */
#include <sys/param.h>
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "protocol_examples_common.h"
#include "esp_eth.h"
#include "esp_log.h"

#include "freertos/event_groups.h"

#ifndef COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_
#define COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_

#define EXAMPLE_ESP_WIFI_SSID CONFIG_EXAMPLE_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS CONFIG_EXAMPLE_WIFI_PASSWORD


#define EXAMPLE_ESP_MAXIMUM_RETRY  8
#define TAG_WIFI "WIFI"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1



/** \brief Handles WiFi connection, provides abstraction to wifi functionallity TODO
 *
 */
class Wifi {
public:
	static int s_retry_num;
	static EventGroupHandle_t s_wifi_event_group;

	Wifi();
	void connectNetwork();
	static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
	void wifi_init_sta(void);
};

#endif /* COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_ */
