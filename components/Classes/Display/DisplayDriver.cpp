/*
 * DisplayDriver.cpp
 *
 *  Created on: 12/04/2021
 *      Author: lance
 */

#include "DisplayDriver.h"

scr_driver_t DisplayDriver::g_lcd;
scr_info_t DisplayDriver::g_lcd_info;

touch_panel_driver_t DisplayDriver::touch_drv;

DisplayDriver::DisplayDriver() {}

DisplayDriver::DisplayDriver(int lcd_spi_cs, int lcd_spi_dc, int lcd_spi_freq, int lcd_spi_reset, int lcd_spi_bklight, int miso, int mosi, int sck) {

	// Set Private Variables
	this->lcd_spi_cs = (gpio_num_t) lcd_spi_cs;
	this->lcd_spi_dc = (gpio_num_t) lcd_spi_dc;
	this->lcd_spi_freq = (gpio_num_t) lcd_spi_freq;
	this->lcd_spi_reset = (gpio_num_t) lcd_spi_reset;
	this->lcd_spi_bklight = (gpio_num_t) lcd_spi_bklight;
	this->miso = (gpio_num_t) miso;
	this->mosi = (gpio_num_t) mosi;
	this->sck = (gpio_num_t) sck;

	this->previousStateTime = esp_timer_get_time();
	this->state = 0;
}

void DisplayDriver::clear_screen(scr_driver_t *lcd, int color)
{
    scr_info_t lcd_info;
    lcd->get_info(&lcd_info);
    uint16_t *buffer = (uint16_t*) malloc(lcd_info.width * sizeof(uint16_t));
    if (NULL == buffer) {
        for (size_t y = 0; y < lcd_info.height; y++) {
            for (size_t x = 0; x < lcd_info.width; x++) {
                lcd->draw_pixel(x, y, color);
            }
        }
    } else {
        for (size_t i = 0; i < lcd_info.width; i++) {
            buffer[i] = color;
        }

        for (int y = 0; y < lcd_info.height; y++) {
            lcd->draw_bitmap(0, y, lcd_info.width, 1, buffer);
        }

        free(buffer);
    }
}

void DisplayDriver::init() {

	spi_config_t bus_conf = {
	    .miso_io_num = (gpio_num_t) this->miso,
	    .mosi_io_num = (gpio_num_t) this->mosi,
	    .sclk_io_num = (gpio_num_t) this->sck,
		.max_transfer_sz = 131072,
	}; // spi_bus configurations

	spi_bus_device_handle_t spi_bus = spi_bus_create(SPI3_HOST, &bus_conf);

    scr_interface_spi_config_t spi_lcd_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = this->lcd_spi_cs,
        .pin_num_dc = this->lcd_spi_dc,
        .clk_freq = this->lcd_spi_freq,
        .swap_data = true,
    };

    scr_interface_driver_t *iface_drv;
	scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv);
	int ret = scr_find_driver(SCREEN_CONTROLLER_ILI9341, &g_lcd);
	if (ESP_OK != ret) {
	   return;
	   ESP_LOGE(TAG_DISP, "screen find failed");
	}

	scr_controller_config_t lcd_cfg = {
		.interface_drv = iface_drv,
		.pin_num_rst = this->lcd_spi_reset,
		.pin_num_bckl = this->lcd_spi_bklight,
		.rst_active_level = 0,
		.bckl_active_level = 1,
		.width = 240,
		.height = 320,
		.offset_hor = 0,
		.offset_ver = 0,
		.rotate = SCR_DIR_TBLR,

	};
	ret = g_lcd.init(&lcd_cfg);

    if (ESP_OK != ret) {
        return;
        ESP_LOGE(TAG_DISP, "screen initialize failed");
    }

    g_lcd.get_info(&g_lcd_info);
    ESP_LOGI(TAG_DISP, "Screen name:%s | width:%d | height:%d", g_lcd_info.name, g_lcd_info.width, g_lcd_info.height);

    touch_panel_config_t touch_cfg = {

    		.interface_spi = {
			.spi_bus = spi_bus,
			.pin_num_cs = 4,
			.clk_freq = 10000000,
         },

         .interface_type = TOUCH_PANEL_IFACE_SPI,
         .pin_num_int = -1,
         .direction = TOUCH_DIR_TBLR,
         .width = 320,
         .height = 240,

    };

    touch_panel_find_driver(TOUCH_PANEL_CONTROLLER_XPT2046, &touch_drv);
    this->touch_drv.init(&touch_cfg);
    this->touch_drv.calibration_run(&this->g_lcd, false);

    vTaskDelay(10/portTICK_RATE_MS);
    lvgl_init(&this->g_lcd, &this->touch_drv);


    this->showPage(1);

}

void DisplayDriver::runDisplay() {
	this->checkState();
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void DisplayDriver::showPage(int screenNum) {
	switch(screenNum) {
		case 0:
			lv_ex_get_started_1();
			break;
		case 1:
			lv_welcome_page("Scan\nBelow\n" LV_SYMBOL_DOWN);
			break;
	}
}

void DisplayDriver::updateHomeText(char *displayTitle, int state,int displayTime) {
	updateHomeScreenText(displayTitle);
	this->setState(state);
	this->displayTime = displayTime;
}

void DisplayDriver::checkState() {
	if (this->state != STATE_HOME) {
		int64_t currentTime = esp_timer_get_time();
		if ((currentTime - previousStateTime)/US_TO_S > this->displayTime) {
			//Set Display as home screen
			this->updateHomeText("Scan\nBelow\n" LV_SYMBOL_DOWN, STATE_HOME, 0);
		}
	}
}

void DisplayDriver::setState(int state) {
	this->state = state;
	this->previousStateTime = esp_timer_get_time();
}
int DisplayDriver::getState() {
	return this->state;
}
