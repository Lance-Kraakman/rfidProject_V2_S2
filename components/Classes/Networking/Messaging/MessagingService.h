/*
 * NetworkService.h
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */
#include "../MqttService/MqttService.h"
#include <vector>

#ifndef COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_
#define COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_

#define MESSAGING_SERVICE "MESSAGING SERVICE"

typedef struct {
	std::string data;
	std::string topic;
	int recvd;
} message;

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

	static void addMessage(message messageToAdd); // Adds a message to the list of messages

	std::vector<message> getMessageList();

	MqttService getMqttService();
	void setMqttService(MqttService mqttService);
	void registerMessageHandler(esp_event_handler_t eventHandler, esp_mqtt_event_id_t event_id);
	static void messageReceivedEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

};

/*
 *	Example Useage
 *
	MessagingService myMessagingServcie = MessagingService();
	myMessagingServcie.initMessagingServices();
	myMessagingServcie.addMessageSubscription("tags", qaulity_of_service)
	myMessagingServcie.addMessageSubscription("Commands", qaulity_of_service);
*/




#endif /* COMPONENTS_CLASSES_NETWORKING_NETWORKSERVICE_NETWORKSERVICE_H_ */
