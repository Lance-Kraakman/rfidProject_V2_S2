/*
 * Wifi.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "Wifi.h"

Wifi::Wifi() {
	// TODO Auto-generated constructor stub
	this->isConnected = false;
}

/**
 * @brief Configure Wi-Fi or Ethernet, connect, wait for IP
 *
 * This all-in-one helper function is used in protocols examples to
 * reduce the amount of boilerplate in the example.
 *
 * It is not intended to be used in real world applications.
 * See examples under examples/wifi/getting_started/ and examples/ethernet/
 * for more complete Wi-Fi or Ethernet initialization code.
 *
 * Read "Establishing Wi-Fi or Ethernet Connection" section in
 * examples/protocols/README.md for more information about this function.
 *
 * @return ESP_OK on successful connection
 */
void Wifi::connectNetwork() {

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_err_t ConnectSuccess = example_connect();
	ESP_ERROR_CHECK(ConnectSuccess);

	if (ConnectSuccess) {
		this->isConnected = true;
	}

}
