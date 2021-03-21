/*
 * networkProcessor.h
 *
 *  Created on: 10/03/2021
 *      Author: lance
 */

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "protocol_examples_common.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include "freertos/FreeRTOS.h"
#include "cJSON.h"

#include <esp_https_server.h>

#include "../inputProcessing/inputProcessor.h"

#define TAG_NP "NETWORK PROCESSOR"

#ifndef COMPONENTS_PROCESSING_NETWORKPROCESSING_NETWORKPROCESSOR_H_
#define COMPONENTS_PROCESSING_NETWORKPROCESSING_NETWORKPROCESSOR_H_

//Define Required Queues etc
extern xQueueHandle xQueueRfid;

void startNetworkProcessor();
void configNetwork();
void start_mdns_service();
void refresh_mdns_service();
char *getTagsFromQueue();

#endif /* COMPONENTS_PROCESSING_NETWORKPROCESSING_NETWORKPROCESSOR_H_ */
