#include "Arduino.h"
#include "InternalTempSensor.h"

ADC_HandleTypeDef AdcHandle;

InternalTempSensorClass::InternalTempSensorClass(void)
{
    InitializeADC();
    CalibrateADC();
    ConfigureADC();
    EnableADC();
}

InternalTempSensorClass::~InternalTempSensorClass(void)
{
    DisableADC();
    delete this;
}

void InternalTempSensorClass::InitializeADC()
{
    AdcHandle.Instance = ADC1;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_FALLING;

    /* Initialize ADC */
    HAL_ADC_Init(&AdcHandle);
}

void InternalTempSensorClass::ConfigureADC()
{
    /* performs a self-calibration of the ADC */
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    /* Configure channel */
    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

    /* Wake-up the Temperature sensor */
    ADC->CCR |= ADC_CCR_TSEN;
}

void InternalTempSensorClass::CalibrateADC()
{
    /* Start calibration */
    HAL_ADCEx_Calibration_Start(&AdcHandle);
}

void InternalTempSensorClass::EnableADC()
{
    /* Enable ADC Pheripheral */
    __HAL_ADC_ENABLE(&AdcHandle);
}

void InternalTempSensorClass::DisableADC()
{
    /* Disable ADC Pheripheral */
    __HAL_ADC_DISABLE(&AdcHandle);
}

int32_t InternalTempSensorClass::ComputeTemperature(uint32_t measure)
{
    int32_t temperature;

    temperature = ((measure * VDD_APPLI / VDD_CALIB) - (int32_t)*TEMP30_CAL_ADDR);
    temperature *= (int32_t)(110 - 30);
    temperature /= (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
    return (temperature);
}

int32_t InternalTempSensorClass::GetTemperature(int calibration)
{

    HAL_ADC_Start(&AdcHandle);
    if (HAL_ADC_PollForConversion(&AdcHandle, 100) == HAL_OK)
    {
        int32_t sensorValue = (int32_t)HAL_ADC_GetValue(&AdcHandle);
        HAL_ADC_Stop(&AdcHandle);
        return ComputeTemperature(sensorValue) + calibration;
    }
    else
        return -273;
}

InternalTempSensorClass internalTemp;
