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


//	RfidScanner MyScanner = RfidScanner();
//	MyScanner.startRfidScanner();


	// Cry
//	configNetwork();
//
//	SntpTime globalTime = SntpTime();
//
//	globalTime.config();
//	globalTime.SyncTime();
//	globalTime.printTime();
//	globalTime.updateToCurrentTime(); // sets the time to the current time
//	globalTime.printTime();
//


	// Employee/Tag classes have lists etc but dont "Have" a messaging service
	//Tag
//	ESP_LOGI("---- ", "1");
//
//	RfidTag empTag = RfidTag("123456789");
////
////	printf("OK");
////
////	// add subscriptions
//	Employee lanceKraakman = Employee();
//	lanceKraakman.setName("Lance Kraakman");
//	lanceKraakman.setTag(empTag);
//
//	ESP_LOGI("---- ", "2");
//
//
//
//
//
//	Device device1 = Device(RfidTag("1234"));
//	Device device2 = Device(RfidTag("223425"));
//	Device device3 = Device(RfidTag("3234"));
//	Device device4 = Device(RfidTag("42341"));
//
//	Device device5 = Device(RfidTag("23425"));
//	Device device6 = Device(RfidTag("646576"));
//	Device device7 = Device(RfidTag("234657"));
//	Device device8 = Device(RfidTag("7438"));
//
//	Device device9 = Device(RfidTag("900"));
//	device9.setName("Akinifenwa");
//
//
//	ESP_LOGI("","%s", device3.getTag().getUUID().c_str());
//	ESP_LOGI("---- ", "3");
//
////
//	lanceKraakman.addDevice(device1);
//	lanceKraakman.addDevice(device2);
//	lanceKraakman.addDevice(device3);
//	lanceKraakman.addDevice(device4);
//	lanceKraakman.addDevice(device5);
//	lanceKraakman.addDevice(device6);
//	lanceKraakman.addDevice(device7);
//	lanceKraakman.addDevice(device8);
//	lanceKraakman.addDevice(device9);
//
//	ESP_LOGI("---- ", "4");
//
//	vTaskDelay(50/portTICK_RATE_MS);
//
//
//	Device dummy = Device(RfidTag("900"));
//	printf("Device Name is %s\n", dummy.getName().c_str());
//
//
//
////	std::vector<Device>::iterator it = lanceKraakman.findDevice(dummy);
//	vTaskDelay(500/portTICK_RATE_MS);
//
//	if(lanceKraakman.findDevice(dummy)) {
//		ESP_LOGI("---- ", "5");
//		ESP_LOGI("---- ", "6");
//
//	} else {
//		printf("Couldnt find device \n");
//	}
//
//	printf("Device Name is Now %s\n", dummy.getName().c_str());



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
