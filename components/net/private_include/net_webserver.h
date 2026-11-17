#pragma once

#include <esp_log.h>
#include "esp_err.h"
#include <stdio.h>
#include <esp_http_server.h>

#define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)

static esp_err_t drone_config_get_handler(httpd_req_t *req);

static esp_err_t drone_config_post_handler(httpd_req_t *req);

static httpd_handle_t start_webserver(void);

static esp_err_t stop_webserver(httpd_handle_t server);

static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

