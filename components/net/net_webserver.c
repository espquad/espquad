#include "net.h"
#include "net_webserver.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <sys/param.h>
#include "esp_netif.h"
#include <esp_http_server.h>
#include "esp_event.h"
#include "esp_check.h"
#include <time.h>
#include <sys/time.h>



static const char *TAG = "net-webserver";


static esp_err_t drone_config_get_handler(httpd_req_t *req)
{
    const char* resp_str = "Drone parameters here...";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t drone_config_get_uri = {
    .uri       = "/drone-config",
    .method    = HTTP_GET,
    .handler   = drone_config_get_handler,
    .user_ctx  = NULL
};

static esp_err_t drone_config_post_handler(httpd_req_t *req)
{
    char buf[255] = {0};

    if (req->content_len > CONFIG_HTTP_REQ_BODY_MAX_LEN)
    {
        return ESP_FAIL;
    }

    if (httpd_req_recv(req, buf, MIN(req->content_len, sizeof(buf))) <= 0) {
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Received Body: %s", buf);
    ESP_LOG_BUFFER_HEXDUMP(TAG, buf, sizeof(buf), ESP_LOG_DEBUG);
    

    char param[CONFIG_HTTP_FORM_PARAM_VALUE_MAX_LEN] = {0};
    if (httpd_query_key_value(buf, "some_param", param, sizeof(param)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
        ESP_LOG_BUFFER_HEXDUMP(TAG, param, sizeof(param), ESP_LOG_DEBUG);
    }

    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);

    return ESP_OK;
}

static const httpd_uri_t drone_config_post_uri = {
    .uri       = "/drone-config",
    .method    = HTTP_POST,
    .handler   = drone_config_post_handler,
    .user_ctx  = NULL
};



static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &drone_config_get_uri);
        httpd_register_uri_handler(server, &drone_config_post_uri);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK) {
            *server = NULL;
        } else {
            ESP_LOGE(TAG, "Failed to stop http server");
        }
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

esp_err_t webserver_init(void)
{
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    server = start_webserver();

    while (server) {
        sleep(5);
    }
    return ESP_OK;
}