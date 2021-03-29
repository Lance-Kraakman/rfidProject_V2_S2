/*
 * networkProcessor.c
 *
 *  Created on: 10/03/2021
 *      Author: lance
 */
#include "networkProcessor.h"

#define MIN_TAG_TIME_MICROSECONDS 1000000
static char *rfidHttpResponse = NULL;

//	Connect to local network and run a HTTPS server
static void checkMdnsService(void *arg);
void processServerData(void *arg);
void resolve_mdns_host(const char * host_name);
static xSemaphoreHandle xSemHttp = NULL;

void startNetworkProcessor() {
	printf("Starting Network Processor \n");
	xSemHttp = xSemaphoreCreateBinary();
	configNetwork();
	xTaskCreate(checkMdnsService, "checkMdnsService", 4096, NULL, 1, NULL);
	xTaskCreate(processServerData, "processServerData", 4096 , NULL, 1, NULL);

}

/* An HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Hello Secure World!</h1>", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler
};

/* Http get handler which gets all of the tagged events from the RFID event tags */
static esp_err_t get_taged_handler(httpd_req_t *req) {

	BaseType_t xHigherPriorityTaskWoken;
	httpd_resp_set_type(req, "text/json");

	if (rfidHttpResponse != NULL) {

		httpd_resp_send(req, rfidHttpResponse, HTTPD_RESP_USE_STRLEN);
		xSemaphoreGiveFromISR(xSemHttp, &xHigherPriorityTaskWoken);
		// Have to do this here, as we need it to be called before the next http call
		free(rfidHttpResponse); // Free allocated memory
		rfidHttpResponse = NULL; // Set equal to null.
		if (xHigherPriorityTaskWoken!=pdFALSE) {
		// Force Context Switch
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}

	} else {
		httpd_resp_send(req, "Get F%$^3d. Empty List", HTTPD_RESP_USE_STRLEN);
	}


    return ESP_OK;
}

static const httpd_uri_t tag_get_request = {
    .uri       = "/getTags",
    .method    = HTTP_GET,
    .handler   = get_taged_handler
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;


    // Start the httpd server
    ESP_LOGI(TAG_NP, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();

    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
    conf.cacert_pem = cacert_pem_start;
    conf.cacert_len = cacert_pem_end - cacert_pem_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    esp_err_t ret = httpd_ssl_start(&server, &conf);

    if (ESP_OK != ret) {
        ESP_LOGI(TAG_NP, "Error starting server!");
        return NULL;
    }

    // Set URI handlers
    ESP_LOGI(TAG_NP, "Registering URI handlers");
    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &tag_get_request);
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the server
    httpd_ssl_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        *server = start_webserver();
    }
}

static void checkMdnsService(void *arg) {
	while(1) {
		vTaskDelay(15000/portTICK_RATE_MS); // Every 45 Seconds
		refresh_mdns_service();
	}
}

void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS Init failed: %d\n", err);
        return;
    } else {
    	printf("MDNS Init Success \n");
    }

    mdns_hostname_set("my-esp32");			    //set hostname
    mdns_instance_name_set("my-esp32");     //set default instance

    //Add Service
    mdns_txt_item_t serviceTxtData[] = {
            {"board", "esp32"},
            {"path", "/"}
        };

	ESP_ERROR_CHECK(mdns_service_add(NULL, "_https", "_tcp", 443, serviceTxtData,
									 sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));


}
// Unused - MDNS service cannot
void resolve_mdns_host(const char * host_name)
{
    printf("Query A: %s.local", host_name);
    esp_ip4_addr_t addr;
    addr.addr = 0;

    esp_err_t err = mdns_query_a(host_name, 3000,  &addr);
    if(err){
    	mdns_free();
    	vTaskDelay(1000/portTICK_RATE_MS);
    	start_mdns_service();

    	if(err == ESP_ERR_NOT_FOUND){
            printf(" Host was not found!\n");
            return;
        }
        printf("Query Failed");
        return;
    }

    printf(IPSTR, IP2STR(&addr));
}

void refresh_mdns_service() {
	printf("---------MDNS REFRESH---------\n");
	mdns_free();
	vTaskDelay(1000/portTICK_RATE_MS);
	start_mdns_service();
}

// Config
void configNetwork() {
	static httpd_handle_t server = NULL;

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	start_mdns_service();

	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

	ESP_ERROR_CHECK(example_connect());
}

