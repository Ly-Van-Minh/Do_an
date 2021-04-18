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

void vSpi1Write(u8 ucAddress, u8 ucData)
{
  u8 ucDataMatrix[2];
  ucAddress |= SPI1_WRITE; /* A wnr bit, which is 1 for write access and 0 for read access */
  ucDataMatrix[0] = ucAddress;
  ucDataMatrix[1] = ucData;
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_Delay(DELAY_SPI);
  ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (u8 *)ucDataMatrix, sizeof(ucDataMatrix), 100));
  HAL_Delay(DELAY_SPI);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

/**
  * @brief Read Data Function from Lora Module
  * @param ucAddress: Address registers or fifo of Lora Module
  * @retval ucData: Data contained in registers or fifo of Lora Module  
  */
u8 ucSpi1Read(u8 ucAddress)
{
  u8 ucData = 0;
  ucAddress &= SPI1_READ; /* A wnr bit, which is 1 for write access and 0 for read access */
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_Delay(DELAY_SPI);
  ERROR_CHECK(HAL_SPI_Transmit(&hspi1, (u8 *)&ucAddress, sizeof(ucAddress), 100));
  ERROR_CHECK(HAL_SPI_Receive(&hspi1, (u8 *)&ucData, sizeof(ucData), 100));
  HAL_Delay(DELAY_SPI);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  return ucData;
}

// /**
//   * @brief Read data of FIFO
//   * @param ucData: Value needed into FIFO
//   * @retval Value of FIFO
//   */
// u8 ucReadFifo(void)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegFifo);
//     return ucData;
// }

// /**
//   * @brief Writre data into FIFO
//   * @param ucData: Value needed write into FIFO
//   * @retval None
//   */
// void vWriteFifo(u8 ucData)
// {
//     vSpi1Write(RegFifo, ucData);
// }

/**
  * @brief Mode Initialization LoraTM or FSK/OOK 
  * @param ucMode: Value of mode: LoraTM if 1, FSK/OOK if 0
  * @retval None
  */
