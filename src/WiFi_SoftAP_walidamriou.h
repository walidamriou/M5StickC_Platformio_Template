/* 

ESP WiFi Soft Access Point

Developed by: Walid Amriou

Website: www.walidamriou.com

Last update: December 2020

*/

#ifndef WIFI_SOFTAP_WALIDAMRIOU_H_INCLUDED
#define WIFI_SOFTAP_WALIDAMRIOU_H_INCLUDED

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_AP_SSID          "Tiniri Signal Generator"
#define WIFI_AP_PASS          "123456789"
#define ESP_WIFI_CHANNEL      0
#define WIFI_MAX_STA_CONN     1

static const char *TAG = "wiFi_softAP_walidamriou";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
void wifi_init_softap(void);
void WiFi_SoftAP_walidamriou(void);

#endif