/*
 * displayProcessor.c
 *
 *  Created on: 10/03/2021
 *      Author: lance
 */
#include "displayProcessor.h"
#include "driver/spi_master.h"
#include "driver/spi_common.h"
#include "spi_bus.h"
#include "hal/spi_types.h"

// Make sure to define requires in Cmake
#include "lvgl.h"
#include "touch_panel.h"
#include "lvgl_gui.h"

// spi bus object
static spi_bus_handle_t s_spi2_bus_handle = NULL;


void startDisplayProcessor() {
	configDisplay();
}

void configDisplay() {

	// Configure SPI

	spi_config_t busConfig = {
		.mosi_io_num = DISPLAY_MOSI_PIN,
		.miso_io_num = DISPLAY_MISO_PIN,
		.sclk_io_num = DISPLAY_SCK_PIN,
		.max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
	};

	s_spi2_bus_handle = spi_bus_create(SPI2_HOST, &busConfig);

	// Error check
	if (s_spi2_bus_handle == NULL) {
		ESP_LOGE(TAG_DP, "spi bus create error");
	}

	//	Display Config
    scr_driver_t lcd_drv;
	scr_interface_spi_config_t spi_lcd_cfg = {
		.spi_bus = s_spi2_bus_handle,
		.pin_num_cs = DISPLAY_CS_PIN,
		.pin_num_dc = DISPLAY_DC_PIN,
		.clk_freq = DISPLAY_CLK_FREQ,
		.swap_data = true,
	};

	scr_interface_driver_t *iface_drv;
	scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv);


	scr_controller_config_t lcd_cfg = {
		.interface_drv = iface_drv,
		.pin_num_rst = DISPLAY_RESET_PIN,
		.pin_num_bckl = DISPLAY_BACKLIGHT_PIN,
		.rst_active_level = 0,
		.bckl_active_level = 1,
		.offset_hor = 0,
		.offset_ver = 0,
		.width = 240,
		.height = 320,
		.rotate = SCR_DIR_TBLR,
	};
	scr_find_driver(SCREEN_CONTROLLER_ILI9341, &lcd_drv);
	lcd_drv.init(&lcd_cfg);


	//	Touch Panel Driver Configuration
    touch_panel_driver_t touch_drv;

    touch_panel_config_t touch_cfg = {

    		.interface_spi = {
			.spi_bus = s_spi2_bus_handle,
			.pin_num_cs = TOUCH_PANEL_CS,
			.clk_freq = TOUCH_PANEL_SPI_FREQ,

		},

		.interface_type = TOUCH_PANEL_IFACE_SPI,
		.pin_num_int = TOUCH_PANEL_INTERUPT,
		.direction = TOUCH_DIR_TBLR,
		.width = 240,
		.height = 320,

	};

	ESP_LOGE(TAG_DP, "WE HERE 1");

	touch_panel_find_driver(TOUCH_PANEL_CONTROLLER_XPT2046, &touch_drv);
	touch_drv.init(&touch_cfg);
	touch_drv.calibration_run(&lcd_drv, false);

	ESP_LOGE(TAG_DP, "WE HERE 1");

	/* Initialize LittlevGL GUI */
	lvgl_init(&lcd_drv, &touch_drv);

    ESP_LOGI(TAG_DP, "[APP] IDF version: %s", esp_get_idf_version());
    ESP_LOGI(TAG_DP, "[APP] Free memory: %d bytes", esp_get_free_heap_size());

}

// This Task should create display functions etc etc on the go
void displayTask(void *arg) {

	while(1) {
	    	lv_task_handler();
	    	vTaskDelay(50/portTICK_RATE_MS);
	}
}