// returns JSON array of scanned in tag objects
// Parse a buffer & return -1 for unseccusefull
char* getTagsFromQueue() {

	cJSON *jsonArray = NULL;
	cJSON *finalJSON = cJSON_CreateObject();
	jsonArray = cJSON_AddArrayToObject(finalJSON, "RFID TAG ARRAY");

	int queueRecieved = (int) pdTRUE;
	rfid_tag_item recvd_rfidTag; //Array of RFID Tag items

	while(queueRecieved == pdTRUE) {

		memset(&recvd_rfidTag, 0, sizeof(recvd_rfidTag));
		queueRecieved = (int) xQueueReceive(xQueueRfid, &recvd_rfidTag,( TickType_t ) 0);

		if (queueRecieved == pdTRUE) { // Keep this check
			printf("RFID Tag Received: %s\n", recvd_rfidTag.uuid);
			cJSON *rfidJsonObject = cJSON_CreateObject();

			// Add cJSON string object
			//cJSON_AddStringToObject(rfidJsonObject, "UUID", recvd_rfidTag.uuid);

			// Add cJSON object to array
			cJSON_AddItemToArray(jsonArray, rfidJsonObject);
		}
	}


	char *string = cJSON_Print(finalJSON); // This Dosent allocates memory.
	ESP_LOGI(TAG_NP,"\n%s", string);

	size_t strLen = strlen(string);
	ESP_LOGI(TAG_NP, "Length of string %d \n",strLen);

	// Create memory block for buffer for http response
	free(rfidHttpResponse);
	char *rfidHttpResponse = (char*) malloc((strLen+5)*sizeof(char));
	memcpy(rfidHttpResponse,string,(strLen)*sizeof(char));

	return rfidHttpResponse;


}
//portMAX_DELAY
// Low priority Task. Add data to server from queue
void processServerData(void *arg) {

	cJSON *jsonArray = NULL;
	cJSON *finalJSON = NULL;

	finalJSON = cJSON_CreateObject();
	jsonArray = cJSON_AddArrayToObject(finalJSON, "RFID TAG ARRAY");

	int queueRecieved = (int) pdTRUE;
	rfid_tag_item recvd_rfidTag; //rfid tag item

	rfid_tag_item prev_recvd_rfidTag; // Copy of previous RFID Tag
	prev_recvd_rfidTag.time = 0;

	while(1) {

		queueRecieved = (int) xQueueReceive(xQueueRfid, &recvd_rfidTag,( TickType_t ) portMAX_DELAY);

		// If an Http response has occurred, Create New JSON
		if (xSemaphoreTake(xSemHttp, 0)) {
			cJSON_Delete(finalJSON);
			finalJSON = cJSON_CreateObject();
			jsonArray = cJSON_AddArrayToObject(finalJSON, "RFID TAG ARRAY");
		}

		// When an Object is succesfully recieved from the queue
		if (queueRecieved == pdTRUE) {

			//Print the Tag times
			ESP_LOGI(TAG_NP, "current %llu: \n", recvd_rfidTag.time);
			ESP_LOGI(TAG_NP, "previous %llu: \n", prev_recvd_rfidTag.time);
			ESP_LOGI(TAG_NP, "difference %llu: \n",recvd_rfidTag.time - prev_recvd_rfidTag.time);


			// Check Time Difference If time difference is great then do thing
			if (recvd_rfidTag.time - prev_recvd_rfidTag.time > MIN_TAG_TIME_MICROSECONDS) {

				printf("RFID Tag Received: %s\n", recvd_rfidTag.uuid);
				cJSON *rfidJsonObject = cJSON_CreateObject();
				cJSON *uuidArray = cJSON_AddArrayToObject(rfidJsonObject, "UUID");

				// Add cJSON string object
				cJSON_AddNumberToObject(uuidArray, "UUID", recvd_rfidTag.uuid[0]);
				cJSON_AddNumberToObject(uuidArray, "UUID", recvd_rfidTag.uuid[1]);
				cJSON_AddNumberToObject(uuidArray, "UUID", recvd_rfidTag.uuid[2]);
				cJSON_AddNumberToObject(uuidArray, "UUID", recvd_rfidTag.uuid[3]);
				cJSON_AddNumberToObject(uuidArray, "UUID", recvd_rfidTag.uuid[4]);


				// Add cJSON object to array
				cJSON_AddItemToArray(jsonArray, rfidJsonObject);
				ESP_LOGI(TAG_NP, "Fuck 3");

				// Update the response
				rfidHttpResponse = cJSON_Print(finalJSON); // This allocates a new block of memory, Containg the string
				ESP_LOGI(TAG_NP,"\n%s", rfidHttpResponse);
				size_t strLen = strlen(rfidHttpResponse);
				ESP_LOGI(TAG_NP, "Length of string %d \n",strLen);

			}

			memcpy(&prev_recvd_rfidTag, &recvd_rfidTag, sizeof(rfid_tag_item));

		}
	}


}

















