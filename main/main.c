#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "user_app.h"
#include "esp_log.h"

void app_main(void)
{
    BaseType_t task_err_code;
    ESP_LOGI("main", "the esp32sdk version :%s\n", esp_get_idf_version());
    task_err_code = xTaskCreate(startup_initialization_task, "startup_initialization_task", 1024 * 4, NULL, 5, NULL);

    if (task_err_code != pdPASS)
    {
        ESP_LOGI("main", "startup_initialization_task create failure,reason is:%d\n", task_err_code);
    }
}
