/*
 * WebServer.h
 *
 *  Created on: 19/04/2021
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
#include "esp_https_server.h"
#include <esp_https_server.h>
#include "../Wifi/Wifi.h"

#define TAG_WS "WEB SERVER"

#ifndef COMPONENTS_CLASSES_NETWORKING_WEBSERVERSKELETON_WEBSERVERSKELETON_H_
#define COMPONENTS_CLASSES_NETWORKING_WEBSERVERSKELETON_WEBSERVERSKELETON_H_

/** Web Server defines event handlers for a web server. For implementation example see MyWebServer
 *
 */
class WebServerSkeleton : public Wifi {
private:
	static void disconnect_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
	static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
	static httpd_handle_t start_webserver(void);
	static void stop_webserver(httpd_handle_t server);
	static esp_err_t root_handler(httpd_req_t *req);
	static int s_servers;
protected:
    static httpd_handle_t server;
public:
    WebServerSkeleton();
};

#endif /* COMPONENTS_CLASSES_NETWORKING_WEBSERVER_WEBSERVER_H_ */
