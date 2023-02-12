#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "nvs_handler.h"
#include "touch.h"

#define TOUCH_PIN 4 

extern void mqtt_envia_mensagem(char *topico, char *mensagem);

int sensor_touch(){
    gpio_config_t io_conf;
    memset(&io_conf, 0, sizeof(io_conf));
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << TOUCH_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    int touch_value = gpio_get_level(TOUCH_PIN); 
    
    return touch_value;
    
}