void vLongRangeModeInit(u8 ucLongRangeMode)
{
  vModeInit(SLEEP_MODE);
  u8 ucData = 0;
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
void vAccessSharedRegInit(u8 ucAccessSharedReg)
{
  u8 ucData = 0;
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
void vLowFrequencyModeOnInit(u8 ucLowFrequencyModeOn)
{
  u8 ucData = 0;
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
void vModeInit(u8 ucMode)
{
  u8 ucData = 0;
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
  vSpi1Write(RegFrfMsb, (u8)(uiFrf >> 16));
  vSpi1Write(RegFrfMid, (u8)(uiFrf >> 8));
  vSpi1Write(RegFrfLsb, (u8)uiFrf);
}

/**
  * @brief Power Amplifier Initialization
  * @param ucPaSelect: Power Amplifier Value 
  * @retval None
  */
void vPaSelectInit(u8 ucPaSelect)
{
  u8 ucData = 0;
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
void vMaxPowerInit(u8 ucMaxPower)
{
  u8 ucData = 0;
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
void vOutputPowerInit(u8 ucOutputPower)
{
  u8 ucData = 0;
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
void vPaRampInit(u8 ucPaRamp)
{
  u8 ucData = 0;
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
void vOcpOnInit(u8 ucOcpOn)
{
  u8 ucData = 0;
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
void vOcpTrimInit(u8 ucOcpTrim)
{
  u8 ucData = 0;
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
void vLnaGainInit(u8 ucLnaGain)
{
  u8 ucData = 0;
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
void vLnaBoostLfInit(u8 ucLnaBoostLf)
{
  u8 ucData = 0;
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
void vLnaBoostHfInit(u8 ucLnaBoostHf)
{
  u8 ucData = 0;
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
// u8 ucFifoAddrPtrRead(void)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegFifoAddrPtr);
//     return ucData;
// }

// /**
//   * @brief Write Address Pointer in FIFO Data Buffer
//   * @param ucFifoAddrPtr: FIFO Address Pointer Value
//   * @retval None
//   */
// void vFifoAddrPtrWrite(u8 ucFifoAddrPtr)
// {
//     vSpi1Write(RegFifoAddrPtr, ucFifoAddrPtr);
// }

/**
  * @brief Base address in FIFO data buffer for TX modulator Initialization
  * @param ucFifoTxBaseAddr: Base Address Value for Tx
  * @retval None
  */
void vFifoTxBaseAddrInit(u8 ucFifoTxBaseAddr)
{
  vSpi1Write(RegFifoTxBaseAddr, ucFifoTxBaseAddr);
}

/**
  * @brief Base address in FIFO data buffer for RX modulator Initialization
  * @param ucFifoRxBaseAddr: Base Address Value for Rx
  * @retval None
  */
void vFifoRxBaseAddrInit(u8 ucFifoRxBaseAddr)
{
  vSpi1Write(RegFifoRxBaseAddr, ucFifoRxBaseAddr);
}

/**
  * @brief Read Start address (in data buffer) of last packet received
  * @param None
  * @retval Value of Start address (in data buffer) of last packet received
  */
u8 ucFifoRxCurrentAddrRead(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegFifoRxCurrentAddr);
  return ucData;
}

// void vRxTimeoutMaskInit(u8 ucRxTimeoutMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0x7F;
//     if(ucRxTimeoutMask == 0u || ucRxTimeoutMask == 1u)
//     {
//         ucData |= (ucRxTimeoutMask << 7);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vRxDoneMaskInit(u8 ucRxDoneMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xBF;
//     if(ucRxDoneMask == 0u || ucRxDoneMask == 1u)
//     {
//         ucData |= (ucRxDoneMask << 6);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vPayloadCrcErrorMaskInit(u8 ucPayloadCrcErrorMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xDF;
//     if(ucPayloadCrcErrorMask == 0u || ucPayloadCrcErrorMask == 1u)
//     {
//         ucData |= (ucPayloadCrcErrorMask << 5);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vValidHeaderMaskInit(u8 ucValidHeaderMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xEF;
//     if(ucValidHeaderMask == 0u || ucValidHeaderMask == 1u)
//     {
//         ucData |= (ucValidHeaderMask << 4);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vTxDoneMaskInit(u8 ucTxDoneMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xF7;
//     if(ucTxDoneMask == 0u || ucTxDoneMask == 1u)
//     {
//         ucData |= (ucTxDoneMask << 3);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vCadDoneMaskInit(u8 ucCadDoneMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xFB;
//     if(ucCadDoneMask == 0u || ucCadDoneMask == 1u)
//     {
//         ucData |= (ucCadDoneMask << 2);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vFhssChangeChannelMaskInit(u8 ucFhssChangeChannelMask)
// {
//     u8 ucData = 0;
//     ucData = ucSpi1Read(RegIrqFlagsMask);
//     ucData &= 0xFD;
//     if(ucFhssChangeChannelMask == 0u || ucFhssChangeChannelMask == 1u)
//     {
//         ucData |= (ucFhssChangeChannelMask << 1);
//         vSpi1Write(RegIrqFlagsMask, ucData);
//     }
// }

// void vCadDetectedMaskInit(u8 ucCadDetectedMask)
// {
//     u8 ucData = 0;
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
void vIrqFlagsMaskInit(u8 ucIrqFlagsMask)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegIrqFlagsMask);
  ucData |= ucIrqFlagsMask;
  vSpi1Write(RegIrqFlagsMask, ucData);
}

/**
  * @brief Read Interrupt Flags
  * @param None
  * @retval Status Interrupt Flags
  */
u8 ucIrqFlagsRead(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegIrqFlags);
  return ucData;
}

/**
  * @brief Clear Interrupt Flags
  * @param ucIrqFlags: Value of Flags need clear
  * @retval None
  */
void vIrqFlagsClear(u8 ucIrqFlags)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegIrqFlags);
  ucData |= ucIrqFlags;
  vSpi1Write(RegIrqFlags, ucData);
}

/**
  * @brief Read Number of payload bytes of latest packet received
  * @param None
  * @retval Number of payload bytes
  */
u8 ucFifoRxBytesNbRead(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegRxNbBytes);
  return ucData;
}

/**
  * @brief Read Number of valid headers received 
  * @param None
  * @retval Number of valid headers received
  */
u16 usValidHeaderCntRead(void)
{
  u16 ucData = 0;
  ucData = ((u16)ucSpi1Read(RegRxHeaderCntValueMsb) << 8);
  ucData |= (u16)ucSpi1Read(RegRxHeaderCntValueLsb);
  return ucData;
}

/**
  * @brief Read Number of valid packets received 
  * @param None
  * @retval Number of valid packets received
  */
u16 usValidPacketCntRead(void)
{
  u16 ucData = 0;
  ucData = ((u16)ucSpi1Read(RegRxPacketCntValueMsb) << 8);
  ucData |= (u16)ucSpi1Read(RegRxPacketCntValueLsb);
  return ucData;
}

/**
  * @brief Read Coding rate of last header received
  * @param None
  * @retval Value Coding rate of last header received
  */
u8 ucRxCodingRateRead(void)
{
  u8 ucData;
  ucData = (ucSpi1Read(RegModemStat) >> 5);
  return ucData;
}

/**
  * @brief Read Modem Status
  * @param None
  * @retval Value of Modem Status
  */
u8 ucModemStatusRead(void)
{
  u8 ucData;
  ucData = 0x1F & ucSpi1Read(RegModemStat);
  return ucData;
}

/**
  * @brief Read Estimation of SNR on last packet received
  * @param None
  * @retval Value Estimation of SNR on last packet received
  */
u8 ucPacketSnrRead(void)
{
  u8 ucData;
  ucData = ucSpi1Read(RegPktSnrValue);
  return ucData;
}

/**
  * @brief Read RSSI of the latest packet received
  * @param None
  * @retval Value RSSI of the latest packet received
  */
u16 ucPacketRssiRead(void)
{
  u16 rawRssi = ucSpi1Read(RegPktRssiValue);
  u16 rawSNR = ucSpi1Read(RegPktSnrValue) / 4;
  return (-164 + rawRssi + rawSNR * 0.25);
}

/**
  * @brief Read Current RSSI value
  * @param None
  * @retval Current RSSI value
  */
u8 ucRssiRead(void)
{
  u8 ucData;
  ucData = ucSpi1Read(RegRssiValue);
  return ucData;
}

/**
  * @brief Read PLL failed to lock while attempting a TX/RX/CAD operation
  * @param None
  * @retval Status PLL 
  */
u8 ucPllTimeoutRead(void)
{
  u8 ucData;
  ucData = (ucSpi1Read(RegHopChannel) >> 7);
  return ucData;
}

/**
  * @brief Read CRC Information extracted from the received packet header
  * @param None
  * @retval Status CRC 
  */
u8 ucCrcOnPayloadread(void)
{
  u8 ucData;
  ucData = 0x01 & (ucSpi1Read(RegHopChannel) >> 6);
  return ucData;
}

/**
  * @brief Read Current value of frequency hopping channel
  * @param None
  * @retval Current value of frequency hopping channel 
  */
u8 ucFhssPresentChannelRead(void)
{
  u8 ucData;
  ucData = 0x3F & ucSpi1Read(RegHopChannel);
  return ucData;
}

/**
  * @brief Signal bandwidth Initialization
  * @param ucBandWidth: BandWidth Value
  * @retval None 
  */
void vBandWidthInit(u8 ucBandWidth)
{
  u8 ucData = 0;
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
void vCodingRateInit(u8 ucCodingRate)
{
  u8 ucData = 0;
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
void vImplicitHeaderModeOnInit(u8 ucHeaderMode)
{
  u8 ucData = 0;
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
void vSpreadingFactorInit(u8 ucSpreadingFactor)
{
  u8 ucData = 0;
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
void vTxContinuousModeInit(u8 ucTxContinuousMode)
{
  u8 ucData = 0;
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
void vRxPayloadCrcOnInit(u8 ucRxPayloadCrcOn)
{
  u8 ucData = 0;
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
void vSymbTimeoutInit(u16 ucSymbTimeout)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegModemConfig2);
  ucData &= 0xFC;
  ucData |= ((0x03) & (u8)(ucSymbTimeout >> 8));
  vSpi1Write(RegModemConfig2, ucData);
  vSpi1Write(RegSymbTimeoutLsb, (u8)ucSymbTimeout);
}

/**
  * @brief Preamble Length Initialization
  * @param ucPreambleLength: Preamble Length Value
  * @retval None
  */
void vPreambleLengthInit(u16 ucPreambleLength)
{
  vSpi1Write(RegPreambleMsb, (u8)(ucPreambleLength >> 8));
  vSpi1Write(RegPreambleLsb, (u8)(ucPreambleLength));
}

/**
  * @brief Payload Length Initialization
  * @param ucPayloadLength: Payload Length Value
  * @retval None
  */
void vPayloadLengthInit(u8 ucPayloadLength)
{
  vSpi1Write(RegPayloadLength, ucPayloadLength);
}

/**
  * @brief Payload Max Length Initialization
  * @param ucPayloadMaxLength: Payload Max Length Value
  * @retval None
  */
void vPayloadMaxLengthInit(u8 ucPayloadMaxLength)
{
  vSpi1Write(RegMaxPayloadLength, ucPayloadMaxLength);
}

/**
  * @brief Symbol periods between frequency hops Initialization
  * @param ucFreqHoppingPeriod: Payload Max Length Value
  * @retval None
  */
void vFreqHoppingPeriodInit(u8 ucFreqHoppingPeriod)
{
  vSpi1Write(RegHopPeriod, ucFreqHoppingPeriod);
}

/**
  * @brief Read Current value of RX databuffer pointer
  * @param None
  * @retval Address of last byte written by Lora receiver
  */
u8 ucFifoRxByteAddrPtr(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegFifoRxByteAddr);
  return ucData;
}

/**
  * @brief Low Data Rate Optimize Initialization
  * @param ucLowDataRateOptimize: Low Data Rate Optimize Value to Disable or Enable 
  * @retval None
  */
void vLowDataRateOptimizeInit(u8 ucLowDataRateOptimize)
{
  u8 ucData = 0;
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
void vAgcAutoOnInit(u8 ucAgcAutoOn)
{
  u8 ucData = 0;
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
  uiData |= (((unsigned int)(ucSpi1Read(RegFeiMsb) & 0x0F) << 16) |
             ((unsigned int)ucSpi1Read(RegFeiMid) << 8) |
             (unsigned int)ucSpi1Read(RegFeiLsb));
  return uiData;
}

/**
  * @brief Read Wideband RSSI measurement
  * @param None
  * @retval  Wideband RSSI measurement Value
  */
u8 ucRssiWidebandInit(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegRssiWideband);
  return ucData;
}

/**
  * @brief  LoRa Detection Optimize Initialization
  * @param ucDetectionOptimize: Detection Optimize Value 
  * @retval None
  */
void vDetectionOptimizeInit(u8 ucDetectionOptimize)
{
  u8 ucData = 0;
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
void vInvertIQInit(u8 ucInvertIQ)
{
  u8 ucData = 0;
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
void vDetectionThresholdInit(u8 ucDetectionThreshold)
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
void vSyncWordInit(u8 ucSyncWord)
{
  vSpi1Write(RegSyncWord, ucSyncWord);
}

/**
  * @brief Mapping of pins DIO0 Initialization
  * @param ucDio0Mapping: Dio0 Mapping Value
  * @retval None
  */
void vDio0MappingInit(u8 ucDio0Mapping)
{
  u8 ucData = 0;
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
void vDio1MappingInit(u8 ucDio1Mapping)
{
  u8 ucData = 0;
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
void vDio2MappingInit(u8 ucDio2Mapping)
{
  u8 ucData = 0;
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
void vDio3MappingInit(u8 ucDio3Mapping)
{
  u8 ucData = 0;
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
void vDio4MappingInit(u8 ucDio4Mapping)
{
  u8 ucData = 0;
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
void vDio5MappingInit(u8 ucDio5Mapping)
{
  u8 ucData = 0;
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
void vMapPreambleDetectInit(u8 ucMapPreambleDetect)
{
  u8 ucData = 0;
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
u8 ucVersionRead(void)
{
  u8 ucData = 0;
  ucData = ucSpi1Read(RegVersion);
  return ucData;
}

/**
  * @brief Controls the crystal oscillator Initialization
  * @param ucTcxoInputOn: Tcxo Input On Value
  * @retval None
  */
void vTcxoInputOnInit(u8 ucTcxoInputOn)
{
  u8 ucData = 0;
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
void vPaDacInit(u8 ucPaDac)
{
  u8 ucData = 0;
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
void vFormerTempInit(u8 ucFormerTemp)
{
  vSpi1Write(RegFormerTemp, ucFormerTemp);
}

/**
  * @brief Sets the floor reference for all AGC thresholds Initialization
  * @param ucAgcReferenceLevel: Agc Reference Level Value
  * @retval None
  */
void vAgcReferenceLevelInit(u8 ucAgcReferenceLevel)
{
  u8 ucData = 0;
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
void vAgcStep1Init(u8 ucAgcStep1)
{
  u8 ucData = 0;
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
void vAgcStep2Init(u8 ucAgcStep2)
{
  u8 ucData = 0;
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
void vAgcStep3Init(u8 ucAgcStep3)
{
  u8 ucData = 0;
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
void vAgcStep4Init(u8 ucAgcStep4)
{
  u8 ucData = 0;
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
void vAgcStep5Init(u8 ucAgcStep5)
{
  u8 ucData = 0;
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
void vPllBandwidth(u8 ucPllBandwidth)
{
  u8 ucData = 0;
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
  STM_LOGD("lora.c", "---------- LoraInit Start ----------");

  vLongRangeModeInit(LORA_MODE); /* ANCHOR CHECKED Init Module Lora into Lora TM Mode */
  LORA_GET_REGISTER(RegOpMode);

  vModeInit(STDBY_MODE);                              /* Init Module Lora into Standby Mode */
  vAccessSharedRegInit(ACCESS_LORA_REGISTERS);        /* ANCHOR CHECKED Access LoRa registers page 0x0D: 0x3F */
  vLowFrequencyModeOnInit(ACCESS_LOW_FREQUENCY_MODE); /* CHECKED Access Low Frequency Mode registers */
  LORA_GET_REGISTER(RegOpMode);

  vFrfInit(RF_FREQUENCY); /* ANCHOR CHECKED Init RF carrier frequency */
  LORA_GET_REGISTER(RegFrfMsb);
  LORA_GET_REGISTER(RegFrfMid);
  LORA_GET_REGISTER(RegFrfLsb);

  vPaSelectInit(PA_BOOST); /* ANCHOR CHECKEDOutput power is limited to +20 dBm */
  // vMaxPowerInit(MAX_POWER);
  vOutputPowerInit(OUTPUT_POWER); /* ANCHOR CHECKEDPout=17-(15-OutputPower) */
  LORA_GET_REGISTER(RegPaConfig);

  // vPaRampInit(PA_RAMP);
  LORA_GET_REGISTER(RegPaRamp);

  // vOcpOnInit(OCP_ON); /* OCP enabled */
  vOcpTrimInit(OCP_TRIM); /* CHECKED Trimming of OCP current: Imax = 240mA */
  LORA_GET_REGISTER(RegOcp);

  // vLnaGainInit(G1); /* LNA gain setting: G1 = maximum gain */
  // vLnaBoostLfInit(LNA_BOOST_LF); /* Low Frequency (RFI_LF) LNA current adjustment Default LNA current */
  // vLnaBoostHfInit(LNA_BOOST_HF); /* High Frequency (RFI_HF) LNA current adjustment Boost on, 150% LNA current */
  LORA_GET_REGISTER(RegLna);

  // vFifoTxBaseAddrInit(FIFO_TX_BASE_ADDR); /* ANCHOR CHECKEDWrite base address in FIFO data buffer for TX modulator */
  LORA_GET_REGISTER(RegFifoTxBaseAddr);

  // vFifoRxBaseAddrInit(FIFO_RX_BASE_ADDR); /* ANCHOR CHECKEDRead base address in FIFO data buffer for RX demodulator */
  LORA_GET_REGISTER(RegFifoRxBaseAddr);

  // vIrqFlagsMaskInit(IRQ_FLAGS_MASK); /* ANCHOR CHECKEDDisable all interrupts mask */
  LORA_GET_REGISTER(RegIrqFlagsMask);

  // vBandWidthInit(BANDWIDTH_125K); /* ANCHOR CHECKED Signal bandwidth: 250kHz */
  vCodingRateInit(CODING_RATE_4_5); /* ANCHOR Error coding rate 4/5 */

  // vBandWidthInit(BANDWIDTH_125K);
  // vCodingRateInit(CODING_RATE_4_5);
  vImplicitHeaderModeOnInit(IMPLICIT_HEADER); /* ANCHOR Init Implicit Header mode */
  LORA_GET_REGISTER(RegModemConfig1);

  vSpreadingFactorInit(SPREADING_FACTOR_6_64); /* ANCHOR SF rate 64 chips / symbol */
  // vTxContinuousModeInit(TX_SINGLE); /* ANCHOR Normal mode, a single packet is sent */
  vRxPayloadCrcOnInit(CRC_ENABLE); /* ANCHOR Enable CRC generation and check on payload */
  LORA_GET_REGISTER(RegModemConfig2);

  // vSymbTimeoutInit(RX_TIMEOUT); /* ANCHOR RX operation time-out */
  // LORA_GET_REGISTER(RegModemConfig2);
  // LORA_GET_REGISTER(RegSymbTimeoutLsb);

  // vPreambleLengthInit(PREAMBLE_LENGTH); /* ANCHOR Preamble length = PreambleLength + 4.25 Symbols */
  vPreambleLengthInit(0x00FF);
  // LORA_GET_REGISTER(RegPreambleMsb);
  // LORA_GET_REGISTER(RegPreambleLsb);

  vPayloadLengthInit(PAYLOAD_LENGTH); /* ANCHOR CHECKED Init Payload length */
  LORA_GET_REGISTER(RegPayloadLength);

  // vPayloadMaxLengthInit(PAYLOAD_MAX_LENGTH); /* ANCHOR Maximum payload length */
  // LORA_GET_REGISTER(RegMaxPayloadLength);

  // vFreqHoppingPeriodInit(FREQ_HOPPING_PERIOD); /* Symbol periods between frequency hops */
  // LORA_GET_REGISTER(RegHopPeriod);

  // vLowDataRateOptimizeInit(LOW_DATA_RATE_OPTIMIZE); /*  Enabled; mandated for when the symbol length exceeds16ms */
  // vAgcAutoOnInit(AGC_AUTO); /* 0 -> LNA gain set by register LnaGain 1 -> LNA gain set by the internal AGC loop*/
  // LORA_GET_REGISTER(RegModemConfig3);

  vDetectionOptimizeInit(LORA_DETECTION_OPTIMIZE); /* LoRa Detection Optimize 0x03 -> SF7 to SF12; 0x05 -> SF6 */
  LORA_GET_REGISTER(RegDetectOptimize);

  // vInvertIQInit(INVERT_IQ); /* ANCHOR Invert the LoRa I and Q signals */
  // LORA_GET_REGISTER(RegInvertIQ);

  vDetectionThresholdInit(LORA_DETECTION_THRESHOLD); /* ANCHOR LoRa detection threshold 0x0A -> SF7 to SF12; 0x0C -> SF6 */
  LORA_GET_REGISTER(RegDetectionThreshold);

  // vSyncWordInit(LORA_SYNC_WORD); /* ANCHOR Init Sync Word */
  LORA_GET_REGISTER(RegSyncWord);

  // vAgcReferenceLevelInit(AGC_REFERENCE); /* Sets the floor reference for all AGC thresholds */
  // LORA_GET_REGISTER(RegAgcRef);

  // vAgcStep1Init(AGC_STEP1); /* Defines the 1st AGC Threshold */
  // LORA_GET_REGISTER(RegAgcRef);

  // vAgcStep2Init(AGC_STEP2); /* Defines the 2nd AGC Threshold */
  // LORA_GET_REGISTER(RegAgcThresh2);

  // vAgcStep3Init(AGC_STEP3); /* Defines the 3rd AGC Threshold */
  // LORA_GET_REGISTER(RegAgcThresh2);

  // vAgcStep4Init(AGC_STEP4); /* Defines the 4th AGC Threshold */
  // LORA_GET_REGISTER(RegAgcThresh3);

  // vAgcStep5Init(AGC_STEP5); /* Defines the 5th AGC Threshold */
  // LORA_GET_REGISTER(RegAgcThresh3);

  // vPllBandwidth(PLL_BANDWIDTH); /* Controls the PLL bandwidth */
  // LORA_GET_REGISTER(RegPll);

  // vDio0MappingInit(RX_DONE); /* CAD: Channel activity detection */
  // LORA_GET_REGISTER(RegDioMapping1);

  // vMapPreambleDetect(PREAMBBLE_DETECT_INTERRUPT);
  // LORA_GET_REGISTER(RegTcxo);
  // printf("RegTcxo = 0x%x\r\n", ucData);

  vTcxoInputOnInit(XTAL_INPUT); /* ANCHOR Controls the crystal oscillator */
  LORA_GET_REGISTER(RegTcxo);

  vPaDacInit(PA_DAC); /* Enables the +20dBm option on PA_BOOST pin */
  LORA_GET_REGISTER(RegPaDac);
  LORA_GET_REGISTER(RegLna);
  LORA_GET_REGISTER(RegVersion);
  LORA_GET_REGISTER(RegOpMode);

  STM_LOGD("lora.c", "---------- LoraInit Finish ----------");
}

/**
  * @brief Transmit Data into FIFO
  * @param pcTxBuffer: Data Address Transmit
  * @param bRepeat: Tx Single or Rx Coninuous
  * @retval None
  */
void vLoraTransmit(u8 *pcTxBuffer, bool bRepeat)
{
  u8 ucData = 0;
  u8 i = 0;
  u8 ucIrqStatus = 0;

  STM_LOGD("lora.c", "---------- Start Transmit ----------");

  vModeInit(STDBY_MODE); /* Init Module Lora into Standby Mode */
  ucData = ucSpi1Read(RegOpMode);
  printf("Init Standby Mode: RegOpMode = 0x%XH\r\n", ucData);

  vSpi1Write(RegFifoAddrPtr, FIFO_TX_BASE_ADDR); /* Set FifoPtrAddr to FifoTxPtrBase */
  ucData = ucSpi1Read(RegFifoAddrPtr);
  printf("Set Write Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);

  do
  {
    for (i = 0u; i < PAYLOAD_LENGTH; i++)
    {
      ucData = *(pcTxBuffer + i);
      vSpi1Write(RegFifo, *(pcTxBuffer + i));
    }
    ucData = ucSpi1Read(RegIrqFlags);
    printf("Check TxDone Flag Before Start Transmit: RegIrqFlags = 0x%XH\r\n", ucData);

    vModeInit(TX_MODE); /* Init Module Lora into TX Mode */
    ucData = ucSpi1Read(RegOpMode);
    printf("Init Tx Mode: RegOpMode = 0x%XH\r\n", ucData);

    ucIrqStatus = ucSpi1Read(RegIrqFlags);
    printf("Check TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

    while ((ucSpi1Read(RegIrqFlags) & 0x08u) == 0u)
      ; /* Wait for TxDone set */

    ucData = ucSpi1Read(RegOpMode);
    printf("Check Automatic Mode change STANDBY: RegOpMode = 0x%XH\r\n", ucData);

    if ((ucData & 0x07u) == 1u) /* If Automatic Mode change STANDBY */
    {
      printf("Automatic Mode change STANDBY from TX Continuous Mode\r\n");
    }
    else /* Automatic Mode change STANDBY */
    {
      printf("Automatic Mode change STANDBY fail from TX Continuous Mode\r\n");
    }

    /* Begin Read Data Transmit in FIFO */
    vSpi1Write(RegFifoAddrPtr, FIFO_TX_BASE_ADDR); /* Set FifoPtrAddr to FifoTxPtrBase */
    ucData = ucSpi1Read(RegFifoAddrPtr);
    printf("Set Write Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);

    ucData = ucSpi1Read(RegFifo); /* Check Destination Address */
    printf("Check Destination Address in FiFo: RegFifo = 0x%XH\r\n", ucData);
    ucData = ucSpi1Read(RegFifo); /* Check Source Address */
    printf("Check Source Address in FiFo: RegFifo = 0x%XH\r\n", ucData);
    ucData = ucSpi1Read(RegFifo); /* Check Led Status */
    printf("Check Led Status in FiFo: RegFifo = 0x%XH\r\n", ucData);
    /* End Read Data Transmit in FIFO */

    ucData = ucSpi1Read(RegIrqFlags);
    printf("Check TxDone Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

    vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x08u)); /* Clear TxDone Flag */
    ucData = ucSpi1Read(RegIrqFlags);
    printf("Clear TxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);
  } while (bRepeat);

  vModeInit(SLEEP_MODE); /* Init Module Lora into Sleep Mode */
  ucData = ucSpi1Read(RegOpMode);
  printf("Check Sleep Mode: RegOpMode = 0x%XH\r\n", ucData);
  printf("Module switch to Sleep Mode\r\n");
  STM_LOGD("lora.c", "---------- Finish Transmit ----------");
}

/**
  * @brief Receive Data in FIFO
  * @param pcRxBuffer: Data Address Receive
  * @param bRepeat: Receive Mode: Rx Single or Continuous
  * @retval None
  */
void vLoraReceive(u8 *pcRxBuffer, bool bRepeat)
{
  u8 ucData = 0;
  u8 i = 0;
  u8 ucIrqStatus = 0;

  printf("****************** Start Receive ********************\r\n\r\n");

  /* Init Module Lora into Standby Mode */
  vModeInit(STDBY_MODE);
  ucData = ucSpi1Read(RegOpMode);
  printf("Init Standby Mode: RegOpMode = 0x%XH\r\n", ucData);

  /* Set FifoPtrAddr to FifoRxPtrBase */
  vSpi1Write(RegFifoAddrPtr, FIFO_RX_BASE_ADDR);
  ucData = ucSpi1Read(RegFifoAddrPtr);
  printf("Set Read Base Address to FifoAddrPtr: RegFifoAddrPtr = 0x%XH\r\n", ucData);

  /* If Rx Continuous Mode */
  if (bRepeat == true)
  {
    /* Init Module Lora into RX Coninuous Mode */
    vModeInit(RXCONTINUOUS_MODE);
    ucData = ucSpi1Read(RegOpMode);
    printf("Init Rx Continuous Mode: RegOpMode = 0x%XH\r\n", ucData);

    while (1)
    {
      ucData = ucSpi1Read(RegIrqFlags);
      printf("Check RxDone Flags Before: RegIrqFlags = 0x%XH\r\n", ucData);

      /* Wait for RxDone set */
      while ((ucSpi1Read(RegIrqFlags) & 0x40u) == 0u)
        ;

      ucIrqStatus = ucSpi1Read(RegIrqFlags);
      printf("Check PayloadCrcError and RxDone Flags: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

      /* If PayloadCrcError Flag not set and ValidHeader Flag set */
      if (((ucIrqStatus & 0x20u) == 0u) && ((ucIrqStatus & 0x10u) != 0u))
      {
        ucData = ucSpi1Read(RegFifoRxCurrentAddr);
        vSpi1Write(RegFifoAddrPtr, ucData);

        for (i = 0u; i < PAYLOAD_LENGTH; i++)
        {
          *(pcRxBuffer + i) = ucSpi1Read(RegFifo);
          ucData = *(pcRxBuffer + i);
          printf("Read Data From FIFO: RegFifo = 0x%XH\r\n", ucData);
        }
        /* Read RSSI Value */

        //        printf("RSSI Value = %d\r\n", sRssiRead());

        /* Clear RxDone Flag */
        vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x40u));
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Clear RxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);

        /* Clear ValidHeader Flag */
        vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x10u));
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Clear ValidHeader Flag: RegIrqFlags = 0x%XH\r\n", ucData);
      }

      /* If PayloadCrcError Flag set */
      else
      {
        printf("Data Receive Failed\r\n");

        ucData = ucSpi1Read(RegIrqFlags);
        printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

        /* Clear PayloadCrcError Flag */
        vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x20u));
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Clear PayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucData);
      }
      printf("****************** Finish Receive ********************\r\n\r\n");
    }
  }

  /* If Rx Single Module */
  else
  {
    /* Init Module Lora into RX Single Mode */
    vModeInit(RXSINGLE_MODE);
    ucData = ucSpi1Read(RegOpMode);
    printf("Init Rx Single Mode: RegOpMode = 0x%XH\r\n", ucData);

    ucData = ucSpi1Read(RegIrqFlags);
    printf("Check RxTimeout and RxDone Flags: RegIrqFlags = 0x%XH\r\n", ucData);

    /* Wait for RxTimeout or RxDone Flag set */
    while (((ucSpi1Read(RegIrqFlags) & 0x40u) | (ucSpi1Read(RegIrqFlags) & 0x80)) == 0u)
      ;

    ucIrqStatus = ucSpi1Read(RegIrqFlags);
    printf("Check RxTimeout or RxDone: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

    /* If RxDone Flag set */
    if ((ucIrqStatus & 0x40u) != 0u)
    {
      ucData = ucSpi1Read(RegOpMode);
      printf("Check Automatic Mode change STANDBY: RegOpMode = 0x%XH\r\n", ucData);

      /* If Automatic Mode change STANDBY */
      if ((ucData & 0x01u) != 0u)
      {
        printf("Automatic Mode change STANBY from RX Single Mode\r\n");

        ucIrqStatus = ucSpi1Read(RegIrqFlags);
        printf("Check PlayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

        /* If PlayloadCrcError Flag not set and ValidHeader Flag set */
        if (((ucIrqStatus & 0x20u) == 0u) && ((ucIrqStatus & 0x10u) != 0u))
        {
          ucData = ucSpi1Read(RegFifoRxCurrentAddr);
          vSpi1Write(RegFifoAddrPtr, ucData);
          for (i = 0u; i < PAYLOAD_LENGTH; i++)
          {
            *(pcRxBuffer + i) = ucSpi1Read(RegFifo);
            ucData = *(pcRxBuffer + i);
            printf("Read Data From FIFO: RegFifo = 0x%XH\r\n", ucData);
          }
          ucData = ucSpi1Read(RegIrqFlags);
          printf("Check RxDone and ValidHeader Flags Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

          vSpi1Write(RegFifoAddrPtr, FIFO_RX_BASE_ADDR);

          /* Clear ValidHeader Flag */
          vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x10u));
          ucData = ucSpi1Read(RegIrqFlags);
          printf("Clear ValidHeader Flag: RegIrqFlags = 0x%XH\r\n", ucData);

          /* Clear RxDone Flag */
          vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x40u));
          ucData = ucSpi1Read(RegIrqFlags);
          printf("Clear RxDone Flag: RegIrqFlags = 0x%XH\r\n", ucData);
        }

        /* If PlayloadCrcError Flag set  */
        else
        {
          /* Read Flags Status */
          ucIrqStatus = ucSpi1Read(RegIrqFlags);
          printf("Check PayloadCrcError Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucIrqStatus);

          /* Clear PayloadCrcError Flag */
          vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x20u));
          ucData = ucSpi1Read(RegIrqFlags);
          printf("Clear PayloadCrcError Flag: RegIrqFlags = 0x%XH\r\n", ucData);
        }

        /* Init Module Lora into Sleep Mode */
        vModeInit(SLEEP_MODE);
        ucData = ucSpi1Read(RegOpMode);
        printf("Init Sleep Mode: RegOpMode = 0x%XH\r\n", ucData);
      }

      /* If Automatic Mode chang STANBY fail */
      else
      {
        printf("Automatic Mode change STANBY fail from RX Single Mode\r\n");
      }
    }

    /* If Rxtimeout Flag set */
    else
    {
      ucData = ucSpi1Read(RegOpMode);
      printf("Check Standby Mode: RegOpMode = 0x%XH\r\n", ucData);
      if ((ucData & 0x01u) != 0u)
      {
        printf("Automatic Mode change STANBY from RX Single Mode\r\n");

        ucData = ucSpi1Read(RegIrqFlags);
        printf("Check Timeout Flag Before Clear: RegIrqFlags = 0x%XH\r\n", ucData);

        /* Clear Timeout Flag */
        vSpi1Write(RegIrqFlags, (ucIrqStatus & 0x80u));
        ucData = ucSpi1Read(RegIrqFlags);
        printf("Clear Timeout Flag: RegIrqFlags = 0x%XH\r\n", ucData);
      }

      /* If Automatic Mode chang STANBY fail */
      else
      {
        printf("Automatic Mode change STANBY fail from RX Single Mode\r\n");
      }
    }
  }
  printf("****************** Finish Receive ********************\r\n\r\n");
}

void vLoRaConfigure(u8 bandwidth, u8 sf, u8 cr)
{
  vBandWidthInit(bandwidth);
  vSpreadingFactorInit(sf);
  vCodingRateInit(cr);
}

u16 usLoRaGetPreamble(void)
{
  return ucSpi1Read(RegPreambleMsb) << 8 |
         ucSpi1Read(RegPreambleLsb);
}

u8 usLoRaGetBandwidth(void)
{
  return (ucSpi1Read(RegModemConfig1) & BANDWIDTH_Msk) >> BANDWIDTH_MskPos;
}

u8 usLoRaGetCodingRate(void)
{
  return (ucSpi1Read(RegModemConfig1) & CODING_RATE_Msk) >> CODING_RATE_MskPos;
}

u8 usLoRaGetHeaderMode(void)
{
  return (ucSpi1Read(RegModemConfig1) & IMPLICIT_HEADER_MODE_ON_Msk) >> IMPLICIT_HEADER_MODE_ON_MskPos;
}

u8 usLoraGetSpreadingFactor(void)
{
  return (ucSpi1Read(RegModemConfig2) & SPREADING_FACTOR_Msk) >> SPREADING_FACTOR_MskPos;
}

void LoRaTransmit(u8 *data, u8 size, u32 timeoutMs)
{
  static bool isInit = false;
  if (!isInit)
  {
    vModeInit(STDBY_MODE);
    LORA_GET_REGISTER(RegOpMode);

    /* TX INIT */
    LORA_GET_REGISTER(RegFifoAddrPtr);
    isInit = true;
  }
  /* STANDBY MODE */

  vSpi1Write(RegFifoAddrPtr, FIFO_TX_BASE_ADDR); /* Set FifoPtrAddr to FifoTxPtrBase */
  /* WRITE DATA FIFO */
  for (size_t i = 0u; i < PAYLOAD_LENGTH; i++)
  {
    STM_LOGV("Transmit", "data transmit[%d]: 0x%x", i, *(data + i));
    LORA_GET_REGISTER(RegFifoAddrPtr);
    vSpi1Write(RegFifo, *(data + i));
  }
  /* MODE REQUEST TX */
  vModeInit(TX_MODE);
  LORA_GET_REGISTER(RegOpMode);
  /* Init Module Lora into TX Mode */
  u32 startTick = HAL_GetTick();
  while ((ucSpi1Read(RegIrqFlags) & TX_DONE_Msk) >> TX_DONE_MskPos == 0u)
  {
    // STM_LOGV("Transmit", "Wait for TX_DONE ...\r\n");
    if (HAL_GetTick() - startTick >= timeoutMs)
    {
      STM_LOGE("TransmitErr", "TX_TIMEOUT");
      break;
    }
  }
  /* Clear TxDone Flag */
  vSpi1Write(RegIrqFlags, TX_DONE_Msk);
  LORA_GET_REGISTER(RegIrqFlags);
}

void LoRaReceiveCont(u8 *outData, u8 size, u32 timeoutMs)
{
  static bool isInit = false;
  if (!isInit)
  {
    /* STANDY BY MODE */
    vModeInit(STDBY_MODE);
    LORA_GET_REGISTER(RegOpMode);

    /* RX CONTINUOUS MODE */
    // if (rxMode == RXCONTINUOUS_MODE)
    vSpi1Write(RegFifoAddrPtr, FIFO_RX_BASE_ADDR);
    vModeInit(RXCONTINUOUS_MODE);
    // else if (rxMode == RXSINGLE_MODE)
    //   vModeInit(RXSINGLE_MODE);
    LORA_GET_REGISTER(RegOpMode);
    isInit = true;
  }

  /* WAIT FOR RX_DONE */
  if (timeoutMs == LORA_MAX_DELAY)
  {
    while ((ucSpi1Read(RegIrqFlags) & RX_DONE_Msk) >> RX_DONE_MskPos == 0u)
      ;
  }
  else
  {
    u32 startTick = HAL_GetTick();
    while ((ucSpi1Read(RegIrqFlags) & RX_DONE_Msk) >> RX_DONE_MskPos == 0u)
    {
      // STM_LOGV("Transmit", "Wait for TX_DONE ...\r\n");
      if (HAL_GetTick() - startTick >= timeoutMs)
      {
        STM_LOGE("ReceiverErr", "RX_TIMEOUT");
        break;
      }
    }
  }

  if ((ucSpi1Read(RegIrqFlags) & RX_DONE_Msk) >> RX_DONE_MskPos)
  {
    /* PAYLOAD_CRC CHECK */
    u8 temp = ucSpi1Read(RegIrqFlags);
    if ((temp & PAYLOAD_CRC_ERROR_Msk) >> PAYLOAD_CRC_ERROR_MskPos == 1)
    {
      STM_LOGE("ReceiveErr", "Payload CRC Failed");
    }
    else
    {
      vSpi1Write(RegFifoAddrPtr, ucSpi1Read(RegFifoRxCurrentAddr));
      for (size_t i = 0u; i < PAYLOAD_LENGTH; i++)
      {
        *(outData + i) = ucSpi1Read(RegFifo);
        STM_LOGV("Transmit", "data receive[%d]: 0x%x", i, *(outData + i));
        LORA_GET_REGISTER(RegFifoAddrPtr);
      }
    }
  }

  /* CLEAR RX_DONE FLAG */
  vSpi1Write(RegIrqFlags, RX_DONE_Msk | PAYLOAD_CRC_ERROR_Msk);
  LORA_GET_REGISTER(RegIrqFlags);
}

void LoRaReceiveContInISR(u8 *outData, u8 size)
{
  if ((ucSpi1Read(RegIrqFlags) & RX_DONE_Msk) >> RX_DONE_MskPos)
  {
    /* PAYLOAD_CRC CHECK */
    u8 temp = ucSpi1Read(RegIrqFlags);
    if ((temp & PAYLOAD_CRC_ERROR_Msk) >> PAYLOAD_CRC_ERROR_MskPos == 1)
    {
      STM_LOGE("ReceiveErr", "Payload CRC Failed");
    }
    else
    {
      vSpi1Write(RegFifoAddrPtr, ucSpi1Read(RegFifoRxCurrentAddr));
      for (size_t i = 0u; i < PAYLOAD_LENGTH; i++)
      {
        *(outData + i) = ucSpi1Read(RegFifo);
        STM_LOGV("Transmit", "data receive[%d]: 0x%x", i, *(outData + i));
        // LORA_GET_REGISTER(RegFifoAddrPtr);
      }
    }
  }

  /* CLEAR RX_DONE FLAG */
  vSpi1Write(RegIrqFlags, RX_DONE_Msk | PAYLOAD_CRC_ERROR_Msk);
}