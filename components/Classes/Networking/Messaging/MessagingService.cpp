/*
 * NetworkService.cpp
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */

#include "MessagingService.h"
#define CONFIG_BROKER_LOCAL "mqtt://192.168.1.73:1883"
std::vector<message> MessagingService::messageList = std::vector<message>(); // Static messaging list

/**
 * MessagingService.cpp
 *
 * 	Provides an abstraction layer for networking and messaging
 */
MessagingService::MessagingService() {
	// TODO Auto-generated constructor stub

}

void MessagingService::initMessagingServices() {
	// Configure Network

	// Configure MQTT
	//MessagingService myMessagingService = MessagingService();
	esp_mqtt_client_config_t mqtt_cfg = {
	    	.uri = CONFIG_BROKER_LOCAL,
			.port = 1883,
	};

	MqttService myService = MqttService(mqtt_cfg);
	this->setMqttService(myService); // pass by copy because myService will be destroyed x
	this->getMqttService().InitMqttService();
	this->registerMessageHandler(this->messageReceivedEventHandler, MQTT_EVENT_DATA); // Register Event for message received
	this->getMqttService().StartMqttService(1);


}

void MessagingService::addMessageSubscription(std::string topic, int qaulity_of_service) {
	this->getMqttService().addSubscriber(topic, qaulity_of_service);
}

void MessagingService::setMessagingList(std::vector<message> messageList) {
	this->clearMessageList();
	this->getMessageList() = messageList;
}

void MessagingService::clearMessageList() {
	this->getMessageList().clear();
}

/*
 * Returns the list item from the message list
 */
message MessagingService::popMessage() {
	if (this->getMessageList().empty()) {
		return message {"","",0};
	} else {
		message recvdMessage = this->getMessageList().back();
		this->getMessageList().pop_back(); // removes the last item in the message list
		return recvdMessage;
	}
}

/*
 * Reads a message from the back of the queue (FIFO)
 */
message MessagingService::readMessage() {
	int size = this->getMessageList().size();
	if (size == 0) {
		return message {"","",0};
	} else {
		message recvdMessage = this->getMessageList().back();
		return recvdMessage;
	}

}

void MessagingService::sendMessage(std::string topic, std::string message, int qaulity_of_service) {
	this->getMqttService().publishMessage(topic, message, qaulity_of_service);
}

void MessagingService::addMessage(message messageToAdd) {
	MessagingService::messageList.insert(MessagingService::messageList.end(), messageToAdd);
}

std::vector<message> MessagingService::getMessageList() {
		return MessagingService::messageList;
}

MqttService MessagingService::getMqttService() {
	return this->myMqttService;
}

void MessagingService::setMqttService(MqttService myMqttService) {
	this->myMqttService = myMqttService;
}

void MessagingService::registerMessageHandler(esp_event_handler_t eventHandler, esp_mqtt_event_id_t event_id) { // Only do this if mqtt service is init
	if (this->getMqttService().isInit()) {
		esp_mqtt_client_register_event((esp_mqtt_client_handle_t) this->getMqttService().getClient(),(esp_mqtt_event_id_t) event_id,(esp_event_handler_t) eventHandler,(void*) NULL);
	} else {
		ESP_LOGW(MESSAGING_SERVICE, "MQTT SERVICE EVENT SERVICE FAILED - MQTT SERVICE NOT INIT");
	}
}

/*
 * Custom event handler for MessagingService Class
 */
void MessagingService::messageReceivedEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) { // Message Recieved Event Handler
	esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
	printf("Hello from message received event handler");

	printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
	printf("DATA=%.*s\r\n", event->data_len, event->data);

	char dataBuf[event->data_len];
	char topicBuf[event->topic_len];

	sprintf(dataBuf, "%.*s\r\n", event->data_len, event->data);
	sprintf(topicBuf, "%.*s\r\n", event->topic_len, event->topic);


	 std::string topic = std::string(topicBuf);
	 std::string data = std::string(dataBuf);

	 printf(dataBuf);
	 printf(topicBuf);


	 message messageToSend = {data, topic, 1}; // CPP struct
	 addMessage(messageToSend); // Passes a copy of the data x

}




