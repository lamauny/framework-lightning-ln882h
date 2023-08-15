set(srcs "zj_adapt_ble.c"
        "zj_adapt_gpio.c"
        "zj_fota_adapt.c"
        "zj_iic_adapt.c"
        "zj_ir_adapt.c"
        "zj_pwm_adapt.c"
        "zj_rtc_time.c"
        "zj_sys_adapt.c"
        "zj_wlan_adapt.c"
        "zj_common_main.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  "."
                    REQUIRES mbedtls esp_hw_support esp_https_ota esp_wifi adapter_common bt driver esp_timer nvs_flash mbedtls bootloader_support app_update esp_http_client
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem )

