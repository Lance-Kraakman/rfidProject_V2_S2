/*
 * MqttService.h
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */

#ifndef COMPONENTS_CLASSES_NETWORKING_MQTTSERVICE_MQTTSERVICE_H_
#define COMPONENTS_CLASSES_NETWORKING_MQTTSERVICE_MQTTSERVICE_H_

#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include <string>

#define MQTT_SERVICE "MQTT SERVICE"
#define MQTT_INITIALIZED 1
#define MQTT_UNINITIALIZED 0

/** Provides abstraction to C functions for the mqtt service, Essentially a wrapper class with some abstraction.
 *
 */
class MqttService {
private:

	esp_mqtt_client_config_t mqttConfiguration;
	int initialized;
public:
	static esp_mqtt_client_handle_t client;
	MqttService();
	MqttService(esp_mqtt_client_config_t config);
	void InitMqttService();
	void StartMqttService(int networkStarted);
	void CloseMqttService();
	void addSubscriber(std::string topic, int qaulity_of_service);
	void publishMessage(std::string topic, std::string message, int qaulity_of_service);

	void setInit(int init);
	int isInit();

	void setConfiguration(esp_mqtt_client_config_t Configuration);
	esp_mqtt_client_config_t getConfiguration();
	void setClient(esp_mqtt_client_handle_t &Configuration);
	esp_mqtt_client_handle_t getClient();

	/* This allows us to add event handlers */
	void registerEventHandler(esp_event_handler_t eventHandler);
	static void mqqt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

};


#endif /* COMPONENTS_CLASSES_NETWORKING_MQTT_MQTTSERVICE_H_ */
