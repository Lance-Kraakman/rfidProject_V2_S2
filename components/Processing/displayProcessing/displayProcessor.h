/*
 * displayProcessor.h
 *
 *  Created on: 10/03/2021
 *      Author: lance
 */

#ifndef COMPONENTS_PROCESSING_DISPLAYPROCESSING_DISPLAYPROCESSOR_H_
#define COMPONENTS_PROCESSING_DISPLAYPROCESSING_DISPLAYPROCESSOR_H_

// Pure SPI Constants
#define SPI_MAX_TRANSFER_SIZE 584000
#define DISPLAY_MOSI_PIN 21
#define DISPLAY_SCK_PIN 22
#define DISPLAY_MISO_PIN 27

// Display SPI Constants
#define DISPLAY_SPI_FREQ 20000000
#define DISPLAY_RESET_PIN 18
#define DISPLAY_BACKLIGHT_PIN 23
#define DISPLAY_CS_PIN 0
#define DISPLAY_DC_PIN 2
#define DISPLAY_CLK_FREQ 1000000

// Touch Panel Constants
#define TOUCH_PANEL_INTERUPT 33
#define TOUCH_PANEL_SPI_FREQ 100000
#define TOUCH_PANEL_CS 5

#define TAG_DP "DISPLAY PROCESSOR"

void startDisplayProcessor();
void configDisplay();
void displayTask(void *arg);


#endif /* COMPONENTS_PROCESSING_DISPLAYPROCESSING_DISPLAYPROCESSOR_H_ */
