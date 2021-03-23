/*
 * lora.c
 *
 *  Created on: Mar 16, 2021
 *      Author: Ly Van Minh
 */
#include "lora.h"
#include "main.h"
#include "string.h"

ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/**
  * @brief Write Data Function To Lora Module
  * @param ucAddress: Address registers or fifo of Lora
  * @param ucData: Data need write into registers or fifo of Lora
  * @retval None
  */

void vUart1Transmit(uint8_t* pcString)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)pcString, strlen(pcString), 200);
}

void vSpi1Write(uint8_t ucAddress, uint8_t ucData)
{
    ucAddress |= 0x80; /* A wnr bit, which is 1 for write access and 0 for read access */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, LED_OUTPUT_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucAddress, sizeof(ucAddress), 100);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucData, sizeof(ucData), 100);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, LED_OUTPUT_Pin, GPIO_PIN_SET);
}

/**
  * @brief Read Data Function from Lora Module
  * @param ucAddress: Address registers or fifo of Lora Module
  * @retval ucData: Data contained in registers or fifo of Lora Module  
  */
uint8_t ucSpi1Read(uint8_t ucAddress)
{
    uint8_t ucData = 0;
    ucAddress &= 0x7F; /* A wnr bit, which is 1 for write access and 0 for read access */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, LED_OUTPUT_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucAddress, sizeof(ucAddress), 100);
    HAL_SPI_Receive(&hspi1, (uint8_t *)&ucData, sizeof(ucData), 100);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, LED_OUTPUT_Pin, GPIO_PIN_SET);
    return ucData;
}

uint8_t ucReadFifo(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegFifo);
    return ucData;
}

void vWriteFifo(uint8_t ucData)
{
    vSpi1Write(RegFifo, ucData);
}

/**
  * @brief Mode Initialization LoraTM or FSK/OOK 
  * @param ucMode: Value of mode: LoraTM if 1, FSK/OOK if 0
  * @retval None
  */
void vLongRangeModeInit(uint8_t ucLongRangeMode)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOpMode);
    ucData &= 0x7F;
    if(ucLongRangeMode == 0u || ucLongRangeMode == 1u)
    {
        ucData |= (ucLongRangeMode << 7);
        vSpi1Write(RegOpMode, ucData);
    }
}

void vAccessSharedRegInit(uint8_t ucAccessSharedReg)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOpMode);
    ucData &= 0xBF;
    if(ucAccessSharedReg == 0u || ucAccessSharedReg == 1u)
    {
        ucData |= (ucAccessSharedReg << 6);
        vSpi1Write(RegOpMode, ucData);
    }
}

void vLowFrequencyModeOnInit(uint8_t ucLowFrequencyModeOn)
{
    uint8_t ucData = 0;
    ucData &= 0xF7;
    if(ucLowFrequencyModeOn == 0u || ucLowFrequencyModeOn == 1u)
    {
        ucData |= (ucLowFrequencyModeOn << 3);
        vSpi1Write(RegOpMode, ucData);
    }
}

/**
  * @brief Device Modes Initialization  
  * @param ucDeviceMode: Value of mode
  * @retval None
  */
void vModeInit(uint8_t ucMode)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOpMode);
    ucData &= 0xF8;
    if(ucMode >= 0 && ucMode <= 7)
    {
        ucData |= ucMode;
        vSpi1Write(RegOpMode, ucData);
    }
}

void vFrfInit(unsigned int uiFrf)
{
    vSpi1Write(RegFrfMsb, (uint8_t)(uiFrf >> 16));
    vSpi1Write(RegFrfMid, (uint8_t)(uiFrf >> 8));
    vSpi1Write(RegFrfLsb, (uint8_t)uiFrf);
}
/**
  * @brief Power Amplifier Initialization
  * @param None
  * @retval None
  */
