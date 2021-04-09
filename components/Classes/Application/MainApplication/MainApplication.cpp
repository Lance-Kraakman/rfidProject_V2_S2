/*
 * MainApplication.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "MainApplication.h"

#define MAX_SECONDS 30

MainApplication::MainApplication() {
	// TODO Auto-generated constructor stub

}

void MainApplication::runConfig() {

	this->DataProcessor = DataProcessing();
	this->TagProcessor = TagProcessing(&this->DataProcessor);

	this->DataProcessor.init();
	this->TagProcessor.init();

}

// Basic State Machine
void MainApplication::runLoop() {

	while(1) {
		// Theese have to be sequential and not in parrellel because they access shared data
		this->TagProcessor.doProcessing();
		this->DataProcessor.doMessageProcessing();
		vTaskDelay(200/portTICK_RATE_MS);
	}
}




