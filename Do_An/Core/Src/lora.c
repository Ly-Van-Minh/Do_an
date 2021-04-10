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
    uint8_t ucDataMatrix[2];
    ucAddress |= SPI1_WRITE; /* A wnr bit, which is 1 for write access and 0 for read access */
    ucDataMatrix[0] = ucAddress;
    ucDataMatrix[1] = ucData;
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (uint8_t *)ucDataMatrix, sizeof(ucDataMatrix), 100));
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
    ucAddress &= SPI1_READ; /* A wnr bit, which is 1 for write access and 0 for read access */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (uint8_t *)&ucAddress, sizeof(ucAddress), 100));
    ERROR_CHECK(HAL_SPI_Receive(&hspi1, (uint8_t *)&ucData, sizeof(ucData), 100));
    HAL_Delay(10);
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
    return ucData;
}

// /**
//   * @brief Read data of FIFO 
//   * @param ucData: Value needed into FIFO
//   * @retval Value of FIFO
//   */
// uint8_t ucReadFifo(void)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegFifo);
//     return ucData;
// }

// /**
//   * @brief Writre data into FIFO 
//   * @param ucData: Value needed write into FIFO
//   * @retval None
//   */
// void vWriteFifo(uint8_t ucData)
// {
//     vSpi1Write(RegFifo, ucData);
// }

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

/**
  * @brief Access Low Frequency Mode Initialization  
  * @param ucAccessSharedReg: Value of mode
  * @retval None
  */
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

/**
  * @brief Access Low Frequency Mode Initialization  
  * @param ucLowFrequencyModeOn: Value of mode
  * @retval None
  */
void vLowFrequencyModeOnInit(uint8_t ucLowFrequencyModeOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOpMode);    
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
    if (ucMode >= 0u && ucMode <= 7u)
    {
        ucData |= ucMode;
        vSpi1Write(RegOpMode, ucData);
    }
}

/**
  * @brief RF carrier frequency Initialization  
  * @param uiFrf: Value of RF carrier frequency
  * @retval None
  */
void vFrfInit(unsigned int uiFrf)
{
    vSpi1Write(RegFrfMsb, (uint8_t)(uiFrf >> 16));
    vSpi1Write(RegFrfMid, (uint8_t)(uiFrf >> 8));
    vSpi1Write(RegFrfLsb, (uint8_t)uiFrf);
}

/**
  * @brief Power Amplifier Initialization
  * @param ucPaSelect: Power Amplifier Value 
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

/**
  * @brief Max Output Power Initialization
  * @param ucMaxPower: Max Output Power Value 
  * @retval None
  */
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

/**
  * @brief Output Power Initialization
  * @param ucOutputPower: Output Power Value 
  * @retval None
  */
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

/**
  * @brief Rise/Fall time of ramp up/down in FSK Initialization
  * @param ucPaRamp: Ramp Value 
  * @retval None
  */
void vPaRampInit(uint8_t ucPaRamp)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaRamp);
    ucData &= 0xF0;
    if (ucPaRamp >= 0u && ucPaRamp <= 15u)
    {
        ucData |= ucPaRamp;
        vSpi1Write(RegPaRamp, ucData);
    }
}

/**
  * @brief Enable or Disable Overload Current Protection Initialization
  * @param ucOcpOn: OcpOn Value 
  * @retval None
  */
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
  * @param ucOcpTrim: OcpTrim Value
  * @retval None
  */
void vOcpTrimInit(uint8_t ucOcpTrim)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegOcp);
    ucData &= 0xE0;
    if (ucOcpTrim >= 0u && ucOcpTrim <= 31u)
    {
        ucData |= ucOcpTrim;
        vSpi1Write(RegOcp, ucData);
    }
}

/**
  * @brief  LNA Gain Initialization
  * @param ucLnaGain: Lna Gain Value
  * @retval None
  */
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

/**
  * @brief  Low Frequency (RFI_LF) LNA Current Adjustment Initialization
  * @param ucLnaBoostLf:  LNA Current Value
  * @retval None
  */
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

/**
  * @brief  High Frequency (RFI_LF) LNA Current Adjustment Initialization
  * @param ucLnaBoostHf:  LNA Current Value
  * @retval None
  */
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

// /**
//   * @brief Read Address Pointer in FIFO Data Buffer
//   * @param None
//   * @retval Address Pointer Value
//   */
// uint8_t ucFifoAddrPtrRead(void)
// {
//     uint8_t ucData = 0;
//     ucData = ucSpi1Read(RegFifoAddrPtr);
//     return ucData;
// }

// /**
//   * @brief Write Address Pointer in FIFO Data Buffer
//   * @param ucFifoAddrPtr: FIFO Address Pointer Value
//   * @retval None
//   */
// void vFifoAddrPtrWrite(uint8_t ucFifoAddrPtr)
// {
//     vSpi1Write(RegFifoAddrPtr, ucFifoAddrPtr);
// }

/**
  * @brief Base address in FIFO data buffer for TX modulator Initialization
  * @param ucFifoTxBaseAddr: Base Address Value for Tx
  * @retval None
  */
void vFifoTxBaseAddrInit(uint8_t ucFifoTxBaseAddr)
{
    vSpi1Write(RegFifoTxBaseAddr, ucFifoTxBaseAddr);
}

