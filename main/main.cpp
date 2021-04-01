#include "main.h"

#define CONFIG_BROKER_LOCAL "mqtt://192.168.1.73:1883"

extern "C" {
	void app_main();
}


// Global Queue Handles
//xQueueHandle xQueueRfid = NULL;
//
void configNetwork() {

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(example_connect());

}

void app_main(void)
{
	// Cry
	configNetwork();

	// Employee/Tag classes have lists etc but dont "Have" a messaging service

	// add subscriptions

	message recvdMessage;

	while(1) {
		vTaskDelay(500/portTICK_RATE_MS);

	}

}
