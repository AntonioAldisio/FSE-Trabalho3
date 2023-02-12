#ifndef NVS_H
#define NVS_H

#include "esp_system.h"

int32_t le_valor_nvs(char *chave);
void grava_valor_nvs(char *chave, int32_t valor);

#endif