/**
  * @brief Base address in FIFO data buffer for RX modulator Initialization
  * @param ucFifoRxBaseAddr: Base Address Value for Rx
  * @retval None
  */
void vFifoRxBaseAddrInit(uint8_t ucFifoRxBaseAddr)
{
    vSpi1Write(RegFifoRxBaseAddr, ucFifoRxBaseAddr);
}

/**
  * @brief Read Start address (in data buffer) of last packet received
  * @param None
  * @retval Value of Start address (in data buffer) of last packet received
  */
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

/**
  * @brief Interrupt Mask Initialization
  * @param ucIrqFlagsMask: Flag Mask Value
  * @retval None
  */
void vIrqFlagsMaskInit(uint8_t ucIrqFlagsMask)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlagsMask);
    ucData |= ucIrqFlagsMask;
    vSpi1Write(RegIrqFlagsMask, ucData);
}

/**
  * @brief Read Interrupt Flags
  * @param None
  * @retval Status Interrupt Flags
  */
uint8_t ucIrqFlagsRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlags);
    return ucData;
}

/**
  * @brief Clear Interrupt Flags
  * @param ucIrqFlags: Value of Flags need clear
  * @retval None
  */
void vIrqFlagsClear(uint8_t ucIrqFlags)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegIrqFlags);
    ucData |= ucIrqFlags;
    vSpi1Write(RegIrqFlags, ucData);
}

/**
  * @brief Read Number of payload bytes of latest packet received
  * @param None
  * @retval Number of payload bytes
  */
uint8_t ucFifoRxBytesNbRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegRxNbBytes);
    return ucData;
}

/**
  * @brief Read Number of valid headers received 
  * @param None
  * @retval Number of valid headers received
  */
uint16_t usValidHeaderCntRead(void)
{
    uint16_t ucData = 0;
    ucData = ((uint16_t)ucSpi1Read(RegRxHeaderCntValueMsb) << 8);
    ucData |= (uint16_t)ucSpi1Read(RegRxHeaderCntValueLsb);
    return ucData;
}

/**
  * @brief Read Number of valid packets received 
  * @param None
  * @retval Number of valid packets received
  */
uint16_t usValidPacketCntRead(void)
{
    uint16_t ucData = 0;
    ucData = ((uint16_t)ucSpi1Read(RegRxPacketCntValueMsb) << 8);
    ucData |= (uint16_t)ucSpi1Read(RegRxPacketCntValueLsb);
    return ucData;
}

/**
  * @brief Read Coding rate of last header received
  * @param None
  * @retval Value Coding rate of last header received
  */
uint8_t ucRxCodingRateRead(void)
{
    uint8_t ucData;
    ucData = (ucSpi1Read(RegModemStat) >> 5);
    return ucData;
}

/**
  * @brief Read Modem Status
  * @param None
  * @retval Value of Modem Status
  */
uint8_t ucModemStatusRead(void)
{
    uint8_t ucData;
    ucData = 0x1F & ucSpi1Read(RegModemStat);
    return ucData;
}

/**
  * @brief Read Estimation of SNR on last packet received
  * @param None
  * @retval Value Estimation of SNR on last packet received
  */
uint8_t ucPacketSnrRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegPktSnrValue);
    return ucData;
}

/**
  * @brief Read RSSI of the latest packet received
  * @param None
  * @retval Value RSSI of the latest packet received
  */
uint8_t ucPacketRssiRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegPktRssiValue);
    return ucData;
}

/**
  * @brief Read Current RSSI value
  * @param None
  * @retval Current RSSI value
  */
uint8_t ucRssiRead(void)
{
    uint8_t ucData;
    ucData = ucSpi1Read(RegRssiValue);
    return ucData;
}

/**
  * @brief Read PLL failed to lock while attempting a TX/RX/CAD operation
  * @param None
  * @retval Status PLL 
  */
uint8_t ucPllTimeoutRead(void)
{
    uint8_t ucData;
    ucData = (ucSpi1Read(RegHopChannel) >> 7);
    return ucData;
}

/**
  * @brief Read CRC Information extracted from the received packet header
  * @param None
  * @retval Status CRC 
  */
uint8_t ucCrcOnPayloadread(void)
{
    uint8_t ucData;
    ucData = 0x01 & (ucSpi1Read(RegHopChannel) >> 6);
    return ucData;
}

/**
  * @brief Read Current value of frequency hopping channel
  * @param None
  * @retval Current value of frequency hopping channel 
  */
uint8_t ucFhssPresentChannelRead(void)
{
    uint8_t ucData;
    ucData = 0x3F & ucSpi1Read(RegHopChannel);
    return ucData;
}

/**
  * @brief Signal bandwidth Initialization
  * @param ucBandWidth: BandWidth Value
  * @retval None 
  */
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

/**
  * @brief Error coding rate Initialization
  * @param ucCodingRate: Error coding rate Value
  * @retval None 
  */
void vCodingRateInit(uint8_t ucCodingRate)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig1);
    ucData &= 0xF1;
    if (ucCodingRate >= 1u && ucCodingRate <= 4u)
    {
        ucData |= (ucCodingRate << 1);
        vSpi1Write(RegModemConfig1, ucData);
    }
}

/**
  * @brief Explicit or Implicit Header mode Initialization
  * @param ucHeaderMode: Error coding rate Value
  * @retval None 
  */
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

