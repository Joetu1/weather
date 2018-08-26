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

static const char *TAG = "APP_WEATHER";

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
		ESP_LOGI(TAG, "Http get Status = %d, conent_length = %d",
				 esp_http_client_get_status_code(client),
				 esp_http_client_get_content_length(client));

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
	}
	else
	{
		ESP_LOGE(TAG, "Http get resquest failed:%s", esp_err_to_name(err));
	}

	esp_http_client_cleanup(client);
}
