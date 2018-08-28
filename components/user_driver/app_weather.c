/*
 * @Author: mikey.zhaopeng 
 * @Date: 2018-08-26 12:01:29 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2018-08-26 13:37:05
 */
#include "app_weather.h"

#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "cJSON.h"
#include "os.h"

static const char *TAG = "APP_WEATHER";
static char *recv_json_buf = NULL;
static char http_get_buf[1024 * 2];

static void https_get_reuest_json_data_parse(cJSON *json_data)
{
		cJSON *results = cJSON_GetObjectItem(json_data, "results");
		char *str = cJSON_Print(results);
		ESP_LOGI("", "%s", str);

	// cJSON *location = cJSON_GetObjectItem(cJSON_GetArrayItem(results, 0), "location");
	// cJSON *daily = cJSON_GetObjectItem(cJSON_GetArrayItem(results, 0), "daily");
	// ESP_LOGI("", "city: %s\n%s\n%s\n%s\n",
	// 		 cJSON_Print(cJSON_GetObjectItem(location, "name")),
	// 		 cJSON_Print(cJSON_GetArrayItem(daily, 0)),
	// 		 cJSON_Print(cJSON_GetArrayItem(daily, 1)),
	// 		 cJSON_Print(cJSON_GetArrayItem(daily, 2)));
}

static void https_get_weather_json_task(void *pvParameters)
{
	char *json_data = (char *)pvParameters;
	https_get_reuest_json_data_parse(cJSON_Parse(os_strchr(json_data, '{')));
	os_free(json_data);
	vTaskDelete(NULL);
}

esp_err_t http_event_hander(esp_http_client_event_t *evt)
{
	switch (evt->event_id)
	{
	case HTTP_EVENT_ERROR:

		ESP_LOGD("http_event:", "HTTP_EVENT_ERROR!");
		break;
	case HTTP_EVENT_ON_CONNECTED:

		ESP_LOGD("http_event:", "HTTP_EVENT_ON_CONNECTED!");
		break;
	case HTTP_EVENT_HEADER_SENT:

		ESP_LOGD("http_event:", "HTTP_EVENT_HEADER_SENT!");
		break;
	case HTTP_EVENT_ON_HEADER:

		ESP_LOGD("http_event:", "HTTP_EVENT_ON_HEADER!");
		break;
	case HTTP_EVENT_ON_DATA:

		ESP_LOGD("http_event:", "HTTP_EVENT_ON_DATA!");
		break;
	case HTTP_EVENT_ON_FINISH:

		ESP_LOGD("http_event:", "HTTP_EVENT_ON_FINISH!");
		break;
	case HTTP_EVENT_DISCONNECTED:

		ESP_LOGD("http_event:", "HTTP_EVENT_DISCONNECTED!");
		break;

	default:
		break;
	}
	return ESP_OK;
}

void app_weather_get()
{
	int content_length = 0;

	esp_http_client_config_t config =
		{
			.url =
				"https://api.seniverse.com/v3/weather/now.json?key=6cnvwhof6kjnljuc&location=zhengzhou&language=en&unit=c",
			.event_handler = &http_event_hander,
		};
	esp_http_client_handle_t client = esp_http_client_init(&config);
	esp_err_t err = esp_http_client_perform(client);

	if (err == ESP_OK)
	{
		content_length = esp_http_client_get_content_length(client);

		ESP_LOGI(TAG, "Http get Status = %d, conent_length = %d",
				 esp_http_client_get_status_code(client),
				 content_length);

		esp_http_client_transport_t client_transpor = esp_http_client_get_transport_type(client);

		switch (client_transpor)
		{
		case HTTP_TRANSPORT_UNKNOWN:
			ESP_LOGI(TAG, "HTTP_TRANSPORT_UNKNOWN!");
			break;
		case HTTP_TRANSPORT_OVER_TCP:
			ESP_LOGI(TAG, "HTTP_TRANSPORT_OVER_TCP!");
			break;
		case HTTP_TRANSPORT_OVER_SSL:
			ESP_LOGI(TAG, "HTTP_TRANSPORT_OVER_SSL!");
			break;

		default:
			break;
		}

		if (-1 != esp_http_client_read(client, http_get_buf, content_length))
		{
			recv_json_buf = (char *)os_malloc(1024);
			os_memcpy(recv_json_buf, http_get_buf, content_length);

			xTaskCreate(https_get_weather_json_task,
						"https_get_weather_json_task",
						1024 * 4,
						(void *)recv_json_buf,
						4,
						NULL);
			//ESP_LOGI(TAG, "%s", http_get_buf);
		}
		else
		{
			ESP_LOGE(TAG, " esp_http_client_read error");
		}
	}
	else
	{
		ESP_LOGE(TAG, "Http get resquest failed:%s", esp_err_to_name(err));
	}
	esp_http_client_cleanup(client);
}
