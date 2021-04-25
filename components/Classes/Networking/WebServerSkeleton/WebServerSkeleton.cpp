/*
 * WebServer.cpp
 *
 *  Created on: 19/04/2021
 *      Author: lance
 */
#include "../WebServerSkeleton/WebServerSkeleton.h"

httpd_handle_t WebServerSkeleton::server = NULL;

WebServerSkeleton::WebServerSkeleton() : Wifi() {
	//Conect to internet
	this->connectAsStation();

	// connect handler for https server
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP, &this->connect_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &this->disconnect_handler, NULL));

    WebServerSkeleton::start_webserver();
}

httpd_handle_t WebServerSkeleton::start_webserver(void)
{

    // Connect to the network

	// Start the httpd server
    ESP_LOGI(TAG_WS, "Starting server");

    /* TO DO Change to HTTPs */

    //httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
    httpd_config_t conf = HTTPD_DEFAULT_CONFIG();

//    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
//    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
//    conf.cacert_pem = cacert_pem_start;
//    conf.cacert_len = cacert_pem_end - cacert_pem_start;
//
//    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
//    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
//    conf.prvtkey_pem = prvtkey_pem_start;
//    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    //esp_err_t ret = httpd_ssl_start(&WebServer::server, &conf);
    esp_err_t ret = httpd_start(&WebServerSkeleton::server, &conf);

    if (ESP_OK != ret) {
        ESP_LOGI(TAG_WS, "Error starting server!");
        return NULL;
    }
    return WebServerSkeleton::server;
}

void WebServerSkeleton::stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    //httpd_ssl_stop(server);
	httpd_stop(WebServerSkeleton::server);
}

void WebServerSkeleton::connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {

		httpd_handle_t* server = (httpd_handle_t*) arg;
		if (*server == NULL) {
			*server = WebServerSkeleton::start_webserver();
		}
}

void WebServerSkeleton::disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        stop_webserver(*server);
        *server = NULL;
    }
}
