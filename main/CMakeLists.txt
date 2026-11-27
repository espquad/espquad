
idf_component_register(SRCS "main.c"
                    REQUIRES esp_driver_mcpwm nvs_flash esp_event net control
                    INCLUDE_DIRS ".")