void vPaSelectInit(uint8_t ucPaSelect)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaConfig);
    ucData &= 0x7F;
    if(ucPaSelect == 0u || ucPaSelect == 1u)
    {
        ucData |= (ucPaSelect << 7);
        vSpi1Write(RegPaConfig, ucData);
    }
}

void vMaxPowerInit(uint8_t ucMaxPower)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaConfig);
    ucData &= 0x8F;
    if(ucMaxPower >= 0u && ucMaxPower <= 7u)
    {
        ucData |= (ucMaxPower << 4);
        vSpi1Write(RegPaConfig, ucData);
    }    
}

void vOutputPowerInit(uint8_t ucOutputPower)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaConfig);
    ucData &= 0xF0;
    if(ucOutputPower >= 0u && ucOutputPower <= 15u)
    {
        ucData |= ucOutputPower;
        vSpi1Write(RegPaConfig, ucData);
    }     
}

void vPaRampInit(uint8_t ucPaRamp)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaConfig);
    ucData &= 0xF0;
    if(ucPaRamp >= 0u && ucPaRamp <= 15u)
    {
        ucData |= ucPaRamp;
        vSpi1Write(RegPaRamp, ucData);
    }    
}

void vOcpOnInit(uint8_t ucOcpOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOcp);
    ucData &= 0xDF;
    if(ucOcpOn == 0u || ucOcpOn == 1u)
    {
        ucData |= (ucOcpOn << 5);
        vSpi1Write(RegOcp, ucData);
    }    
}

/**
  * @brief  Overload Current Protection Initialization
  * @param None
  * @retval None
  */
void vOcpTrim(uint8_t ucOcpTrim)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOcp);
    ucData &= 0xE0;
    if(ucOcpTrim == 0u || ucOcpTrim == 32u)
    {
        ucData |= ucOcpTrim;
        vSpi1Write(RegOcp, ucData);
    }
}

void vLnaGainInit(uint8_t ucLnaGain)
{   
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegLna);
    ucData &= 0x1F;
    if(ucLnaGain >= 1u && ucLnaGain <= 6u)
    {
        ucData |= (ucLnaGain << 5);
        vSpi1Write(RegLna, ucData);
    }   
}

void vLnaBoostLfInit(uint8_t ucLnaBoostLf)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegLna);
    ucData &= 0xE7;
    if(ucLnaBoostLf == 0u)
    {
        ucData |= (ucLnaBoostLf << 3);
        vSpi1Write(RegLna, ucData);
    }    
}

void vLnaBoostHfInit(uint8_t ucLnaBoostHf)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegLna);
    ucData &= 0xFC;
    if(ucLnaBoostHf == 0u || ucLnaBoostHf == 3u)
    {
        ucData |= ucLnaBoostHf;
        vSpi1Write(RegLna, ucData);
    }  
}

uint8_t ucFifoAddrPtrRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegFifoAddrPtr);
    return ucData;
}

void vFifoAddrPtrWrite(uint8_t ucFifoAddrPtr)
{
    vSpi1Write(RegFifoAddrPtr, ucFifoAddrPtr);
}

void vFifoTxBaseAddrInit(uint8_t ucFifoTxBaseAddr)
{
    vSpi1Write(RegFifoTxBaseAddr, ucFifoTxBaseAddr);     
}

void vFifoRxBaseAddrInit(uint8_t ucFifoRxBaseAddr)
{
    vSpi1Write(RegFifoRxBaseAddr, ucFifoRxBaseAddr);
}

uint8_t ucFifoRxCurrentAddrRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegFifoRxCurrentAddr);
    return ucData;

}

// void vRxTimeoutMaskInit(uint8_t ucRxTimeoutMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0x7F;
//     if(ucRxTimeoutMask == 0u || ucRxTimeoutMask == 1u)
//     {
//         ucData |= (ucRxTimeoutMask << 7);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vRxDoneMaskInit(uint8_t ucRxDoneMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xBF;
//     if(ucRxDoneMask == 0u || ucRxDoneMask == 1u)
//     {
//         ucData |= (ucRxDoneMask << 6);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }    
// }

