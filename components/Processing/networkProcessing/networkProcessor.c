/*
 * networkProcessor.c
 *
 *  Created on: 10/03/2021
 *      Author: lance
 */
#include "networkProcessor.h"
#include "protocol_examples_common.h"
#include "mqtt_client.h"
#define MIN_TAG_TIME_MICROSECONDS 1000000
static char *rfidDataBuffer = NULL;

//	Connect to local network and run a HTTPS server
void processServerData(void *arg);
void sendDataMqtt(void *arg);
void resolve_mdns_host(const char * host_name);
static xSemaphoreHandle xSemDataSent = NULL;
static esp_mqtt_client_handle_t client = NULL;
static int desktop_app_listening = 0;


void startNetworkProcessor() {
	printf("Starting Network Processor \n");
	xSemDataSent = xSemaphoreCreateBinary();
	configNetwork();
	xTaskCreate(sendDataMqtt, "sendDataMqtt", 4096, NULL, 1, NULL);
	xTaskCreate(processServerData, "processServerData", 4096 , NULL, 1, NULL);

}

/* Mqtt Client */
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG_NP, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG_NP, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    ESP_LOGI(TAG_NP, "Event ID %d", event_id);
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        desktop_app_listening = (int) *event->data; // Will just be a single bit tehe x
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG_NP, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG_NP, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG_NP, "Other event id:%d", event->event_id);
        break;
    }
}

/* Mqtt App Config */
static void mqtt_app_start(void)
{
    static esp_mqtt_client_config_t mqtt_cfg = {
    	.uri = CONFIG_BROKER_LOCAL,
		.port = 1883,

    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

// Config
void configNetwork() {

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	ESP_ERROR_CHECK(example_connect());


	//start_mdns_service();
	mqtt_app_start();

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

	rfid_tag_item prev_recvd_rfidTag;
	prev_recvd_rfidTag.time = 0;

	while(1) {


		queueRecieved = (int) xQueueReceive(xQueueRfid, &recvd_rfidTag,( TickType_t ) portMAX_DELAY);

		// If an Http response has occurred, Create New JSON
		if (xSemaphoreTake(xSemDataSent, 0)) {
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

				// Update the response
				rfidDataBuffer = cJSON_Print(finalJSON); // This allocates a new block of memory, Containg the string

				// Print information
				ESP_LOGI(TAG_NP,"\n%s", rfidDataBuffer);
				size_t strLen = strlen(rfidDataBuffer);
				ESP_LOGI(TAG_NP, "Length of string %d \n",strLen);



			}
			memcpy(&prev_recvd_rfidTag, &recvd_rfidTag, sizeof(rfid_tag_item));
		}
	}
}

void sendDataMqtt(void *arg) {
	//Subscribe to topics

    int msg_id = esp_mqtt_client_subscribe(client, "desktop-application", 1);
    ESP_LOGI(TAG_NP, "sent subscribe successful, msg_id=%d", msg_id);


	while(1) {

		// Check if the mqtt subscriber is listening and there is data in the buffer
		if ((rfidDataBuffer != NULL) && (desktop_app_listening)) {

			ESP_LOGI(TAG_NP, "We made it baby");

			msg_id = esp_mqtt_client_publish(client, "rfid", rfidDataBuffer, 0, 0, 0);
			ESP_LOGI(TAG_NP, "sent publish successful, msg_id=%d", msg_id);

			// hell

			xSemaphoreGive(xSemDataSent); // Let the data processing service know that we have sent the data
			free(rfidDataBuffer); // Free allocated memory
			rfidDataBuffer = NULL;

		}

		vTaskDelay(500/portTICK_RATE_MS);
	}
}



///* Http get handler which gets all of the tagged events from the RFID event tags */
//static esp_err_t get_taged_handler(httpd_req_t *req) {
//
//	BaseType_t xHigherPriorityTaskWoken;
//	httpd_resp_set_type(req, "text/json");
//
//	if (rfidDataBuffer != NULL) {
//
//		httpd_resp_send(req, rfidDataBuffer, HTTPD_RESP_USE_STRLEN);
//		xSemaphoreGiveFromISR(xSemHttp, &xHigherPriorityTaskWoken);
//		// Have to do this here, as we need it to be called before the next http call
//		free(rfidDataBuffer); // Free allocated memory
//		rfidDataBuffer = NULL; // Set equal to null.
//		if (xHigherPriorityTaskWoken!=pdFALSE) {
//		// Force Context Switch
//			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		}
//
//	} else {
//		httpd_resp_send(req, "Get F%$^3d. Empty List", HTTPD_RESP_USE_STRLEN);
//	}
//
//
//    return ESP_OK;
//}









