/* 

Tiniri Signal Generator
Open source WebThings device to generate electronic signals with set of properties 

Developed by: Walid Amriou

Website: www.walidamriou.com

Last update: December 2020

Walid Amriou © copyright 

*/

#include <WiFi_SoftAP_walidamriou.h>

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_eth.h"
#include <esp_http_server.h>

// URI handler function to be called during GET /TiniriSignalGenerator/about request 
esp_err_t get_about_handler(httpd_req_t *req){
    const char resp[] = " {\"device\": \"Tiniri Signal Generator\",\"Developer\": \"Walid Amriou\",\"Firmware version\": \"1.0.0\",\"Chip\": \"Espressif Systems ESP32 PICO (4M)\",}";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// URI handler function to be called during GET /TiniriSignalGenerator/CurrentStatus request 
esp_err_t get_CurrentStatus_handler(httpd_req_t *req){
    const char resp[] = " {\"GeneratorStatus\": \"stop\",\"SignalType\": \"Square\",\"Frequncy\": 10,\"DutyCycle\": 50,}";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// URI handler function to be called during POST /TiniriSignalGenerator/Config request 
esp_err_t post_handler(httpd_req_t *req){
    /* Destination buffer for content of HTTP POST request.
     * httpd_req_recv() accepts char* only, but content could
     * as well be any binary data (needs type casting).
     * In case of string data, null termination will be absent, and
     * content length would give length of string */
    char content[100];

    /* Truncate if content length larger than the buffer */
    size_t recv_size = MIN(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {  /* 0 return value indicates connection closed */
        /* Check if timeout occurred */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            /* In case of timeout one can choose to retry calling
             * httpd_req_recv(), but to keep it simple, here we
             * respond with an HTTP 408 (Request Timeout) error */
            httpd_resp_send_408(req);
        }
        /* In case of error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }

    /* Send a simple response */
    const char resp[] = "URI POST Response";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// URI handler structure for GET /TiniriSignalGenerator/about 
httpd_uri_t uri_get_about = {
    .uri      = "/TiniriSignalGenerator/about",
    .method   = HTTP_GET,
    .handler  = get_about_handler,
    .user_ctx = NULL
};

// URI handler structure for GET /TiniriSignalGenerator/CurrentStatus
httpd_uri_t uri_get_CurrentStatus = {
    .uri      = "/TiniriSignalGenerator/CurrentStatus",
    .method   = HTTP_GET,
    .handler  = get_CurrentStatus_handler,
    .user_ctx = NULL
};

// URI handler structure for POST /TiniriSignalGenerator/Config
httpd_uri_t uri_post = {
    .uri      = "/TiniriSignalGenerator/Config",
    .method   = HTTP_POST,
    .handler  = post_handler,
    .user_ctx = NULL
};

/* Function for starting the webserver */
httpd_handle_t start_webserver(void){
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_get_about);
        httpd_register_uri_handler(server, &uri_get_CurrentStatus);
        httpd_register_uri_handler(server, &uri_post);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}


void app_main(void){
    //static httpd_handle_t server = NULL;

    //ESP_ERROR_CHECK(nvs_flash_init());
    //ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

   // Create WiFi Access point 
   WiFi_SoftAP_walidamriou();

    /* Start the server for the first time */
   start_webserver();
}


