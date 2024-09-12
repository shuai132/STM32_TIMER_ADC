#include "app.h"
#include "log.h"
#include "md_adc.h"
#include "test_a.h"
#include "test_b.h"
#include <gpio.h>

void init(void) {
  test_func_a();
  test_func_b();
}

void setup(void) {
    LOGD("setup...");
    adcInit();
}

void loop(void) {
    LOGI("adc: %u, %u, %u, %u", ADCValue[0], ADCValue[1], ADCValue[2], ADCValue[3]);
    LOGI("mV:  %d, %u, %u", getVolmV(0), getVolmV(1), getVolmV(2));
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(1000);
}
