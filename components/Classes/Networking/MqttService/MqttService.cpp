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

void MqttService::InitMqttService() {
	esp_mqtt_client_config_t confCopy = this->getConfiguration();
	esp_mqtt_client_handle_t cli = esp_mqtt_client_init(&confCopy);
	this->setClient(cli);
	this->setInit(MQTT_INITIALIZED);
}

void MqttService::StartMqttService(int networkStarted) {
	if (this->isInit()) {
		esp_mqtt_client_start(this->getClient());
	}
}

void MqttService::CloseMqttService() {
	esp_mqtt_client_stop(this->getClient());
}

void MqttService::addSubscriber(std::string topic, int qaulity_of_service) {

	int msg_id = esp_mqtt_client_subscribe(this->getClient(),topic.c_str(), qaulity_of_service);
	ESP_LOGI(MQTT_SERVICE, "sent subscribe successful, msg_id=%d", msg_id);

}

void MqttService::publishMessage(std::string topic, std::string message, int qaulity_of_service) {

	int msg_id = esp_mqtt_client_publish(this->getClient(), topic.c_str(), message.c_str(), 0, qaulity_of_service, 0);
	ESP_LOGI(MQTT_SERVICE, "sent publish successful, msg_id=%d", msg_id);

}

void MqttService::setConfiguration(esp_mqtt_client_config_t Configuration) {
	this->mqttConfiguration = Configuration;
}

esp_mqtt_client_config_t MqttService::getConfiguration() {
	return this->mqttConfiguration;
}

void MqttService::setClient(esp_mqtt_client_handle_t &Configuration) {
	this->client = Configuration;
}

esp_mqtt_client_handle_t MqttService::getClient() {
	return this->client;
}

//esp_mqtt_client_handle_t client, esp_mqtt_event_id_t event, esp_event_handler_t event_handler, void* event_handler_arg
void MqttService::registerEventHandler(esp_event_handler_t eventHandler) {
	esp_mqtt_client_register_event((esp_mqtt_client_handle_t) this->getClient(),(esp_mqtt_event_id_t) ESP_EVENT_ANY_ID,(esp_event_handler_t) eventHandler,(void*) NULL);
}

//
//void MqttService::mqqt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
//	ESP_LOGD(MQTT_SERVICE, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
//	    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
//	    esp_mqtt_client_handle_t client = event->client;
//	    int msg_id;
//	    ESP_LOGI(MQTT_SERVICE, "Event ID %d", event_id);
//	    switch ((esp_mqtt_event_id_t)event_id) {
//	    case MQTT_EVENT_CONNECTED:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_CONNECTED");
//	        break;
//	    case MQTT_EVENT_DISCONNECTED:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_DISCONNECTED");
//	        break;
//
//	    case MQTT_EVENT_SUBSCRIBED:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//	        break;
//	    case MQTT_EVENT_UNSUBSCRIBED:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
//	        break;
//	    case MQTT_EVENT_PUBLISHED:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
//	        break;
//	    case MQTT_EVENT_DATA:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_DATA");
//	        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
//	        printf("DATA=%.*s\r\n", event->data_len, event->data);
//	        break;
//	    case MQTT_EVENT_ERROR:
//	        ESP_LOGI(MQTT_SERVICE, "MQTT_EVENT_ERROR");
//	        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
////	            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
////	            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
////	            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
////	            ESP_LOGI(MQTT_SERVICE, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
//
//	        }
//	        break;
//	    default:
//	        ESP_LOGI(MQTT_SERVICE, "Other event id:%d", event->event_id);
//	        break;
//	    }
//}
//

void MqttService::setInit(int init) {
	this->initialized = init;
}

int MqttService::isInit() {
	return this->initialized;
}

