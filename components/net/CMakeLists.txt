idf_component_register(SRCS "net.c"
                    REQUIRES esp_netif esp_http_server esp_wifi
                    INCLUDE_DIRS "include")
