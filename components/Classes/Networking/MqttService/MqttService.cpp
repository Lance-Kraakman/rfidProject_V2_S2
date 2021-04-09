/*
 * MqttService.cpp
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */

#include "MqttService.h"

esp_mqtt_client_handle_t MqttService::client;

MqttService::MqttService() {
	// TODO Auto-generated constructor stub
	this->client = NULL; // client config and client handles are differnet ! fuck
	this->initialized = 0;
}

MqttService::MqttService(esp_mqtt_client_config_t config) {
	this->setConfiguration(config);
	this->client = NULL;
}

/** Initializes the MQTT service.
 *
 */
void MqttService::InitMqttService() {
	esp_mqtt_client_config_t confCopy = this->getConfiguration();
	esp_mqtt_client_handle_t cli = esp_mqtt_client_init(&confCopy);
	this->setClient(cli);
	this->setInit(MQTT_INITIALIZED);
}

/** Starts the MQTT service.
 *
 * @param networkStarted
 */
void MqttService::StartMqttService(int networkStarted) {
	if (this->isInit()) {
		esp_mqtt_client_start(this->getClient());
	}
}

/** Closes the MQTT service.
 *
 */
void MqttService::CloseMqttService() {
	esp_mqtt_client_stop(this->getClient());
}

/** Adds a subscriber to the MQTT service.
 *
 * @param topic
 * @param qaulity_of_service
 */
void MqttService::addSubscriber(std::string topic, int qaulity_of_service) {

	int msg_id = esp_mqtt_client_subscribe(this->getClient(),topic.c_str(), qaulity_of_service);
	ESP_LOGI(MQTT_SERVICE, "sent subscribe successful, msg_id=%d", msg_id);

}

/** Publishes to the MQTT service.
 *
 * @param topic
 * @param message
 * @param qaulity_of_service
 */
void MqttService::publishMessage(std::string topic, std::string message, int qaulity_of_service) {

	int msg_id = esp_mqtt_client_publish(this->getClient(), topic.c_str(), message.c_str(), 0, qaulity_of_service, 0);
	ESP_LOGI(MQTT_SERVICE, "sent publish successful, msg_id=%d", msg_id);

}

/**
 *
 * @param Configuration
 */
void MqttService::setConfiguration(esp_mqtt_client_config_t Configuration) {
	this->mqttConfiguration = Configuration;
}

/**
 *
 * @return mqtt config params
 */
esp_mqtt_client_config_t MqttService::getConfiguration() {
	return this->mqttConfiguration;
}

/**
 *
 * @param Configuration
 */
void MqttService::setClient(esp_mqtt_client_handle_t &Configuration) {
	this->client = Configuration;
}

/**
 *
 * @return esp_mqtt_client_handle_t client object (static)
 */
esp_mqtt_client_handle_t MqttService::getClient() {
	return this->client;
}

/**
 *
 * @param eventHandler
 */
void MqttService::registerEventHandler(esp_event_handler_t eventHandler) {
	esp_mqtt_client_register_event((esp_mqtt_client_handle_t) this->getClient(),(esp_mqtt_event_id_t) ESP_EVENT_ANY_ID,(esp_event_handler_t) eventHandler,(void*) NULL);
}

/**
 *
 * @param init
 */
void MqttService::setInit(int init) {
	this->initialized = init;
}

/**
 *
 * @return initialized state
 */
int MqttService::isInit() {
	return this->initialized;
}

