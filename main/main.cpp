#include "main.h"




extern "C" {
	void app_main();
}


// Global Queue Handles
//xQueueHandle xQueueRfid = NULL;
//


void app_main(void)
{

	MainApplication myApp = MainApplication();
	myApp.runConfig();
	myApp.runLoop();


	printf("-----------------   ENTERING LOOP   ----------------------\n");

	while(1) {
		vTaskDelay(500/portTICK_RATE_MS);

	}

}
