/*
 * eventLoopSkeleton.cpp
 *
 *  Created on: 22/04/2021
 *      Author: lance
 */

#include "eventSkeleton.h"

esp_event_loop_handle_t eventSkeleton::loop_handle = NULL;

int eventSkeleton::instances = 0;
esp_event_base_t eventSkeleton::EVENT_BASE_ONE = "EVENT_BASE_ONE";

esp_event_loop_args_t eventSkeleton::loop_without_task_args = {
		.queue_size = CONFIG_EVENT_LOOP_SKELETON_SIZE,
		.task_name = NULL, // task will not be created
	};

/**\brief Default Constructor
 *
 */
eventSkeleton::eventSkeleton() {


	// Only run once
	if (eventSkeleton::instances == 0) {
		ESP_LOGI("", "starting user loop");
		ESP_ERROR_CHECK(esp_event_loop_create(&eventSkeleton::loop_without_task_args, &eventSkeleton::loop_handle));

		instances++;
	}
}

/** \brief add a event handler to the event loop
 *
 * @param event_id
 * @param event_handler
 * @param event_handler_args
 * @param instance
 */
void eventSkeleton::addHandler(int event_id, esp_event_handler_t event_handler, void *event_handler_args, esp_event_handler_instance_t *instance) {
	//esp_event_handler_instance_register_with(event_loop, event_base, event_id, event_handler, event_handler_arg, instance)
	esp_event_handler_instance_register_with(eventSkeleton::loop_handle, eventSkeleton::EVENT_BASE_ONE, event_id, event_handler, &eventSkeleton::loop_without_task_args, instance);
}

/** \brief post to a created event
 *
 * @param event_id
 * @param event_data
 * @param event_data_size
 */
void eventSkeleton::eventPostTo(int event_id, void * event_data, size_t event_data_size) {
	esp_event_post_to(eventSkeleton::loop_handle,eventSkeleton::EVENT_BASE_ONE, event_id, event_data, event_data_size, portMAX_DELAY);
	//esp_event_post(TASK_EVENTS, TASK_ITERATION_EVENT, &iteration, sizeof(iteration), portMAX_DELAY);
}

/** \brief Runs the event loop
 *
 * @param ticksToRun
 */
void eventSkeleton::runEventLoop(int ticksToRun) {
	esp_event_loop_run(eventSkeleton::loop_handle, ticksToRun);
}



