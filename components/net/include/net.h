#pragma once

#include <esp_log.h>
#include "esp_err.h"
#include "esp_wifi.h"
#include <stdio.h>


#define NETIF_DESC_STA "netif_sta"

esp_err_t esp_wifi_disconnect(void);

bool is_our_netif(const char *prefix, esp_netif_t *netif);

static void handler_on_sta_got_ip(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void wifi_start(void);

static void handler_on_wifi_disconnect(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

esp_err_t wifi_sta_do_connect(wifi_config_t wifi_config, bool wait);

esp_err_t wifi_sta_do_disconnect(void);

esp_err_t wifi_connect(void);

void wifi_stop(void);

void wifi_shutdown(void);


esp_err_t connect(void);
