/*
 * @Author: mikey.zhaopeng 
 * @Date: 2018-08-26 10:05:00 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2018-08-26 13:05:37
 */

#include "app_wifi.h"

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/event_groups.h" //TODO:路径问题

#include "sdkconfig.h"

static const char *TAG = "APP_WIFI";

static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_ERROR_CHECK(esp_wifi_connect())
            ;
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START! ");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP! ");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED! ");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            /* This is a workaround as ESP32 WiFi libs don't currently
             auto-reassociate. */
            ESP_ERROR_CHECK(esp_wifi_connect())
            ;
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED! ");
            break;
        default:
            break;
    }

    return ESP_OK;
}
void app_wifi_init()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT()
    ;
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = { .sta = { .ssid = CONFIG_WIFI_SSID, //TODO: KCONFIG的使用
            .password = CONFIG_WIFI_PASSWORD, } };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
void app_wifi_wait_connected()
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true,
    portMAX_DELAY);
}
