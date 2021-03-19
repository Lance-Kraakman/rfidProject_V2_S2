#include "main.h"

// Global Queue Handles
xQueueHandle xQueueRfid = NULL;

void app_main(void)
{
	// Create Queues, Semaphores and Message Boxes
	xQueueRfid = xQueueCreate(25,sizeof(rfid_tag_item));

	rfid_tag_item dummyTag;

	uint8_t dummyArray[] = {1,2,3,4};
	memcpy(dummyTag.uuid, &dummyArray, 4*sizeof(uint8_t));
	dummyTag.time = 4521;

	for (int i=0; i<10;i++) {
		xQueueSend(xQueueRfid, &dummyTag, 1000);
	}

	// Start Processing Tasks

	// Start Network Processor
	startNetworkProcessor();
	ESP_LOGI(TAG_NP, "START NETWORK PROCESSOR FINISHED \n");

	// Start Input Processor
	startInputProcessor(); // Somewhere in start input processor memory may not be allocated correctly -> Might just be on INIT//
	ESP_LOGI(TAG_IP, "START INPUT PROCESSOR FINISHED \n");


	while(1) {
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
