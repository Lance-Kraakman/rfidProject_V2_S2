/*
 * input_processor.h
 *
 *  Created on: 9/03/2021
 *      Author: lance
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "string.h"



#include "esp_log.h"
#include "../../Drivers/esp-idf-rc522/rc522.h"

//Define Required Queues etc
extern xQueueHandle xQueueRfid;

#define TAG_IP "INPUT_PROCESSOR_TAG"

#ifndef COMPONENTS_PROCESSING_INPUTPROCESSING_INPUTPROCESSOR_H_
#define COMPONENTS_PROCESSING_INPUTPROCESSING_INPUTPROCESSOR_H_

QueueHandle_t buttonInputQueue;

// Function configures input processor settings
void configInputProcessor();

// Function starts the input processor task
void startInputProcessor();

typedef struct {
	uint8_t uuid[5];
	int64_t time;

} rfid_tag_item;

struct buttonInput {
	int number;
};

struct touchInput {
	int x, y;
};

void configRFID();





#endif /* COMPONENTS_PROCESSING_INPUTPROCESSING_INPUT_PROCESSOR_H_ */