/**
  * @brief Spreading Factor rate Initialization
  * @param ucSpreadingFactor: Spreading Factor Value
  * @retval None 
  */
void vSpreadingFactorInit(uint8_t ucSpreadingFactor)
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

/**
  * @brief Continuous Mode Initialization
  * @param ucTxContinuousMode: Mode Value
  * @retval None 
  */
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

/**
  * @brief CRC generation and check on payload Initialization
  * @param ucRxPayloadCrcOn: CRCON Value
  * @retval None 
  */
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

/**
  * @brief RX Time-Out MSB Initialization
  * @param ucSymbTimeout: Symbols Time Value
  * @retval None 
  */
void vSymbTimeoutInit(uint16_t ucSymbTimeout)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig2);
    ucData &= 0xFC;
    ucData |= ((0x03) & (uint8_t)(ucSymbTimeout >> 8));
    vSpi1Write(RegModemConfig2, ucData);
    vSpi1Write(RegSymbTimeoutLsb, (uint8_t)ucSymbTimeout);
}

/**
  * @brief Preamble Length Initialization
  * @param ucPreambleLength: Preamble Length Value
  * @retval None
  */
void vPreambleLengthInit(uint16_t ucPreambleLength)
{
    vSpi1Write(RegPreambleMsb, (uint8_t)(ucPreambleLength >> 8));
    vSpi1Write(RegPreambleLsb, (uint8_t)(ucPreambleLength));
}

/**
  * @brief Payload Length Initialization
  * @param ucPayloadLength: Payload Length Value
  * @retval None
  */
void vPayloadLengthInit(uint8_t ucPayloadLength)
{
    vSpi1Write(RegPayloadLength, ucPayloadLength);
}

/**
  * @brief Payload Max Length Initialization
  * @param ucPayloadMaxLength: Payload Max Length Value
  * @retval None
  */
void vPayloadMaxLengthInit(uint8_t ucPayloadMaxLength)
{
    vSpi1Write(RegMaxPayloadLength, ucPayloadMaxLength);
}

/**
  * @brief Symbol periods between frequency hops Initialization
  * @param ucFreqHoppingPeriod: Payload Max Length Value
  * @retval None
  */
void vFreqHoppingPeriodInit(uint8_t ucFreqHoppingPeriod)
{
    vSpi1Write(RegHopPeriod, ucFreqHoppingPeriod);
}

/**
  * @brief Read Current value of RX databuffer pointer
  * @param None
  * @retval Address of last byte written by Lora receiver
  */
uint8_t ucFifoRxByteAddrPtr(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegFifoRxByteAddr);
    return ucData;
}

/**
  * @brief Low Data Rate Optimize Initialization
  * @param ucLowDataRateOptimize: Low Data Rate Optimize Value to Disable or Enable 
  * @retval None
  */
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

/**
  * @brief  Internal AGC Loop Initialization
  * @param ucAgcAutoOn: Value to Disable or Enable 
  * @retval None
  */
void vAgcAutoOnInit(uint8_t ucAgcAutoOn)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegModemConfig3);
    ucData &= 0xFB;
    if (ucAgcAutoOn == 0u || ucAgcAutoOn == 1u)
    {
        ucData |= (ucAgcAutoOn << 2);
        vSpi1Write(RegModemConfig3, ucData);
    }
}

/**
  * @brief Read Estimated frequency error from modem
  * @param None
  * @retval  RF Frequency Error Value
  */
unsigned int uiFreqError(void)
{
    unsigned int uiData = 0;
    uiData |= (((unsigned int)(ucSpi1Read(RegFeiMsb) & 0x0F) << 16) | \
               ((unsigned int)ucSpi1Read(RegFeiMid) << 8) | \
               (unsigned int)ucSpi1Read(RegFeiLsb));
    return uiData;
}

/**
  * @brief Read Wideband RSSI measurement
  * @param None
  * @retval  Wideband RSSI measurement Value
  */
uint8_t ucRssiWidebandInit(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegRssiWideband);
    return ucData;
}

/**
  * @brief  LoRa Detection Optimize Initialization
  * @param ucDetectionOptimize: Detection Optimize Value 
  * @retval None
  */
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

/**
  * @brief  Invert the LoRa I and Q signals Initialization
  * @param ucInvertIQ: Invert I and Q signals Value 
  * @retval None
  */
void vInvertIQInit(uint8_t ucInvertIQ)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegInvertIQ);
    ucData &= 0xBF;
    if (ucInvertIQ == 0u || ucInvertIQ == 1u)
    {
        ucData |= (ucInvertIQ << 6);
        vSpi1Write(RegInvertIQ, ucData);
    }
}

/**
  * @brief LoRa detection threshold Initialization
  * @param ucDetectionThreshold: LoRa detection threshold Value 
  * @retval None
  */
void vDetectionThresholdInit(uint8_t ucDetectionThreshold)
{
    if (ucDetectionThreshold == 0x0A || ucDetectionThreshold == 0x0C)
    {
        vSpi1Write(RegDetectionThreshold, ucDetectionThreshold);
    }
}

/**
  * @brief LoRa Sync Word Initialization
  * @param ucSyncWord: Sync Word Value, Value 0x34 is reserved for LoRaWAN networks 
  * @retval None
  */
