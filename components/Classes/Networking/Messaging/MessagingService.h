/*
 * NetworkService.h
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */
#include "../MqttService/MqttService.h"
#include <vector>
#include <string>

#ifndef COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_
#define COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_

#define MESSAGING_SERVICE "MESSAGING SERVICE"
#define DEFAULT_QOS 1

/** \brief struct representing a MQTT message.
 *
 */
typedef struct {
	std::string data;
	std::string topic;
	int recvd;
} message;

/** \brief Provides an abstraction layer for networking and messaging communications.
 * 		Connection is made to a MQTT service. Messages that come in are stored in a list and can be accessed at any time.
 *		Static functionality as it is only reasonable to connect to one MQTT service at a time
 *
 *
 */
class MessagingService {
private:
	MqttService myMqttService;
public:
	// Public Attributes
	static std::vector<message> messageList;

	// Public Methods
	MessagingService();
	void initMessagingServices();
	void addMessageSubscription(std::string topic, int qaulity_of_service); // Subscribe to topic
	void setMessagingList(std::vector<message> messageList);
	void clearMessageList();
	message popMessage();
	message readMessage();
	void sendMessage(std::string topic, std::string message, int qaulity_of_service);
	void updateMessageList();
	void printMessage(message mes);

	static void addMessage(message messageToAdd); // Adds a message to the list of messages

	std::vector<message> getMessageList();

	MqttService getMqttService();
	void setMqttService(MqttService mqttService);
	void registerMessageHandler(esp_event_handler_t eventHandler, esp_mqtt_event_id_t event_id);
	static void messageReceivedEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

};






#endif /* COMPONENTS_CLASSES_NETWORKING_NETWORKSERVICE_NETWORKSERVICE_H_ */
