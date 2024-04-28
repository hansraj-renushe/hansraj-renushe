/*
 * wifi_app.h
 *
 *  Created on: 27-Apr-2024
 *      Author: hans
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_


#include "esp_netif_types.h"
#include "esp_wifi_types.h"



// WIFI application settings
//
//// WiFi application settings
//#define WIFI_AP_SSID				"ESP32_AP"			// AP name
//#define WIFI_AP_PASSWORD			"password"			// AP password
//#define WIFI_AP_CHANNEL				1					// AP channel
//#define WIFI_AP_SSID_HIDDEN			0					// AP visibility
//#define WIFI_AP_MAX_CONNECTIONS		5					// AP max clients
//#define WIFI_AP_BEACON_INTERVAL		100					// AP beacon: 100 milliseconds recommended
//#define WIFI_AP_IP					"192.168.0.1"		// AP default IP
//#define WIFI_AP_GATEWAY				"192.168.0.1"		// AP default Gateway (should be the same as the IP)
//#define WIFI_AP_NETMASK				"255.255.255.0"		// AP netmask
//#define WIFI_AP_BANDWIDTH			WIFI_BW_HT20		// AP bandwidth 20 MHz (40 MHz is the other option)
//#define WIFI_STA_POWER_SAVE			WIFI_PS_NONE		// Power save not used
//#define MAX_SSID_LENGTH				32					// IEEE standard maximum
//#define MAX_PASSWORD_LENGTH			64					// IEEE standard maximum
//#define MAX_CONNECTION_RETRIES		5					// Retry number on disconnect




//netif objects for station and access point

extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;

/*
 * message IDs for wifi application task
 */

typedef enum wifi_app_message
{
	WIFI_APP_MSG_START_HTTP_SERVER = 0,
	WIFI_APP_MSG_CONNECTING_FROM_HTTP_SEVER,
	WIFI_APP_MSG_STA_CONNECTING_GOT_IP

}wifi_app_message_e;

/*
 * structure for the message queue
 */
typedef struct wifi_app_queue_message
{
	wifi_app_message_e msgID;

}wifi_app_queue_message_t;

/*
 * send message to queue
 * @para msgID message ID from the wifi_app_message_e enum.
 * @return pdtrue if an item was successfully sent to the queue,otherwise pdfalse.
 *
 */

int  wifi_app_send_message(wifi_app_message_e msgID);

/*
 * Start the wifi RTOS task
 */


void wifi_app_start(void);



#endif /* MAIN_WIFI_APP_H_ */