void vSyncWordInit(uint8_t ucSyncWord)
{
    vSpi1Write(RegSyncWord, ucSyncWord);
}

/**
  * @brief Mapping of pins DIO0 Initialization
  * @param ucDio0Mapping: Dio0 Mapping Value
  * @retval None
  */
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

/**
  * @brief Mapping of pins DIO1 Initialization
  * @param ucDio1Mapping: DIO1 Mapping Value
  * @retval None
  */
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

/**
  * @brief Mapping of pins DIO2 Initialization
  * @param ucDio2Mapping: DIO2 Mapping Value
  * @retval None
  */
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

/**
  * @brief Mapping of pins DIO3 Initialization
  * @param ucDio3Mapping: DIO3 Mapping Value
  * @retval None
  */
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

/**
  * @brief Mapping of pins DIO4 Initialization
  * @param ucDio4Mapping: DIO4 Mapping Value
  * @retval None
  */
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

/**
  * @brief Mapping of pins DIO5 Initialization
  * @param ucDio4Mapping: DIO5 Mapping Value
  * @retval None
  */
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

/**
  * @brief Allows the mapping of either Rssi Or PreambleDetect to the DIO
  *          pins, Initialization
  * @param ucMapPreambleDetect: Map Preamble Detect Value
  * @retval None
  */
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

/**
  * @brief Read Version code of the chip
  * @param None
  * @retval  Version code of the chip Value
  */
uint8_t ucVersionRead(void)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegVersion);
    return ucData;
}

/**
  * @brief Controls the crystal oscillator Initialization
  * @param ucTcxoInputOn: Tcxo Input On Value
  * @retval None
  */
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

/**
  * @brief Enables the +20dBm option on PA_BOOST pin Initialization
  * @param ucPaDac: Pa Dac Value
  * @retval None
  */
void vPaDacInit(uint8_t ucPaDac)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPaDac);
    ucData &= 0xF8;
    if (ucPaDac == 4u || ucPaDac == 7u)
    {
        ucData |= ucPaDac;
        vSpi1Write(RegPaDac, ucData);
    }
}

/**
  * @brief Temperature saved during the latest IQ (RSSI and Image)
  *           calibrationInitialization
  * @param ucFormerTemp: Temperature Value
  * @retval None
  */
void vFormerTempInit(uint8_t ucFormerTemp)
{
    vSpi1Write(RegFormerTemp, ucFormerTemp);
}

/**
  * @brief Sets the floor reference for all AGC thresholds Initialization
  * @param ucAgcReferenceLevel: Agc Reference Level Value
  * @retval None
  */
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

/**
  * @brief Defines the 1st AGC Threshold Initialization
  * @param ucAgcStep1: Agc Step1 Value
  * @retval None
  */
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

/**
  * @brief Defines the 2st AGC Threshold Initialization
  * @param ucAgcStep2: Agc Step2 Value
  * @retval None
  */
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

/**
  * @brief Defines the 3st AGC Threshold Initialization
  * @param ucAgcStep3: Agc Step3 Value
  * @retval None
  */
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

/**
  * @brief Defines the 4st AGC Threshold Initialization
  * @param ucAgcStep4: Agc Step4 Value
  * @retval None
  */
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

/**
  * @brief Defines the 5st AGC Threshold Initialization
  * @param ucAgcStep5: Agc Step5 Value
  * @retval None
  */
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

/**
  * @brief Controls the PLL bandwidth Initialization
  * @param ucPllBandwidth: Pll Bandwidth Value
  * @retval None
  */
void vPllBandwidth(uint8_t ucPllBandwidth)
{
    uint8_t ucData = 0;
    ucData = ucSpi1Read(RegPll);
    ucData &= 0x3F;
    if (ucPllBandwidth >= 0u && ucPllBandwidth <= 3u)
    {
        ucData |= (ucPllBandwidth << 6);
        vSpi1Write(RegPll, ucData);
    }
}

/**
  * @brief Lora Module Initialization
  * @param: None
  * @retval: None
  */
