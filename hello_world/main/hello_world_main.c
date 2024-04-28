/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

//#include <stdio.h>
//#include <inttypes.h>
//#include "sdkconfig.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "esp_chip_info.h"
//#include "esp_flash.h"

#include "nvs_flash.h"
#include "wifi_app.h"

void app_main(void)
{

	// Initialize NVS

		esp_err_t ret = nvs_flash_init();
		if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
		{
			ESP_ERROR_CHECK(nvs_flash_erase());
			ret = nvs_flash_init();
		}

		// Start Wifi

			wifi_app_start();
}
