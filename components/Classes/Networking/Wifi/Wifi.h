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
#include "protocol_examples_common.h"
#include "esp_eth.h"

#ifndef COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_
#define COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_

class Wifi {
public:
	Wifi();
	void connectNetwork();
	bool isConnected;
};

#endif /* COMPONENTS_CLASSES_NETWORKING_WIFI_WIFI_H_ */
