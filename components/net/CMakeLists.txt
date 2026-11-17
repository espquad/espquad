idf_component_register(SRCS "net_webserver.c" "net_wifi.c"
                    REQUIRES esp_netif esp_http_server esp_wifi
                    PRIV_INCLUDE_DIRS "private_include"
                    INCLUDE_DIRS "include")
