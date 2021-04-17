#pragma once

#include "main.h"

#define WHICH_LIGHTSENSOR(lightSensorAdcValue) ((lightSensorAdcValue < LIGHTSENSOR_THRESHOLD) ? "ON" : "OFF")
#define IS_LIGHT_ON(lightSensorAdcValue) ((lightSensorAdcValue < LIGHTSENSOR_THRESHOLD) ? true : false)
#define LIGHTSENSOR_THRESHOLD (1000u)

#define getLightSensorVal() ERROR_CHECK(HAL_ADC_Start_IT(&hadc1))