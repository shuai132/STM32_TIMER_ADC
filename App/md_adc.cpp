#include <adc.h>
#include <tim.h>
#include <cmath>
#include "md_adc.h"
#include "log.h"
#include "PortableMCU.h"

volatile uint16_t ADCValue[CHANNEL_NUM];

void adcInit() {
    // 自动校准
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
        FATAL();
    }

    // start adc dma
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCValue, CHANNEL_NUM) != HAL_OK) {
        FATAL();
    }

    // 初始采样频率: 10kHz
    adcSetFrequency(10000);

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        FATAL();
    }

    // start timer
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        FATAL();
    }
}

void adcSetFrequency(uint32_t frequency) {
    const int SYS_MHZ = 72;
    const uint32_t CLOCKS = SYS_MHZ * 1000000;
    uint32_t period = CLOCKS / frequency;

    uint16_t p1 = 0;
    uint16_t p2 = 0;
    {
        uint16_t pm = sqrt(period);
        do {
            if (pm == 1) {
                p1 = 72;
                p2 = 1000000 / frequency;
                break;
            }
            if (period % pm == 0) {
                p1 = pm;
                if (p1 * SYS_MHZ > 0xFFFF) continue;
                p2 = period / pm;
                break;
            }
        } while (--pm);
    }
    htim3.Init.Prescaler = p1 - 1;
    htim3.Init.Period    = p2 - 1;

    using namespace G;
    message.sampleFs = CLOCKS / (p1 * p2);
}

void adcSetSampleNum(uint32_t sampleNum) {
    using namespace G;
    message.sampleNum = sampleNum;
}

/**
 * STM32F0每颗芯片有个基于某个电压及温度下的精确测定值: VREFINT_CAL
 * STM32F1里同样有内部基准电压，但它没有像F0一样每颗芯片有个精确测定值。
 * 其电压范围在1.16-1.26间，一般取1.2V。
 */
uint16_t getVolmV(int ch) {
    assert_param(0 <= ch && ch < CHANNEL_NUM);
    return 1200 * ADCValue[ch] / ADCValue[CHANNEL_NUM - 1];
}

/****************** weak callback ******************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    using namespace G;

    if (sampleOk) return;

    message.sampleCh1[samplePos] = getVolmV(0);
    if (++samplePos == message.sampleNum) {
        HAL_TIM_Base_Stop_IT(&htim3);
        samplePos = 0;
        sampleOk = true;
    }
}