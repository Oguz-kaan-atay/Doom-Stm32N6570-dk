/**
 * @file  appTask.c
 * @brief Uygulama task'larinin govdeleri. Task'lar main.c'de olusturulur.
 */
#include "appTask.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

/* ------------------------------------------------------------------ LED -- */

/* Iki LED de ayni task fonksiyonunu kullanir; farki parametre olarak gelen ayar */
void BlinkTask(void *arg)
{
  const BlinkConfig *cfg = (const BlinkConfig *)arg;
  TickType_t last_wake = xTaskGetTickCount();

  for (;;)
  {
    HAL_GPIO_TogglePin(cfg->port, cfg->pin);
    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(cfg->period_ms));
  }
}

/* -------------------------------------------------------------- Monitor -- */

/* Saniyede bir CPU yukunu ve FreeRTOS heap'ini, 10 saniyede bir task tablosunu yazar */
void MonitorTask(void *arg)
{
  (void)arg;
  static char stats[512];   /* static: task'in kendi stack'ini sisirmesin */

  uint32_t prev_idle  = ulTaskGetIdleRunTimeCounter();
  uint32_t prev_total = ulGetRunTimeCounter();
  uint32_t seconds    = 0;
  TickType_t last_wake = xTaskGetTickCount();

  for (;;)
  {
    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(1000));

    uint32_t idle  = ulTaskGetIdleRunTimeCounter();
    uint32_t total = ulGetRunTimeCounter();

    /* uint32_t farki, 32 bit sayac tasinca da dogru sonuc verir */
    uint32_t d_idle  = idle - prev_idle;
    uint32_t d_total = total - prev_total;
    prev_idle  = idle;
    prev_total = total;

    /* binde cinsinden yuk: 1000 = %100.0 */
    uint32_t load_x10 = 1000 - (uint32_t)(((uint64_t)d_idle * 1000) / d_total);

    printf("CPU: %lu.%lu %% | Heap bos: %u B (en az: %u B)\r\n",
           load_x10 / 10, load_x10 % 10,
           (unsigned)xPortGetFreeHeapSize(), (unsigned)xPortGetMinimumEverFreeHeapSize());

    if (++seconds % 10 == 0)
    {
      vTaskGetRunTimeStats(stats);
      printf("--- task / sure (us) / %%\r\n%s\r\n", stats);
    }
  }
}