void vLoraInit(void)
{
    uint8_t ucData = 0;
    vModeInit(SLEEP_MODE);          /* Init Module Lora into Sleep Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vLongRangeModeInit(LORA_MODE);  /* Init Module Lora into Lora TM Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vModeInit(STDBY_MODE);          /* Init Module Lora into Standby Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vAccessSharedRegInit(ACCESS_LORA_REGISTERS);    /* Access LoRa registers page 0x0D: 0x3F */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vLowFrequencyModeOnInit(ACCESS_LOW_FREQUENCY_MODE);     /* Access Low Frequency Mode registers */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vFrfInit(RF_FREQUENCY);         /* Init RF carrier frequency */
    ucData = ucSpi1Read(RegFrfMsb);
    printf("RegFrfMsb = 0x%XH\r\n", ucData);

    ucData = ucSpi1Read(RegFrfMid);
    printf("RegFrfMid = 0x%XH\r\n", ucData);

    ucData = ucSpi1Read(RegFrfLsb);
    printf("RegFrfLsb = 0x%XH\r\n", ucData);

    vPaSelectInit(PA_BOOST);        /* Output power is limited to +20 dBm */
    ucData = ucSpi1Read(RegPaConfig);
    printf("RegPaConfig = 0x%XH\r\n", ucData);

    vMaxPowerInit(MAX_POWER);
    ucData = ucSpi1Read(RegPaConfig);
    printf("RegPaConfig = 0x%XH\r\n", ucData); 

    vOutputPowerInit(OUTPUT_POWER); /* Pout=17-(15-OutputPower) */
    ucData = ucSpi1Read(RegPaConfig);
    printf("RegPaConfig = 0x%XH\r\n", ucData);   

    vPaRampInit(PA_RAMP);
    ucData = ucSpi1Read(RegPaRamp);
    printf("RegPaRamp = 0x%XH\r\n", ucData);

    vOcpOnInit(OCP_ON);             /* OCP enabled */
    ucData = ucSpi1Read(RegOcp);
    printf("RegOcp = 0x%XH\r\n", ucData);

    vOcpTrimInit(OCP_TRIM);         /* Trimming of OCP current: Imax = 240mA */
    ucData = ucSpi1Read(RegOcp);
    printf("RegOcp = 0x%XH\r\n", ucData);

    vLnaGainInit(G1);               /* LNA gain setting: G1 = maximum gain */
    ucData = ucSpi1Read(RegLna);
    printf("RegLna = 0x%XH\r\n", ucData);

    vLnaBoostLfInit(LNA_BOOST_LF);  /* Low Frequency (RFI_LF) LNA current 
                                       adjustment Default LNA current */
    ucData = ucSpi1Read(RegLna);
    printf("RegLna = 0x%XH\r\n", ucData);

    vLnaBoostHfInit(LNA_BOOST_HF);  /* High Frequency (RFI_HF) LNA current 
                                       adjustment Boost on, 150% LNA current */
    ucData = ucSpi1Read(RegLna);
    printf("RegLna = 0x%XH\r\n", ucData);

    vFifoTxBaseAddrInit(FIFO_TX_BASE_ADDR); /* Write base address in FIFO data 
                                               buffer for TX modulator */
    ucData = ucSpi1Read(RegFifoTxBaseAddr);
    printf("RegFifoTxBaseAddr = 0x%XH\r\n", ucData);

    vFifoRxBaseAddrInit(FIFO_RX_BASE_ADDR); /* Read base address in FIFO data 
                                               buffer for RX demodulator */
    ucData = ucSpi1Read(RegFifoRxBaseAddr);
    printf("RegFifoRxBaseAddr = 0x%XH\r\n", ucData);

    vIrqFlagsMaskInit(IRQ_FLAGS_MASK);  /* Disable all interrupts mask */
    ucData = ucSpi1Read(RegIrqFlagsMask);
    printf("RegIrqFlagsMask = 0x%XH\r\n", ucData);

    vBandWidthInit(BANDWIDTH_125K);     /* Signal bandwidth: 250kHz */
    ucData = ucSpi1Read(RegModemConfig1);
    printf("RegModemConfig1 = 0x%XH\r\n", ucData);

    vCodingRateInit(CODING_RATE_4_5);   /* Error coding rate 4/5 */
    ucData = ucSpi1Read(RegModemConfig1);
    printf("RegModemConfig1 = 0x%XH\r\n", ucData);

    vImplicitHeaderModeOnInit(IMPLICIT_HEADER); /* Init Implicit Header mode */
    ucData = ucSpi1Read(RegModemConfig1);
    printf("RegModemConfig1 = 0x%XH\r\n", ucData);

    vSpreadingFactorInit(SPREADING_FACTOR_64); /* SF rate 64 chips / symbol */
    ucData = ucSpi1Read(RegModemConfig2);
    printf("RegModemConfig2 = 0x%XH\r\n", ucData);

    vTxContinuousModeInit(TX_NORMAL_MODE);  /* Normal mode, a single packet is sent */
    ucData = ucSpi1Read(RegModemConfig2);
    printf("RegModemConfig2 = 0x%XH\r\n", ucData);

    vRxPayloadCrcOnInit(CRC_ENABLE);        /* Enable CRC generation and check on payload */
    ucData = ucSpi1Read(RegModemConfig2);
    printf("RegModemConfig2 = 0x%XH\r\n", ucData);

    vSymbTimeoutInit(RX_TIMEOUT);           /* RX operation time-out */
    ucData = ucSpi1Read(RegModemConfig2);
    printf("RegModemConfig2 = 0x%XH\r\n", ucData);
    ucData = ucSpi1Read(RegSymbTimeoutLsb);
    printf("RegSymbTimeoutLsb = 0x%XH\r\n", ucData);

    vPreambleLengthInit(PREAMBLE_LENGTH);   /* Preamble length  = PreambleLength + 4.25 Symbols */
    ucData = ucSpi1Read(RegPreambleMsb);
    printf("RegPreambleMsb = 0x%XH\r\n", ucData);
    ucData = ucSpi1Read(RegPreambleLsb);
    printf("RegPreambleLsb = 0x%XH\r\n", ucData);

    vPayloadLengthInit(PAYLOAD_LENGHT);     /* Init Payload length */
    ucData = ucSpi1Read(RegPayloadLength);
    printf("RegPayloadLength = 0x%XH\r\n", ucData);

    vPayloadMaxLengthInit(PAYLOAD_MAX_LENGTH);  /* Maximum payload length */
    ucData = ucSpi1Read(RegMaxPayloadLength);
    printf("RegMaxPayloadLength = 0x%XH\r\n", ucData); 
    
    vFreqHoppingPeriodInit(FREQ_HOPPING_PERIOD);    /* Symbol periods between frequency hops */
    ucData = ucSpi1Read(RegHopPeriod);  
    printf("RegHopPeriod = 0x%XH\r\n", ucData);
    
    vLowDataRateOptimizeInit(LOW_DATA_RATE_OPTIMIZE); /*  Enabled; mandated for when the 
                                                    symbol length exceeds16ms */
    ucData = ucSpi1Read(RegModemConfig3);
    printf("RegModemConfig3 = 0x%XH\r\n", ucData);

    vAgcAutoOnInit(AGC_AUTO) ;   /* 0 -> LNA gain set by register LnaGain
                                    1 -> LNA gain set by the internal AGC loop*/
    ucData = ucSpi1Read(RegModemConfig3);
    printf("RegModemConfig3 = 0x%XH\r\n", ucData);

    vDetectionOptimizeInit(LORA_DETECTION_OPTIMIZE);   /* LoRa Detection Optimize 0x03 -> 
                                                       SF7 to SF12; 0x05 -> SF6 */
    ucData = ucSpi1Read(RegDetectOptimize);
    printf("RegDetectOptimize = 0x%XH\r\n", ucData);

    vInvertIQInit(INVERT_IQ);   /* Invert the LoRa I and Q signals */
    ucData = ucSpi1Read(RegInvertIQ);
    printf("RegInvertIQ = 0x%XH\r\n", ucData);

    vDetectionThresholdInit(LORA_DETECTION_THRESHOLD); /* LoRa detection threshold 0x0A 
                                                            -> SF7 to SF12; 0x0C -> SF6 */
    ucData = ucSpi1Read(RegDetectionThreshold);
    printf("RegDetectionThreshold = 0x%XH\r\n", ucData);

    vSyncWordInit(LORA_SYNC_WORD);          /* Init Sync Word */
    ucData = ucSpi1Read(RegSyncWord);
    printf("RegSyncWord = 0x%XH\r\n", ucData);

    vAgcReferenceLevelInit(AGC_REFERENCE);  /* Sets the floor reference for all AGC thresholds */
    ucData = ucSpi1Read(RegAgcRef);
    printf("RegAgcRef = 0x%XH\r\n", ucData);

    vAgcStep1Init(AGC_STEP1);   /* Defines the 1st AGC Threshold */
    ucData = ucSpi1Read(RegAgcThresh1);
    printf("RegAgcThresh1 = 0x%XH\r\n", ucData);

    vAgcStep2Init(AGC_STEP2);   /* Defines the 2nd AGC Threshold */
    ucData = ucSpi1Read(RegAgcThresh2);
    printf("RegAgcThresh2 = 0x%XH\r\n", ucData);

    vAgcStep3Init(AGC_STEP3);   /* Defines the 3rd AGC Threshold */
    ucData = ucSpi1Read(RegAgcThresh2);
    printf("RegAgcThresh2 = 0x%XH\r\n", ucData);

    vAgcStep4Init(AGC_STEP4);   /* Defines the 4th AGC Threshold */
    ucData = ucSpi1Read(RegAgcThresh3);
    printf("RegAgcThresh3 = 0x%XH\r\n", ucData);

    vAgcStep5Init(AGC_STEP5);   /* Defines the 5th AGC Threshold */
    ucData = ucSpi1Read(RegAgcThresh3);
    printf("RegAgcThresh3 = 0x%XH\r\n", ucData);

    vPllBandwidth(PLL_BANDWIDTH);   /* Controls the PLL bandwidth */
    ucData = ucSpi1Read(RegPll);
    printf("RegPll = 0x%XH\r\n", ucData);

    vDio0MappingInit(CAD_DONE);
    ucData = ucSpi1Read(RegDioMapping1);
    printf("RegDioMapping1 = 0x%XH\r\n", ucData);
    // vMapPreambleDetect(PREAMBBLE_DETECT_INTERRUPT);
    // ucData = ucSpi1Read(RegTcxo);
    // printf("RegTcxo = 0x%XH\r\n", ucData);

    vTcxoInputOnInit(TCXO_INPUT);   /* Controls the crystal oscillator */
    ucData = ucSpi1Read(RegTcxo);
    printf("RegTcxo = 0x%XH\r\n", ucData);

    vPaDacInit(PA_DAC); /* Enables the +20dBm option on PA_BOOST pin */
    ucData = ucSpi1Read(RegPaDac);
    printf("RegPaDac = 0x%XH\r\n", ucData);
    
    ucData = ucSpi1Read(RegLna);
    printf("RegLna = 0x%XH\r\n", ucData);

    ucData = ucVersionRead();
    printf("RegVersion = 0x%XH\r\n", ucData);

    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);
}

