/*
 * http_server.h
 *
 *  Created on: 27-Apr-2024
 *      Author: hans
 */

#ifndef MAIN_HTTP_SERVER_H_
#define MAIN_HTTP_SERVER_H_


#define ESP_EVENT_ANY_BASE     NULL             /**< register handler for any event base */
#define ESP_EVENT_ANY_ID       -1               /**< register handler for any event id */

// WiFi application settings
#define WIFI_AP_SSID				"ESP32_AP"			// AP name
#define WIFI_AP_PASSWORD			"password"			// AP password
#define WIFI_AP_CHANNEL				1					// AP channel
#define WIFI_AP_SSID_HIDDEN			0					// AP visibility
#define WIFI_AP_MAX_CONNECTIONS		5					// AP max clients
#define WIFI_AP_BEACON_INTERVAL		100					// AP beacon: 100 milliseconds recommended
#define WIFI_AP_IP					"192.168.0.1"		// AP default IP
#define WIFI_AP_GATEWAY				"192.168.0.1"		// AP default Gateway (should be the same as the IP)
#define WIFI_AP_NETMASK				"255.255.255.0"		// AP netmask
#define WIFI_AP_BANDWIDTH			WIFI_BW_HT20		// AP bandwidth 20 MHz (40 MHz is the other option)
#define WIFI_STA_POWER_SAVE			WIFI_PS_NONE		// Power save not used
#define MAX_SSID_LENGTH				32					// IEEE standard maximum
#define MAX_PASSWORD_LENGTH			64					// IEEE standard maximum
#define MAX_CONNECTION_RETRIES		5					// Retry number on disconnect


/**
 * Messages for the HTTP monitor
 */
typedef enum http_server_message
{
	HTTP_MSG_WIFI_CONNECT_INIT = 0,
	HTTP_MSG_WIFI_CONNECT_SUCCESS,
	HTTP_MSG_WIFI_CONNECT_FAIL,
	HTTP_MSG_WIFI_USER_DISCONNECT,
	HTTP_MSG_OTA_UPDATE_SUCCESSFUL,
	HTTP_MSG_OTA_UPDATE_FAILED,
	HTTP_MSG_TIME_SERVICE_INITIALIZED,
} http_server_message_e;

/**
 * Structure for the message queue
 */
typedef struct http_server_queue_message
{
	http_server_message_e msgID;
} http_server_queue_message_t;

/**
 * Sends a message to the queue
 * @param msgID message ID from the http_server_message_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE.
 * @note Expand the parameter list based on your requirements e.g. how you've expanded the http_server_queue_message_t.
 */
BaseType_t http_server_monitor_send_message(http_server_message_e msgID);

/**
 * Starts the HTTP server.
 */
void http_server_start(void);

/**
 * Stops the HTTP server.
 */
void http_server_stop(void);

/**
 * Timer callback function which calls esp_restart upon successful firmware update.
 */
//void http_server_fw_update_reset_callback(void *arg);













#endif /* MAIN_HTTP_SERVER_H_ */
