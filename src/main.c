#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
#include "gpio_setup.h"
#include "adc_module.h"
#include "dht11.h"
#include "touch.h"
#include "nvs_module.h"

#define LDR ADC_CHANNEL_0


SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  adc_init(ADC_UNIT_1);
  pinMode(LDR, GPIO_ANALOG);

  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       
      float temperatura = return_temp();
      sprintf(mensagem, "{\"temperature\": %f}", temperatura);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      printf("%s \n", mensagem);
      grava_valor_nvs("Temperature", temperatura);

      float humidade = return_hum();
      sprintf(mensagem, "{\"umidade\": %f}", humidade);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      printf("%s \n", mensagem);
      grava_valor_nvs("Umidade", humidade);


      int ldr = analogRead(LDR);
      sprintf(mensagem, "{\"LDR\": %d}", ldr);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      printf("%s \n", mensagem);
      grava_valor_nvs("LDR", ldr);


      int status_touch = sensor_touch();
      sprintf(mensagem, "{\"Touch\": %d}", status_touch);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      printf("%s \n", mensagem);
      grava_valor_nvs("Touch", status_touch);

      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void app_main(void){

    init_NVS();
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();

    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}