// void vPayloadCrcErrorMaskInit(uint8_t ucPayloadCrcErrorMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xDF;
//     if(ucPayloadCrcErrorMask == 0u || ucPayloadCrcErrorMask == 1u)
//     {
//         ucData |= (ucPayloadCrcErrorMask << 5);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }      
// }

// void vValidHeaderMaskInit(uint8_t ucValidHeaderMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xEF;
//     if(ucValidHeaderMask == 0u || ucValidHeaderMask == 1u)
//     {
//         ucData |= (ucValidHeaderMask << 4);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }      
// }

// void vTxDoneMaskInit(uint8_t ucTxDoneMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xF7;
//     if(ucTxDoneMask == 0u || ucTxDoneMask == 1u)
//     {
//         ucData |= (ucTxDoneMask << 3);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }     
// }

// void vCadDoneMaskInit(uint8_t ucCadDoneMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xFB;
//     if(ucCadDoneMask == 0u || ucCadDoneMask == 1u)
//     {
//         ucData |= (ucCadDoneMask << 2);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }    
// }

// void vFhssChangeChannelMaskInit(uint8_t ucFhssChangeChannelMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xFD;
//     if(ucFhssChangeChannelMask == 0u || ucFhssChangeChannelMask == 1u)
//     {
//         ucData |= (ucFhssChangeChannelMask << 1);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }     
// }

// void vCadDetectedMaskInit(uint8_t ucCadDetectedMask)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xFE;
//     if(ucCadDetectedMask == 0u || ucCadDetectedMask == 1u)
//     {
//         ucData |= ucCadDetectedMask;
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }   
// }

void vIrqFlagsMaskInit(uint8_t ucIrqFlagsMask)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlagsMask);
    ucData |= ucIrqFlagsMask;
    vSpi1Write(RegIrqFlagsMask, ucData);
}

uint8_t ucIrqFlagsRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlags);
    return ucData;
}

void vIrqFlagsClear(uint8_t ucIrqFlags)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlags);
    ucData |= ucIrqFlags;
    vSpi1Write(RegIrqFlags, ucData);    
}

uint8_t ucFifoRxBytesNbRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegRxNbBytes);
    return ucData;    
}

uint16_t usValidHeaderCntRead(void)
{
    uint16_t ucData = 0;
    ucData = ((uint16_t)ucSpi1Read(RegRxHeaderCntValueMsb) << 8);
    ucData |= (uint16_t)ucSpi1Read(RegRxHeaderCntValueLsb);
    return ucData;    
}

uint16_t usValidPacketCntRead(void)
{
    uint16_t ucData = 0;
    ucData = ((uint16_t)ucSpi1Read(RegRxPacketCntValueMsb) << 8);
    ucData |= (uint16_t)ucSpi1Read(RegRxPacketCntValueLsb);
    return ucData;    
}

uint8_t ucRxCodingRateRead(void)
{
    uint8_t ucData;
    ucData = (ucSpi1Read(RegModemStat) >> 5);
    return ucData;
}

uint8_t ucModemStatusRead(void)
{
    uint8_t ucData;
    ucData = 0x1F & ucSpi1Read(RegModemStat);
    return ucData;    
}

uint8_t ucPacketSnrRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegPktSnrValue);
    return ucData;    
}

uint8_t ucPacketRssiRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegPktRssiValue);
    return ucData;   
}

uint8_t ucRssiRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegRssiValue);
    return ucData;    
}

uint8_t ucPllTimeoutRead(void)
{
    uint8_t ucData;
    ucData = (ucSpi1Read(RegHopChannel) >> 7);
    return ucData;    
}

uint8_t ucCrcOnPayloadread(void)
{
    uint8_t ucData;
    ucData = 0x01 & (ucSpi1Read(RegHopChannel) >> 6);
    return ucData;    
}

uint8_t ucFhssPresentChannelRead(void)
{
    uint8_t ucData;
    ucData = 0x3F & ucSpi1Read(RegHopChannel);
    return ucData; 
}

