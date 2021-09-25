#include "main.h"



extern "C" {
	void app_main();
}


// Global Queue Handles
//xQueueHandle xQueueRfid = NULL;
//


void app_main(void)
{

	vTaskDelay(500);

/* To Get 80,000000 Hz SPI
https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/spi_master.html
 *
 */

	MainApplication myApp = MainApplication();
	myApp.runConfig();
	myApp.runLoop();

	printf("-----------------   ENTERING LOOP   ----------------------\n");
	while(1) {}

}
