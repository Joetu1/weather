/*
 * @Author: mikey.zhaopeng 
 * @Date: 2018-08-19 09:27:22 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2018-08-26 12:48:16
 */

#include "user_app.h"

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "app_wifi.h"
#include "app_weather.h"

static const char *TAG = "USER_APP";

void startup_initialization_task(void *pvParameters)
{
	app_wifi_init();
	app_wifi_wait_connected();
	app_weather_get();

	uint32_t count = 0;
	while (1)
	{
		count++;
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

//end