/**
  * @brief Transmit Data into FIFO
  * @param pcTxBuffer: Data Address Transmit
  * @param ucTxMode: Transmit Mode: Tx Single or Rx Coninuous
  * @retval None
  */
void vLoraTransmit(uint8_t* pcTxBuffer, uint8_t ucTxMode)
{
    uint8_t ucData = 0;
    uint8_t i = 0;
    uint8_t ucIrqStatus = 0;

    printf("****************** Start Transmit ********************\r\n\r\n");

    vModeInit(STDBY_MODE);          /* Init Module Lora into Standby Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf(" Init Standby Mode: RegOpMode = 0x%XH\r\n", ucData);

    vSpi1Write(RegFifoAddrPtr , FIFO_TX_BASE_ADDR); /* Set FifoPtrAddr to FifoTxPtrBase */
    ucData = ucSpi1Read(RegFifoAddrPtr);
    printf("Set Write Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);
    if(ucTxMode == TX_CONTINOUS)    /* If Tx Coninuous Mode */
    {
        while (1)
        {
            for(i = 0u; i < PAYLOAD_LENGHT; i++)
            {
                ucData = *(pcTxBuffer + i);
                vSpi1Write(RegFifo, *(pcTxBuffer + i));
                // ucData = ucSpi1Read(RegFifo);
                // printf("Write Data into FiFo: RegFifo = 0x%XH\r\n", ucData);
            }

            vModeInit(TX_MODE); /* Init Module Lora into TX Mode */
            ucData = ucSpi1Read(RegOpMode);
            printf("Init Tx Mode: RegOpMode = 0x%XH\r\n", ucData);

            ucIrqStatus = ucSpi1Read(RegIrqFlags);
            printf("Check TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

            while((ucIrqStatus & 0x08u) == 0u);  /* Wait for TxDone set */

            ucData = ucSpi1Read(RegOpMode);
            printf("Check Automatic Mode change STANDBY: RegOpMode = 0x%XH\r\n", ucData);

            if((ucData & 0x01u) != 0u)  /* If Automatic Mode change STANDBY */
            {
                printf("Automatic Mode change STANDBY from TX Continuous Mode\r\n");
            }
            else /* Automatic Mode change STANDBY */
            {
                printf("Automatic Mode change STANDBY fail from TX Continuous Mode\r\n");
            }

            vSpi1Write(RegFifoAddrPtr , FIFO_TX_BASE_ADDR); /* Set FifoPtrAddr to FifoTxPtrBase */
            ucData = ucSpi1Read(RegFifoAddrPtr);
            printf("Set Write Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);

            ucData = ucSpi1Read(RegFifo);
            printf("Write Data into FiFo: RegFifo = 0x%XH\r\n", ucData);
            ucData = ucSpi1Read(RegFifo);
            printf("Write Data into FiFo: RegFifo = 0x%XH\r\n", ucData);
            ucData = ucSpi1Read(RegFifo);
            printf("Write Data into FiFo: RegFifo = 0x%XH\r\n", ucData);

            ucData = ucSpi1Read(RegIrqFlags);
            printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

            vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x08u));  /* Clear TxDone Flag */
            ucData = ucSpi1Read(RegIrqFlags);
            printf("Clear TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);
        }
    }    
    else    /* If Tx Single Mode */
    {
        for(i = 0u; i < PAYLOAD_LENGHT; i++)
        {
            ucData = *(pcTxBuffer + i);
            vSpi1Write(RegFifo, *(pcTxBuffer + i));
            ucData = ucSpi1Read(RegFifo);
            printf("Write Data into FiFo: RegFifo = 0x%XH\r\n", ucData);
        }
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Check PayloadCrcError Flag Before Transmit: RegIrqFlags = 0x%XH\r\n", ucData);

        vModeInit(TX_MODE); /* Init Module Lora into TX Mode */
        ucData = ucSpi1Read(RegOpMode);
        printf("Init Tx Mode: RegOpMode = 0x%XH\r\n", ucData);

        ucIrqStatus = ucSpi1Read(RegIrqFlags);
        printf("Check TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

        while((ucIrqStatus & 0x08u) == 0u);  /* Wait for TxDone set */

        ucData = ucSpi1Read(RegOpMode);
        printf("Check Automatic Mode change STANDBY: RegOpMode = 0x%XH\r\n", ucData);

        if((ucData & 0x01u) == 0u)  /* Automatic Mode change STANDBY */
        {
            printf("Automatic Mode change STANDBY fail from TX Single Mode\r\n");
        }
        else /* Automatic Mode change STANDBY fail */
        {
            printf("Automatic Mode change STANDBY from TX Single Mode\r\n");
        }
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

        vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x08u));  /* Clear TxDone Flag */
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Clear TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);

        vModeInit(SLEEP_MODE);          /* Init Module Lora into Sleep Mode */
        ucData = ucSpi1Read(RegOpMode);
        printf("Check Sleep Mode: RegOpMode = 0x%XH\r\n", ucData);
        printf("Module switch to Sleep Mode\r\n");
    }

    printf("****************** Finish Transmit ********************\r\n\r\n");
}

