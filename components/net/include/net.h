#pragma once

#include <esp_log.h>
#include "esp_err.h"
#include "esp_wifi.h"
#include <stdio.h>



esp_err_t connect_to_wifi(void);

esp_err_t webserver_init(void);