void vBandWidthInit(uint8_t ucBandWidth)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig1);
    ucData &= 0x0F;
    if(ucBandWidth >= 0u && ucBandWidth <= 9u)
    {
        ucData |= (ucBandWidth << 4);
        vSpi1Write(RegModemConfig1, ucData);
    }
}

void vCodingRateInit(uint8_t ucCodingRate)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig1);
    ucData &= 0xF1;
    if(ucCodingRate >= 0u && ucCodingRate <= 4u)
    {
        ucData |= (ucCodingRate << 1);
        vSpi1Write(RegModemConfig1, ucData);
    }
}

void vImplicitHeaderModeOnInit(uint8_t ucHeaderMode)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig1);
    ucData &= 0xFE;
    if(ucHeaderMode == 0u || ucHeaderMode == 1u)
    {
        ucData |= ucHeaderMode;
        vSpi1Write(RegModemConfig1, ucData);
    }
}

void vSpreadingFactor(uint8_t ucSpreadingFactor)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig2);
    ucData &= 0x0F;
    if(ucSpreadingFactor >= 6u && ucSpreadingFactor <= 12u)
    {
        ucData |= (ucSpreadingFactor << 4);
        vSpi1Write(RegModemConfig2, ucData);
    }

}

void vTxContinuousModeInit(uint8_t ucTxContinuousMode)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig2);
    ucData &= 0xF7;
    if(ucTxContinuousMode == 0u || ucTxContinuousMode == 1u)
    {
        ucData |= (ucTxContinuousMode << 3);
        vSpi1Write(RegModemConfig2, ucData);
    }
}

void vRxPayloadCrcOnInit(uint8_t ucRxPayloadCrcOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig2);
    ucData &= 0xFB;
    if(ucRxPayloadCrcOn == 0u || ucRxPayloadCrcOn == 1u)
    {
        ucData |= (ucRxPayloadCrcOn << 2);
        vSpi1Write(RegModemConfig2, ucData);
    }
}

void vSymbTimeoutInit(uint16_t ucSymbTimeout)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig1);
    ucData &= 0xFC;
    ucData |= ((0x03) & (uint8_t)(ucSymbTimeout >> 8));
    vSpi1Write(RegModemConfig1, ucData);
    vSpi1Write(RegModemConfig2, (uint8_t)ucSymbTimeout);
}

void vPreambleLengthInit(uint16_t ucPreambleLength)
{
    vSpi1Write(RegPreambleMsb, (uint8_t)(ucPreambleLength >> 8));
    vSpi1Write(RegPreambleLsb, (uint8_t)(ucPreambleLength));
}

void vPayloadLengthInit(uint8_t ucPayloadLength)
{
    vSpi1Write(RegPayloadLength, ucPayloadLength);
}

void vPayloadMaxLengthInit(uint8_t ucPayloadMaxLength)
{
    vSpi1Write(RegMaxPayloadLength, ucPayloadMaxLength);    
}

void vFreqHoppingPeriodInit(uint8_t ucFreqHoppingPeriod)
{
    vSpi1Write(RegHopPeriod, ucFreqHoppingPeriod);
}

uint8_t ucFifoRxByteAddrPtr(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegFifoRxByteAddr);
    return ucData;
}

void vLowDataRateOptimizeInit(uint8_t ucLowDataRateOptimize)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig3);
    ucData &= 0xF7;
    if(ucLowDataRateOptimize == 0u || ucLowDataRateOptimize == 1u)
    {
        ucData |= (ucLowDataRateOptimize << 3);
        vSpi1Write(RegModemConfig3, ucData);
    }
}

void vAgcAutoOnInit(uint8_t ucAgcAutoOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig3);
    ucData &= 0xF7;
    if(ucAgcAutoOn == 0u || ucAgcAutoOn == 1u)
    {
        ucData |= (ucAgcAutoOn << 2);
        vSpi1Write(RegModemConfig3, ucData);
    }    
}

