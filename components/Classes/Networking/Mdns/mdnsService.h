/*
 * mdnsService.h
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */
#include "mdns.h"

#ifndef COMPONENTS_CLASSES_NETWORKING_MDNS_MDNSSERVICE_H_
#define COMPONENTS_CLASSES_NETWORKING_MDNS_MDNSSERVICE_H_

/** \brief TODO - Will provide a mdns resolve service to resolve advertising protocols on the network.
 *
 */
class mdnsService {
public:
	mdnsService();
	void resolve_mdns_host(const char * host_name);
};

#endif /* COMPONENTS_CLASSES_NETWORKING_MDNS_MDNSSERVICE_H_ */
