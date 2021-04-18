/*
 * WebServer.cpp
 *
 *  Created on: 18/04/2021
 *      Author: lance
 */

#include "WebServer.h"


WebServer::WebServer() : Wifi() {
	// connect handler for https server
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP, &this->connect_handler,NULL));
}

void WebServer::connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {

		httpd_handle_t* server = (httpd_handle_t*) arg;
		if (*server == NULL) {
			*server = start_webserver();
		}
}

httpd_handle_t WebServer::start_webserver(void)
{
//    httpd_handle_t server = NULL;
//
//    // Start the httpd server
//    ESP_LOGI(TAG, "Starting server");
//
//    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
//
//    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
//    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
//    conf.cacert_pem = cacert_pem_start;
//    conf.cacert_len = cacert_pem_end - cacert_pem_start;
//
//    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
//    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
//    conf.prvtkey_pem = prvtkey_pem_start;
//    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;
//
//    esp_err_t ret = httpd_ssl_start(&server, &conf);
//    if (ESP_OK != ret) {
//        ESP_LOGI(TAG, "Error starting server!");
//        return NULL;
//    }
//
//    // Set URI handlers
//    ESP_LOGI(TAG, "Registering URI handlers");
//    httpd_register_uri_handler(server, &root);
//    return server;
	return NULL;
}

void WebServer::stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
//    httpd_ssl_stop(server);
}

void WebServer::disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        stop_webserver(*server);
        *server = NULL;
    }
}

