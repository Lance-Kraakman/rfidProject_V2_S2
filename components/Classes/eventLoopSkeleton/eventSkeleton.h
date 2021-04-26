/*
 * eventLoopSkeleton.h
 *
 *  Created on: 22/04/2021
 *      Author: lance
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_base.h"

#include "esp_event.h"
#include "esp_timer.h"

#define CONFIG_EVENT_LOOP_SKELETON_SIZE 5

/* Event source task related definitions */

#ifndef COMPONENTS_CLASSES_EVENTSKELETON_EVENTSKELETON_H_
#define COMPONENTS_CLASSES_EVENTSKELETON_EVENTSKELETON_H_

/** \brief Class Runs a event loop. Classes which inherit
 * this class can add functions to the event loop.
 *
 *	To run the event loop, runEventLoop must be called
 */
class eventSkeleton {
public:
	eventSkeleton();
	void addHandler(int event_id, esp_event_handler_t event_handler, void *event_handler_args, esp_event_handler_instance_t *instance);
	static void eventPostTo(int event_id, void * event_data, size_t event_data_size);
	static void runEventLoop(int ticksToRun);
private:
	static int instances;
	static esp_event_base_t EVENT_BASE_ONE;
	static esp_event_loop_args_t loop_without_task_args;
	static esp_event_loop_handle_t loop_handle;


};

#endif /* COMPONENTS_CLASSES_EVENTSKELETON_EVENTLOOPSKELETON_H_ */
