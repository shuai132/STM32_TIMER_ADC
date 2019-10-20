#include <adc.h>
#include <tim.h>
#include "md_adc.h"
#include "log.h"

volatile uint16_t ADCValue[CHANNEL_NUL];

/**
 * 当前ADC时钟12M 通道4个 采样时间239.5个时钟
 * 所以每组采样频率最快为: 12.5kHz (12000 / 4 / 239.5)
 */
void adcInit(void) {
    const int ChannelNum = 4;

    // 自动校准
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
        FATAL();
    }

    // start adc dma
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCValue, ChannelNum) != HAL_OK) {
        FATAL();
    }

    // 设置采样频率: 1kHz
    htim3.Init.Prescaler = 72 - 1;
    htim3.Init.Period = 1000 - 1;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        FATAL();
    }

    // start timer
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        FATAL();
    }
}

/**
 * STM32F0每颗芯片有个基于某个电压及温度下的精确测定值: VREFINT_CAL
 * STM32F1里同样有内部基准电压，但它没有像F0一样每颗芯片有个精确测定值。
 * 其电压范围在1.16-1.26间，一般取1.2V。
 */
uint16_t getVolmV(int ch) {
    assert_param(0 <= ch && ch < CHANNEL_NUL);
    return 1200 * ADCValue[ch] / ADCValue[3];
}

/****************** weak callback ******************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    //LOGD("HAL_ADC_ConvCpltCallback");
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //LOGD("HAL_TIM_PeriodElapsedCallback");
}
