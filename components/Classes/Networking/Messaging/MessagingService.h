/*
 * NetworkService.h
 *
 *  Created on: 31/03/2021
 *      Author: lance
 */
#include "../Mqtt/MqttService.h"

#ifndef COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_
#define COMPONENTS_CLASSES_NETWORKING_MESSAGING_MESSAGINGSERVICE_H_

class MessagingService {
private:
	MqttService mqttClient;
public:
	MessagingService();
};

#endif /* COMPONENTS_CLASSES_NETWORKING_NETWORKSERVICE_NETWORKSERVICE_H_ */
