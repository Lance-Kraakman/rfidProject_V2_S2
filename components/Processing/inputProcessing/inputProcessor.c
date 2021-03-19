/*
 * input_processor.cpp
 *
 *  Created on: 9/03/2021
 *      Author: lance
 */

#include "inputProcessor.h"

// Static Function Declarations
static void configButtonInputs(gpio_num_t *buttonList, int buttonListLength);
static void pollButtonInputs(void* arg);

// Static Variable Declarations
static xQueueHandle gpioEventQueue = NULL;
static gpio_num_t gpioButtonInputList[] = {GPIO_NUM_18, GPIO_NUM_26};

#define BUTTON_INPUT_LIST_LENGTH 2

static void tag_handler(uint8_t* serial_no) {
	rfid_tag_item dummyTag;
	// Loop for testing
    for(int i = 0; i < 5; i++) {
        printf("%#x ", serial_no[i]);
    }
    dummyTag.time = esp_timer_get_time();
    memcpy(dummyTag.uuid, serial_no, 4*sizeof(uint8_t));
    xQueueSend(xQueueRfid, &dummyTag, 1000);

    printf("\n ------ FIN ----- \n");
}

xQueueHandle getGpioEventQueue() { // Allows us to get out event queues in other scopes
	return gpioEventQueue;
}

void configInputProcessor() {
	//Configure Buttons and Queues
	configRFID();
	gpioEventQueue = xQueueCreate(20, sizeof(uint32_t)); // Configure Event Queue
}

void startInputProcessor() {
	configInputProcessor();
	xTaskCreate(pollButtonInputs, "pollButtonInputs", 4096, NULL, 1, NULL);
}

// Configures Button inputs
void configButtonInputs(gpio_num_t *buttonList, int buttonListLength) {

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;

	// Add pins to GPIO bit mask
	gpio_num_t gpioInputPin;
	uint64_t gpioBitMask = 0;

	for (int i = 0; i < buttonListLength; i++) {
		gpioInputPin =(gpio_num_t) *(buttonList+i);
		gpioBitMask += (1<<gpioInputPin);
	}

	vTaskDelay(50/portTICK_RATE_MS);

	io_conf.pin_bit_mask = gpioBitMask;
	gpio_config(&io_conf);

}

static void pollButtonInputs(void* arg)
{
	// Task Setup
	configButtonInputs(gpioButtonInputList, BUTTON_INPUT_LIST_LENGTH);

    while(1) {

    	// Check for button Inputs
    	for (int i = 0; i < BUTTON_INPUT_LIST_LENGTH; i++) {
    		if (gpio_get_level(gpioButtonInputList[i]) == 0) { // If Button Pressed
    			// Add Button input to queue. Now we need to act on button inputs
    			xQueueSend(gpioEventQueue, &gpioButtonInputList[i], 0);
    			ESP_LOGI(TAG_IP,"Button Pressed: %d",(int) gpioButtonInputList[i]);
    		}
    	}
    	//Task Delay
        vTaskDelay(30/portTICK_RATE_MS);
    }
}

// Config RFID Inputs
void configRFID() {

	const rc522_start_args_t start_args = {
	        .miso_io  = 17,
	        .mosi_io  = 16,
	        .sck_io   = 15,
	        .sda_io   = 14,
	        .callback = &tag_handler
	    };

	    rc522_start(start_args);
}