/**
  * @brief Receive Data in FIFO
  * @param pcRxBuffer: Data Address Receive
  * @param ucRxMode: Receive Mode: Rx Single or Continuous
  * @retval None
  */
void vLoraReceive(uint8_t* pcRxBuffer, uint8_t ucRxMode)
{
    uint8_t ucData = 0;
    uint8_t i = 0;
    uint8_t ucIrqStatus = 0;
    printf("****************** Start Receive ********************\r\n\r\n");
    vModeInit(STDBY_MODE);          /* Init Module Lora into Standby Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf("RegOpMode = 0x%XH\r\n", ucData);

    vSpi1Write(RegFifoAddrPtr , FIFO_RX_BASE_ADDR); /* Set FifoPtrAddr to FifoRxPtrBase */
    ucData = ucSpi1Read(RegFifoAddrPtr);
    printf("Set Read Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);

    if(ucRxMode == RX_CONTINUOUS)   /* If Rx Continuous Mode */
    {
        vModeInit(RXCONTINUOUS_MODE); /* Init Module Lora into RX Coninuous Mode */
        ucData = ucSpi1Read(RegOpMode);
        printf("Init Rx Continuous Mode: RegOpMode = 0x%XH\r\n", ucData);

        while(1)
        {
            ucIrqStatus = ucSpi1Read(RegIrqFlags);
            printf("Check RxDone Flags: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);
            while((ucIrqStatus & 0x40u) == 0u);  /* Wait for RxDone set */

            ucIrqStatus = ucSpi1Read(RegIrqFlags);
            printf("Check PayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);
            
            if((ucIrqStatus & 0x20u) == 0)  /* If PayloadCrcError Flag not set */
            {
                for(i = 0u; i < PAYLOAD_LENGHT; i++)
                {
                    *(pcRxBuffer + i) = ucSpi1Read(RegFifo);
                    ucData = ucSpi1Read(RegFifo);
                    printf("Read Data From FIFO: RegFifo = 0x%XH\r\n", ucData); 
                }

            }
            else    /* If PayloadCrcError Flag set */
            {
                ucData = ucSpi1Read(RegIrqFlags);
                printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);                

                vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x20u));  /* Clear PayloadCrcError Flag */
                ucData = ucSpi1Read(RegIrqFlags);
                printf("Clear PayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucData);
            }
            printf("****************** Finish Receive ********************\r\n\r\n");
        }
    }
    else    /* If Rx Single Module */
    {
        vModeInit(RXSINGLE_MODE); /* Init Module Lora into RX Single Mode */
        ucData = ucSpi1Read(RegOpMode);
        printf("Init Rx Single Mode: RegOpMode = 0x%XH\r\n", ucData);

        while(((ucIrqStatus & 0x40u) | (ucIrqStatus & 0x80)) == 0u);  /* Wait for RxTimeout or RxDone Flag set */
        ucIrqStatus = ucSpi1Read(RegIrqFlags);
        printf("Check RxTimeout or RxDone: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);
        if((ucIrqStatus & 0x40u) != 0u) /* If RxDone Flag set */
        {
            ucData = ucSpi1Read(RegOpMode);
            printf("Check Automatic Mode change STANDBY: RegOpMode = 0x%XH\r\n", ucData);
            if((ucData & 0x01u) != 0u)  /* If Automatic Mode change STANDBY */
            {
                printf("Automatic Mode change STANBY from RX Single Mode\r\n");

                ucIrqStatus = ucSpi1Read(RegIrqFlags);
                printf("Check PlayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

                if((ucIrqStatus & 0x20) == 0u)  /* If PlayloadCrcError Flag not set */
                {
                    for(i = 0u; i < PAYLOAD_LENGHT; i++)
                    {
                        *(pcRxBuffer + i) = ucSpi1Read(RegFifo);
                        ucData = ucSpi1Read(RegFifo);
                        printf("Read Data From FIFO: RegFifo = 0x%XH\r\n", ucData);
                    }
                    ucData = ucSpi1Read(RegIrqFlags);
                    printf("Check RxDone Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

                    vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x40u));  /* Clear RxDone Flag */
                    ucData = ucSpi1Read(RegIrqFlags);
                    printf("Clear RxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);
                }
                else    /* If PlayloadCrcError Flag set  */
                {
                    ucIrqStatus = ucSpi1Read(RegIrqFlags);

                    ucData = ucSpi1Read(RegIrqFlags);
                    printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

                    vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x20u));  /* Clear PayloadCrcError Flag */
                    ucData = ucSpi1Read(RegIrqFlags);
                    printf("Clear PayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucData);
                }
                vModeInit(SLEEP_MODE); /* Init Module Lora into Sleep Mode */
                ucData = ucSpi1Read(RegOpMode);
                printf("Init Sleep Mode: RegOpMode = 0x%XH\r\n", ucData);
            }
            else    /* If Automatic Mode chang STANBY fail */
            {
                printf("Automatic Mode change STANBY fail from RX Single Mode\r\n");
            }
        }
        else    /* If Rxtimeout Flag set */
        {
            ucData = ucSpi1Read(RegOpMode);
            printf("Check RxTimeout: RegOpMode = 0x%XH\r\n", ucData);
            if((ucData & 0x01u) != 0u)
            {
                printf("Automatic Mode change STANBY from RX Single Mode\r\n");

                ucData = ucSpi1Read(RegIrqFlags);
                printf("Check Timeout Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

                vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x80u));  /* Clear Timeout Flag */
                ucData = ucSpi1Read(RegIrqFlags);
                printf("Clear Timeout Flag: RegIrqFlags = 0x%XH\r\n", ucData);
            }
            else    /* If Automatic Mode chang STANBY fail */
            {
                printf("Automatic Mode change STANBY fail from RX Single Mode\r\n");
            }
        }
    }
    printf("****************** Finish Receive ********************\r\n\r\n");
}