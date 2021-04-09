/*
 * Time.cpp
 *
 *  Created on: 3/04/2021
 *      Author: lance
 */

#include "SntpTime.h"

/** Struct represents time info, Static because one variable needs to be shared between all instances
 *
 *
 */
tm SntpTime::timeInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Current time struct, Static because latest update is the same between classes.
 */
time_t SntpTime::timeNow = {0};

SntpTime::SntpTime() {
	this->updateToCurrentTime();
}

/** Configures SntpTime
 *
 */
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

/** Event handler for time synchronization event
 *
 * @param tv
 */
void SntpTime::time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TIME_TAG, "Notification of a time synchronization event");
}

/**
 * Synchronizes the time with the classes time variables
 */
void SntpTime::SyncTime() {

	int retry = 0;
	const int retry_count = 10;

	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
	        ESP_LOGI(TIME_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
	        vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

	time(&(SntpTime::timeNow));
	localtime_r(&(SntpTime::timeNow), &(SntpTime::timeInfo));

	this->setTimeZoneNZ(TIMEZONE_STR_NZ);
}

/** Sets the timezone to nz
 *
 */
void SntpTime::setTimeZoneNZ(const char * timezoneInfo) {
	setenv("TZ", timezoneInfo, 1);
	tzset();
}

/** prints the time
 *
 */
void SntpTime::printTime() {
    char strftime_buf[64];
	localtime_r(&(SntpTime::timeNow), &(SntpTime::timeInfo));
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &(SntpTime::timeInfo));
	ESP_LOGI(TIME_TAG, "The current date/time in New Zealand is: %s", strftime_buf);
	ESP_LOGI(TIME_TAG, "day:hour:minute:second %d, %d, %d, %d", this->day, this->hour, this->minute, this->second);
}

/** updates classes time variables to the current time
 *
 */
void SntpTime::updateToCurrentTime() {
	time(&(SntpTime::timeNow));
	localtime_r(&(SntpTime::timeNow), &(SntpTime::timeInfo));
	this->day = this->timeInfo.tm_mday;
	this->year = this->timeInfo.tm_year;
	this->month = this->timeInfo.tm_mon;
	this->minute = this->timeInfo.tm_min;
	this->hour = this->timeInfo.tm_hour;
	this->second = this->timeInfo.tm_sec;

}

/**
 *
 * @return a std::string representation of objects time
 */
std::string SntpTime::timeString() {

	std::string returnString = "";

	returnString.append(std::to_string(this->year)); returnString.append(":");
	returnString.append(std::to_string(this->month)); returnString.append(":");
	returnString.append(std::to_string(this->day)); returnString.append(":");
	returnString.append(std::to_string(this->hour)); returnString.append(":");
	returnString.append(std::to_string(this->minute)); returnString.append(":");
	returnString.append(std::to_string(this->second));

	return returnString;
}

