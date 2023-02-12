#ifndef _NVS_MODULE_H
#define _NVS_MODULE_H

#include "esp_system.h"

void init_NVS();
int32_t le_valor_nvs();
void grava_valor_nvs(char *chave, int32_t valor);

#endif