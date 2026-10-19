#pragma once

#include "main.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  uint32_t      period_ms;
} BlinkConfig;

void BlinkTask(void *arg);
void MonitorTask(void *arg);
