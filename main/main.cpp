#include "main.h"

#include "../components/Classes/Employee/Employee.h"
#define CONFIG_BROKER_LOCAL "mqtt://192.168.1.73:1883"

extern "C" {
	void app_main();
}


// Global Queue Handles
//xQueueHandle xQueueRfid = NULL;
//
void configNetwork() {

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(example_connect());

}

void app_main(void)
{
	// Cry
	//configNetwork();

	// Employee/Tag classes have lists etc but dont "Have" a messaging service
	//Tag
	ESP_LOGI("---- ", "1");

	RfidTag empTag = RfidTag("123456789");
//
//	printf("OK");
//
//	// add subscriptions
	Employee lanceKraakman = Employee();
	lanceKraakman.setName("Lance Kraakman");
	lanceKraakman.setTag(empTag);

	ESP_LOGI("---- ", "2");




	Device device1 = Device(RfidTag("1234"));
	Device device2 = Device(RfidTag("223425"));
	Device device3 = Device(RfidTag("3234"));
	Device device4 = Device(RfidTag("42341"));

	Device device5 = Device(RfidTag("23425"));
	Device device6 = Device(RfidTag("646576"));
	Device device7 = Device(RfidTag("234657"));
	Device device8 = Device(RfidTag("7438"));
	Device device9 = Device(RfidTag("900"));

	ESP_LOGI("","%s", device3.getTag().getUUID().c_str());
	ESP_LOGI("---- ", "3");

//
	lanceKraakman.addDevice(device1);
	lanceKraakman.addDevice(device2);
	lanceKraakman.addDevice(device3);
	lanceKraakman.addDevice(device4);
	lanceKraakman.addDevice(device5);
	lanceKraakman.addDevice(device6);
	lanceKraakman.addDevice(device7);
	lanceKraakman.addDevice(device8);
	lanceKraakman.addDevice(device9);

	ESP_LOGI("---- ", "4");



	Device dummy = Device(RfidTag("7438"));
	auto it = lanceKraakman.findDevice(dummy);

	if(lanceKraakman.checkDataReturned(it)) {
		Device dev = *it;
	} else {
		printf("Couldnt find device \n");
	}



//
//	//Device dev = lanceKraakman.getDeviceList().back();
//	//dev.getTag();
//
//	for (Device dev: lanceKraakman.getDeviceList()) {
//		dev.getTag();
//		ESP_LOGI("---- ", "hdsjblsakjfb");
//	}
//



//	printf("%s" ,lanceKraakman.getDeviceList().back().getTag().getUUID().c_str());

	ESP_LOGI("---- ", "5");

	printf("-----------------   ENTERING LOOP   ----------------------\n");

	while(1) {
		vTaskDelay(500/portTICK_RATE_MS);

	}

}
