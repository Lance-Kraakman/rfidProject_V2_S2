/*
 * Time.cpp
 *
 *  Created on: 3/04/2021
 *      Author: lance
 */

#include "SntpTime.h"

SntpTime::SntpTime() {
	// TODO Auto-generated constructor stub
	this->year = 0;
	this->month = 0;
	this->day = 0;
	this->hour = 0;
	this->minute = 0;
	this->second = 0;
	this->timeInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	this->timeNow = {0};
}

// Static because only one per class
void SntpTime::config() {

	if (sntp_enabled()) { // SNTP must be inited inside LWIP driver code
	sntp_stop();
	}
	ESP_LOGI(TIME_TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "pool.ntp.org");
	sntp_set_time_sync_notification_cb(this->time_sync_notification_cb);
	sntp_init();
}


void SntpTime::time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TIME_TAG, "Notification of a time synchronization event");
}

/*
 * Time sync function Call once, every couple of days-ish
 */
void SntpTime::SyncTime() {

	int retry = 0;
	const int retry_count = 10;

	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
	        ESP_LOGI(TIME_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
	        vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

	time(&(this->timeNow));
	localtime_r(&(this->timeNow), &(this->timeInfo));

	this->setTimeZoneNZ(TIMEZONE_STR_NZ);
}


void SntpTime::setTimeZoneNZ(const char * timezoneInfo) {
	setenv("TZ", timezoneInfo, 1);
	tzset();
}

void SntpTime::printTime() {
    char strftime_buf[64];
	localtime_r(&(this->timeNow), &(this->timeInfo));
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &(this->timeInfo));
	ESP_LOGI(TIME_TAG, "The current date/time in New Zealand is: %s", strftime_buf);
	ESP_LOGI(TIME_TAG, "year:hour:minute:second %d, %d, %d, %d", this->year, this->hour, this->minute, this->second);
}

/*
 * Function Updates its time variables <3
 */
void SntpTime::updateToCurrentTime() {
	localtime_r(&(this->timeNow), &(this->timeInfo));
	this->day = this->timeInfo.tm_mday;
	this->year = this->timeInfo.tm_year;
	this->month = this->timeInfo.tm_mon;
	this->minute = this->timeInfo.tm_min;
	this->hour = this->timeInfo.tm_hour;
	this->second = this->timeInfo.tm_sec;

}

