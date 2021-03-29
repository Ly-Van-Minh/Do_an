/*
 * lora.c
 *
 *  Created on: Mar 16, 2021
 *      Author: Ly Van Minh
 */
#include "lora.h"
#include "main.h"
#include "string.h"
#include "stm_log.h"

/* Variables */
const char *LORA_TAG = "LORA_TAG";
extern SPI_HandleTypeDef hspi1;

/**
  * @brief Write Data Function To Lora Module
  * @param ucAddress: Address registers or fifo of Lora
  * @param ucData: Data need write into registers or fifo of Lora
  * @retval None
  */

void vSpi1Write(uint8_t ucAddress, uint8_t ucData)
{
    ucAddress |= 0x80; /* A wnr bit, which is 1 for write access and 0 for read access */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucAddress, sizeof(ucAddress), 100));
    ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucData, sizeof(ucData), 100));
    HAL_Delay(10);
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
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
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucAddress, sizeof(ucAddress), 100));
    ERROR_CHECK(HAL_SPI_Receive(&hspi1, (uint8_t *)&ucData, sizeof(ucData), 100));
    HAL_Delay(10);
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
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
    if (ucLongRangeMode == 0u || ucLongRangeMode == 1u)
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
    if (ucAccessSharedReg == 0u || ucAccessSharedReg == 1u)
    {
        ucData |= (ucAccessSharedReg << 6);
        vSpi1Write(RegOpMode, ucData);
    }
}

void vLowFrequencyModeOnInit(uint8_t ucLowFrequencyModeOn)
{
    uint8_t ucData = 0;
    ucData &= 0xF7;
    if (ucLowFrequencyModeOn == 0u || ucLowFrequencyModeOn == 1u)
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
    if (ucMode >= 0 && ucMode <= 7)
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
    if (ucPaSelect == 0u || ucPaSelect == 1u)
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
    if (ucMaxPower >= 0u && ucMaxPower <= 7u)
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
    if (ucOutputPower >= 0u && ucOutputPower <= 15u)
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
    if (ucPaRamp >= 0u && ucPaRamp <= 15u)
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
    if (ucOcpOn == 0u || ucOcpOn == 1u)
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
    if (ucOcpTrim == 0u || ucOcpTrim == 32u)
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
    if (ucLnaGain >= 1u && ucLnaGain <= 6u)
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
    if (ucLnaBoostLf == 0u)
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
    if (ucLnaBoostHf == 0u || ucLnaBoostHf == 3u)
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
    if (ucBandWidth >= 0u && ucBandWidth <= 9u)
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
    if (ucCodingRate >= 0u && ucCodingRate <= 4u)
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
    if (ucHeaderMode == 0u || ucHeaderMode == 1u)
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
    if (ucSpreadingFactor >= 6u && ucSpreadingFactor <= 12u)
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
    if (ucTxContinuousMode == 0u || ucTxContinuousMode == 1u)
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
    if (ucRxPayloadCrcOn == 0u || ucRxPayloadCrcOn == 1u)
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
    if (ucLowDataRateOptimize == 0u || ucLowDataRateOptimize == 1u)
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
    if (ucAgcAutoOn == 0u || ucAgcAutoOn == 1u)
    {
        ucData |= (ucAgcAutoOn << 2);
        vSpi1Write(RegModemConfig3, ucData);
    }
}

unsigned int uiFreqError(void)
{
    unsigned int uiData = 0;
    uiData |= (((unsigned int)(ucSpi1Read(RegFeiMsb) & 0x0F) << 16) |
               ((unsigned int)ucSpi1Read(RegFeiMid) << 8) |
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
    if (ucDetectionOptimize == 3u || ucDetectionOptimize == 5u)
    {
        ucData |= ucDetectionOptimize;
        vSpi1Write(RegDetectOptimize, ucData);
    }
}

void vInvertIQInit(uint8_t ucInvertIQ)
{
    uint8_t ucData = 0;
    ucData &= 0xBF;
    if (ucInvertIQ == 0u || ucInvertIQ == 1u)
    {
        ucData |= (ucInvertIQ << 6);
        vSpi1Write(RegInvertIQ, ucData);
    }
}

void vDetectionThresholdInit(uint8_t ucDetectionThreshold)
{
    if (ucDetectionThreshold == 0x0A || ucDetectionThreshold == 0x0C)
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
    if (ucDio0Mapping >= 0u && ucDio0Mapping <= 3u)
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
    if (ucDio1Mapping >= 0u && ucDio1Mapping <= 3u)
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
    if (ucDio2Mapping >= 0u && ucDio2Mapping <= 3u)
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
    if (ucDio3Mapping >= 0u && ucDio3Mapping <= 3u)
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
    if (ucDio4Mapping >= 0u && ucDio4Mapping <= 3u)
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
    if (ucDio5Mapping >= 0u && ucDio5Mapping <= 3u)
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
    if (ucMapPreambleDetect == 0u || ucMapPreambleDetect == 1u)
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
    if (ucTcxoInputOn == 0u || ucTcxoInputOn == 1u)
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
    if (ucPaDac >= 0u && ucPaDac <= 3u)
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
    if (ucAgcReferenceLevel >= 0x00 && ucAgcReferenceLevel <= 0x3F)
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
    if (ucAgcStep1 >= 0x00 && ucAgcStep1 <= 0x1F)
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
    if (ucAgcStep2 >= 0x00 && ucAgcStep2 <= 0x0F)
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
    if (ucAgcStep3 >= 0x00 && ucAgcStep3 <= 0x0F)
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
    if (ucAgcStep4 >= 0x00 && ucAgcStep4 <= 0x0F)
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
    if (ucAgcStep5 >= 0x00 && ucAgcStep5 <= 0x0F)
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
    if (ucRegPll >= 0u && ucRegPll <= 3u)
    {
        ucData |= (ucRegPll << 6);
        vSpi1Write(RegPll, ucData);
    }
}