unsigned int uiFreqError(void)
{
    unsigned int uiData = 0;
    uiData |= (((unsigned int)(ucSpi1Read(RegFeiMsb) & 0x0F) << 16) | \
              ((unsigned int)ucSpi1Read(RegFeiMid) << 8) | \
              (unsigned int)ucSpi1Read(RegFeiLsb));
    return uiData;
}

uint8_t ucRssiWidebandInit(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegRssiWideband);
    return ucData;
}

void vDetectionOptimizeInit(uint8_t ucDetectionOptimize)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDetectOptimize);
    ucData &= 0xF8;
    if(ucDetectionOptimize == 3u || ucDetectionOptimize == 5u)
    {
        ucData |= ucDetectionOptimize;
        vSpi1Write(RegDetectOptimize, ucData);
    }
}

void vInvertIQInit(uint8_t ucInvertIQ)
{
    uint8_t ucData = 0;
    ucData &= 0xBF;
    if(ucInvertIQ == 0u || ucInvertIQ == 1u)
    {
        ucData |= (ucInvertIQ << 6);
        vSpi1Write(RegInvertIQ, ucData);
    }
}

void vDetectionThresholdInit(uint8_t ucDetectionThreshold)
{
    if(ucDetectionThreshold == 0x0A || ucDetectionThreshold == 0x0C)
    {
        vSpi1Write(RegDetectionThreshold, ucDetectionThreshold);
    }
}

void vSyncWordInit(uint8_t ucSyncWord)
{
    vSpi1Write(RegSyncWord, ucSyncWord);
}

void vDio0MappingInit(uint8_t ucDio0Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping1);
    ucData &= 0x3F; 
    if(ucDio0Mapping >= 0u && ucDio0Mapping <= 3u)
    {
       ucData |= (ucDio0Mapping << 6);
       vSpi1Write(RegDioMapping1, ucData);
    }
}

void vDio1MappingInit(uint8_t ucDio1Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping1);
    ucData &= 0xCF; 
    if(ucDio1Mapping >= 0u && ucDio1Mapping <= 3u)
    {
       ucData |= (ucDio1Mapping << 4);
       vSpi1Write(RegDioMapping1, ucData);
    }
}

void vDio2MappingInit(uint8_t ucDio2Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping1);
    ucData &= 0xF3; 
    if(ucDio2Mapping >= 0u && ucDio2Mapping <= 3u)
    {
       ucData |= (ucDio2Mapping << 2);
       vSpi1Write(RegDioMapping1, ucData);
    }
}

void vDio3MappingInit(uint8_t ucDio3Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping1);
    ucData &= 0xFC; 
    if(ucDio3Mapping >= 0u && ucDio3Mapping <= 3u)
    {
       ucData |= ucDio3Mapping;
       vSpi1Write(RegDioMapping1, ucData);
    }
}

void vDio4MappingInit(uint8_t ucDio4Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping2);
    ucData &= 0x3F; 
    if(ucDio4Mapping >= 0u && ucDio4Mapping <= 3u)
    {
       ucData |= (ucDio4Mapping << 6);
       vSpi1Write(RegDioMapping2, ucData);
    }
}

void vDio5MappingInit(uint8_t ucDio5Mapping)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping2);
    ucData &= 0xCF; 
    if(ucDio5Mapping >= 0u && ucDio5Mapping <= 3u)
    {
       ucData |= (ucDio5Mapping << 4);
       vSpi1Write(RegDioMapping2, ucData);
    }
}

void vMapPreambleDetectInit(uint8_t ucMapPreambleDetect)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegDioMapping2);
    ucData &= 0xFE; 
    if(ucMapPreambleDetect == 0u || ucMapPreambleDetect == 1u)
    {
       ucData |= ucMapPreambleDetect;
       vSpi1Write(RegDioMapping2, ucData);
    }   
}

uint8_t ucVersionRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegVersion);
    return ucData;   
}

