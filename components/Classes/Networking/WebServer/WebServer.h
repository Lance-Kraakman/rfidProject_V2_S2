/*
 * WebServer.h
 *
 *  Created on: 18/04/2021
 *      Author: lance
 */

#include "../Wifi/Wifi.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_https_server.h>

#ifndef COMPONENTS_CLASSES_NETWORKING_WEBSERVER_WEBSERVER_H_
#define COMPONENTS_CLASSES_NETWORKING_WEBSERVER_WEBSERVER_H_

class WebServer : public Wifi {
private:
	static void disconnect_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
	static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
	static httpd_handle_t start_webserver(void);
	static void stop_webserver(httpd_handle_t server);
public:
	WebServer();
};

#endif /* COMPONENTS_CLASSES_NETWORKING_WEBSERVER_WEBSERVER_H_ */
