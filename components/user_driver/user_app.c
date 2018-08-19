/*
 * @Author: mikey.zhaopeng 
 * @Date: 2018-08-19 09:27:22 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2018-08-19 10:31:30
 */

#include "user_app.h"

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "USER_APP";

esp_err_t event_handler(void *ctx, system_event_t *event)
{

    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        ESP_ERROR_CHECK(esp_wifi_connect());
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START! ");
        break;

    default:
        break;
    }

    return ESP_OK;
}

static void user_wifi_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Snow_5G",
            .password = "Joetu12345",
        }};
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}


void startup_initialization_task(void* pvParameters)
{
    user_wifi_init();
    uint32_t count = 0;
    while (1)
    {   
        count++;
        ESP_LOGI(TAG,"Count == %d", count);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


//end