void vTcxoInputOnInit(uint8_t ucTcxoInputOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegTcxo);
    ucData &= 0xEF; 
    if(ucTcxoInputOn == 0u || ucTcxoInputOn == 1u)
    {
       ucData |= (ucTcxoInputOn << 4);
       vSpi1Write(RegTcxo, ucData);
    }
}

void vPaDacInit(uint8_t ucPaDac)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaDac);
    ucData &= 0xF8; 
    if(ucPaDac >= 0u && ucPaDac <= 3u)
    {
       ucData |= ucPaDac;
       vSpi1Write(RegPaDac, ucData);
    }
}

void vFormerTempInit(uint8_t ucFormerTemp)
{
    vSpi1Write(RegFormerTemp, ucFormerTemp);
}

void vAgcReferenceLevelInit(uint8_t ucAgcReferenceLevel)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcRef);
    ucData &= 0xC0; 
    if(ucAgcReferenceLevel >= 0x00 && ucAgcReferenceLevel <= 0x3F)
    {
       ucData |= ucAgcReferenceLevel;
       vSpi1Write(RegAgcRef, ucData);
    }
}

void vAgcStep1Init(uint8_t ucAgcStep1)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcThresh1);
    ucData &= 0xE0; 
    if(ucAgcStep1 >= 0x00 && ucAgcStep1 <= 0x1F)
    {
       ucData |= ucAgcStep1;
       vSpi1Write(RegAgcThresh1, ucData);
    }    
}

void vAgcStep2Init(uint8_t ucAgcStep2)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcThresh2);
    ucData &= 0x0F; 
    if(ucAgcStep2 >= 0x00 && ucAgcStep2 <= 0x0F)
    {
       ucData |= (ucAgcStep2 << 4);
       vSpi1Write(RegAgcThresh2, ucData);
    }    
}

void vAgcStep3Init(uint8_t ucAgcStep3)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcThresh2);
    ucData &= 0xF0; 
    if(ucAgcStep3 >= 0x00 && ucAgcStep3 <= 0x0F)
    {
       ucData |= ucAgcStep3;
       vSpi1Write(RegAgcThresh2, ucData);
    }     
}

void vAgcStep4Init(uint8_t ucAgcStep4)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcThresh3);
    ucData &= 0x0F; 
    if(ucAgcStep4 >= 0x00 && ucAgcStep4 <= 0x0F)
    {
       ucData |= (ucAgcStep4 << 4);
       vSpi1Write(RegAgcThresh3, ucData);
    }    
}

void vAgcStep5Init(uint8_t ucAgcStep5)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegAgcThresh3);
    ucData &= 0xF0; 
    if(ucAgcStep5 >= 0x00 && ucAgcStep5 <= 0x0F)
    {
       ucData |= ucAgcStep5;
       vSpi1Write(RegAgcThresh3, ucData);
    }    
}

void vRegPllInit(uint8_t ucRegPll)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPll);
    ucData &= 0x3F; 
    if(ucRegPll >= 0u && ucRegPll <= 3u)
    {
       ucData |= (ucRegPll << 6);
       vSpi1Write(RegPll, ucData);
    }      
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Common config
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RELAY_OUTPUT_GPIO_Port, RELAY_OUTPUT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED_OUTPUT_GPIO_Port, LED_OUTPUT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : RELAY_OUTPUT_Pin SPI1_NSS_Pin */
    GPIO_InitStruct.Pin = RELAY_OUTPUT_Pin|SPI1_NSS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : BUTTON_INPUT_Pin */
    GPIO_InitStruct.Pin = BUTTON_INPUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON_INPUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : INTERRUPT_LORA_Pin */
    GPIO_InitStruct.Pin = INTERRUPT_LORA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(INTERRUPT_LORA_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED_OUTPUT_Pin */
    GPIO_InitStruct.Pin = LED_OUTPUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_OUTPUT_GPIO_Port, &GPIO_InitStruct);
}


