/*
 * DisplayDriver.h
 *
 *  Created on: 12/04/2021
 *      Author: lance
 */
extern "C" {
	#include "board.h"
	#include <stdio.h>
	#include <string.h>
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
	#include "freertos/queue.h"
	#include "esp_log.h"
	#include "esp_task_wdt.h"
	#include "board.h"
	#include "screen_driver.h"
	#include "lvgl_gui.h"
	#include "lvgl.h"
	#include "../../CustomLvgl/Pages/HomePage/HomePage.h"
}
#define TAG_DISP "DISPLAY DRIVER"

#ifndef COMPONENTS_CLASSES_DISPLAY_DISPLAYDRIVER_H_
#define COMPONENTS_CLASSES_DISPLAY_DISPLAYDRIVER_H_

#define LV_TICK_PERIOD_MS 10

#define US_TO_S 1000000
#define STATE_HOME 0
#define STATE_DEVICE_TAGGED 2
#define STATE_EMPLOYEE_TAGGED 3
#define STATE_NOT_HOME 1

class DisplayDriver {
private:
	static scr_driver_t g_lcd;
	static scr_info_t g_lcd_info;
	static touch_panel_driver_t touch_drv;
	gpio_num_t lcd_spi_cs;
	gpio_num_t lcd_spi_dc;
	gpio_num_t lcd_spi_freq;
	gpio_num_t lcd_spi_reset;
	gpio_num_t lcd_spi_bklight;

	gpio_num_t miso;
	gpio_num_t mosi;
	gpio_num_t sck;

	int displayTime;
	int state;
	int64_t previousStateTime;
public:
	DisplayDriver();
	DisplayDriver(int lcd_spi_cs, int lcd_spi_dc, int lcd_spi_freq, int lcd_spi_reset, int lcd_spi_bklight, int miso, int mosi, int sck);
	void init();
	void runDisplay();
	void clear_screen(scr_driver_t *lcd, int color);
	void showPage(int screenNum);
	void updateHomeText(char *displayTitle, int state, int displayTime);
	void checkState();
	void setState(int state);
	int getState();

};

#endif /* COMPONENTS_CLASSES_DISPLAY_DISPLAYDRIVER_H_ */
