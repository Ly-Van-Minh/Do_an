/*
 * lora.h
 *
 *  Created on: Mar 16, 2021
 *      Author: Ly Van Minh
 */

/*****************************************************************************/
#ifndef __LORA_H
#define __LORA_H

#include <stdint.h>


/* Define Long Range Mode*/
#define LORA_MODE           1u   /* LoRaTM Mode */
#define FSK_OOK_MODE        0u   /* FSK/OOK Mode */
/* Define Node Address */
#define NODE1_ADDRESS       0x11 /* Address node 1 */
#define NODE2_ADDRESS       0x22 /* Address node 2 */
#define NODE3_ADDRESS       0x33 /* Address node 3 */

#define PAYLOAD_LENGHT      3u   /* Payload Lenght */
#define RELAY_ON            0xAA
#define RELAY_OFF           0xBB

#define BANDWIDTH_7K8       0u
#define BANDWIDTH_10K4      1u
#define BANDWIDTH_15K6      2u
#define BANDWIDTH_20K8      3u
#define BANDWIDTH_31K25     4u
#define BANDWIDTH_41K7      5u
#define BANDWIDTH_62K5      6u
#define BANDWIDTH_125K      7u
#define BANDWIDTH_250K      8u
#define BANDWIDTH_500K      9u

#define CODING_RATE_4_5     1u
#define CODING_RATE_4_6     2u
#define CODING_RATE_4_7     3u
#define CODING_RATE_4_8     4u

#define EXPLICIT_HEADER     0u
#define IMPLICIT_HEADER     0u

/* Define Device Mode */
#define SLEEP_MODE          0u   /* Sleep */
#define STDBY_MODE          1u   /* Standby */
#define FSTX_MODE           2u   /* Frequency synthesis TX */
#define TX_MODE             3u   /* Transmit */
#define FSRX_MODE           4u   /* Frequency synthesis RX */
#define RXCONTINUOUS_MODE   5u   /* Receive continuous */
#define RXSINGLE_MODE       6u   /* Receive single */
#define CAD_MODE            7u   /* Channel activity detection */
/****************** Begin define registers of module Lora ********************/

/* Registers use general for LORA and FSK/OOK Mode ------------------------- */
#define RegFifo         0x00u   /* FIFO read/write access */
#define RegOpMode       0x01u   /* Operating mode & LoRaTM / FSK selection */
#define RegFrfMsb       0x06u   /* RF Carrier Frequency, Most Significant Bits */
#define RegFrfMid       0x07u   /* RF Carrier Frequency, Intermediate Bits */
#define RegFrfLsb       0x08u   /* RF Carrier Frequency, Least Significant Bits */
#define RegPaConfig     0x09u   /* PA selection and Output Power control */
#define RegPaRamp       0x0Au   /* Control of PA ramp time, low phase noise PLL */
#define RegOcp          0x0Bu   /* Over Current Protection control */
#define RegLna          0x0Cu   /* LNA settings */
#define RegDioMapping1  0x40u   /* Mapping of pins DIO0 to DIO3 */
#define RegDioMapping2  0x41u   /* Mapping of pins DIO4 and DIO5, ClkOut frequency */
#define RegVersion      0x42u   /* Semtech ID relating the silicon revision */
#define RegTcxo         0x4Bu   /* TCXO or XTAL input setting */
#define RegPaDac        0x4Du   /* Higher power settings of the PA */
#define RegFormerTemp   0x5Bu   /* Stored temperature during the former IQ Calibration */
#define RegAgcRef       0x61u   /* Adjustment of the AGC thresholds Reference */
#define RegAgcThresh1   0x62u   /* Adjustment of the AGC thresholds 1 */
#define RegAgcThresh2   0x63u   /* Adjustment of the AGC thresholds 2 */
#define RegAgcThresh3   0x64u   /* Adjustment of the AGC thresholds 3 */
#define RegPll          0x70u   /* Control of the PLL bandwidth */

#if LORA_MODE   /* Registers for LORA Mode */
    #define RegFifoAddrPtr          0x0Du   /* FIFO SPI pointer */
    #define RegFifoTxBaseAddr       0x0Eu   /* Start Tx data */
    #define RegFifoRxBaseAddr       0x0Fu   /* Start Rx data */
    #define RegFifoRxCurrentAddr    0x10u   /* Start address of last packet received */
    #define RegIrqFlagsMask         0x11u   /* Optional IRQ flag mask */
    #define RegIrqFlags             0x12u   /* IRQ flags */
    #define RegRxNbBytes            0x13u   /* Number of received bytes */
    #define RegRxHeaderCntValueMsb  0x14u   /* Number of valid headers received MSB */
    #define RegRxHeaderCntValueLsb  0x15u   /* Number of valid headers received LSB */
    #define RegRxPacketCntValueMsb  0x16u   /* Number of valid packets received MSB */
    #define RegRxPacketCntValueLsb  0x17u   /* Number of valid packets received LSB */
    #define RegModemStat            0x18u   /* Live LoRaTM modem status */
    #define RegPktSnrValue          0x19u   /* Espimation of last packet SNR */
    #define RegPktRssiValue         0x1Au   /* RSSI of last packet */
    #define RegRssiValue            0x1Bu   /* Current RSSI */
    #define RegHopChannel           0x1Cu   /* FHSS start channel */
    #define RegModemConfig1         0x1Du   /* Modem PHY config 1 */
    #define RegModemConfig2         0x1Eu   /* Modem PHY config 2 */
    #define RegSymbTimeoutLsb       0x1Fu   /* Receiver timeout value */
    #define RegPreambleMsb          0x20u   /* Size of preamble MSB */
    #define RegPreambleLsb          0x21u   /* Size of preamble LSB */
    #define RegPayloadLength        0x22u   /* LoRaTM payload length */
    #define RegMaxPayloadLength     0x23u   /* LoRaTM maximum payload length */
    #define RegHopPeriod            0x24u   /* FHSS Hop period */
    #define RegFifoRxByteAddr       0x25u   /* Address of last byte written in FIFO */
    #define RegModemConfig3         0x26u   /* Modem PHY config 3 */
    #define RegFeiMsb               0x28u   /* Estimated frequency error MSB */
    #define RegFeiMid               0x29u   /* Estimated frequency error MID */
    #define RegFeiLsb               0x2Au   /* Estimated frequency error LSB */
    #define RegRssiWideband         0x2Cu   /* Wideband RSSI measurement */
    #define RegDetectOptimize       0x31u   /* LoRa detection Optimize for SF6 */
    #define RegInvertIQ             0x33u   /* Invert LoRa I and Q signals */
    #define RegDetectionThreshold   0x37u   /* LoRa detection threshold for SF6 */
    #define RegSyncWord             0x39u   /* LoRa Sync Word */
#else   /* Registers for FSK/OOK Mode */
    #define RegBitrateMsb       0x02u
    #define RegBitrateLsb       0x03u
    #define RegFdevMsb          0x04u
    #define RegFdevLsb          0x05u
    #define RegRxConfig         0x0Du   /* AFC, AGC, ctrl */
    #define RegRssiConfig       0x0Eu   /* RSSI */
    #define RegRssiCollision    0x0Fu   /* RSSI Collision detector */
    #define RegRssiThresh       0x10u   /* RSSI Threshold control */
    #define RegRssiValue        0x11u   /* RSSI value in dBm */
    #define RegRxBw             0x12u   /* Channel Filter BW Control */
    #define RegAfcBw            0x13u   /* AFC Channel Filter BW */
    #define RegOokPeak          0x14u   /* OOK demodulator */
    #define RegOokFix           0x15u   /* Threshold of the OOK demod */
    #define RegOokAvg           0x16u   /* Average of the OOK demod */
    #define Reserved17          0x17u   /* - */
    #define Reserved18          0x18u   /* - */
    #define Reserved19          0x19u   /* - */
    #define RegAfcFei           0x1Au   /* AFC and FEI control */
    #define RegAfcMsb           0x1Bu   /* Frequency correction value of the AFC MSB */
    #define RegAfcLsb           0x1Cu   /* Frequency correction value of the AFC LSB */
    #define RegFeiMsb           0x1Du   /* Value of the calculated frequency error MSB */
    #define RegFeiLsb           0x1Eu   /* Value of the calculated frequency error LSB */
    #define RegPreambleDetect   0x1Fu   /* Settings of the Preamble Detector */
    #define RegRxTimeout1       0x20u   /* Timeout Rx request and RSSI */
    #define RegRxTimeout2       0x21u   /* Timeout RSSI and PayloadReady */
    #define RegRxTimeout3       0x22u   /* Timeout RSSI and SyncAddress */
    #define RegRxDelay          0x23u   /* Delay between Rx cycles */
    #define RegOsc              0x24u   /* RC Oscillators Settings, CLKOUT frequency */
    #define RegPreambleMsb      0x25u   /* Preamble length MSB */
    #define RegPreambleLsb      0x26u   /* Preamble length LSB */
    #define RegSyncConfig       0x27u   /* Sync Word Recognition control */
    #define RegSyncValue1       0x28u   /* Sync Word bytes 1 */
    #define RegSyncValue2       0x29u   /* Sync Word bytes 2 */
    #define RegSyncValue3       0x2Au   /* Sync Word bytes 3 */
    #define RegSyncValue4       0x2Bu   /* Sync Word bytes 4 */
    #define RegSyncValue5       0x2Cu   /* Sync Word bytes 5 */
    #define RegSyncValue6       0x2Du   /* Sync Word bytes 6 */
    #define RegSyncValue7       0x2Eu   /* Sync Word bytes 7 */
    #define RegSyncValue8       0x2Fu   /* Sync Word bytes 8 */
    #define RegPacketConfig1    0x30u   /* Packet mode settings 1 */
    #define RegPacketConfig2    0x31u   /* Packet mode settings 2 */
    #define RegPayloadLength    0x32u   /* Payload length setting */
    #define RegNodeAdrs         0x33u   /* Node address */
    #define RegBroadcastAdrs    0x34u   /* Broadcast address */
    #define RegFifoThresh       0x35u   /* Fifo threshold, Tx start condition */
    #define RegSeqConfig1       0x36u   /* Top level Sequencer settings 1 */
    #define RegSeqConfig2       0x37u   /* Top level Sequencer settings 2 */
    #define RegTimerResol       0x38u   /* Timer 1 and 2 resolution control */
    #define RegTimer1Coef       0x39u   /* Timer 1 setting */
    #define RegTimer2Coef       0x3Au   /* Timer 2 setting */
    #define RegImageCal         0x3Bu   /* Image calibration engine control */
    #define RegTemp             0x3Cu   /* Temperature Sensor value */
    #define RegLowBat           0x3Du   /* Low Battery Indicator Settings */
    #define RegIrqFlags1        0x3Eu   /* Status register: PLL Lock state, Timeout, RSSI */
    #define RegIrqFlags2        0x3Fu   /* Status register: FIFO handling flags, Low Battery */
    #define RegPllHop           0x44u   /* Control the fast frequency hopping mode */
    #define RegBitRateFrac      0x5Du   /* Fractional part in the Bit Rate division ratio */
#endif
/********************* End define registers of modele Lora *******************/

/**************************** Private functions ******************************/
/* Init Lora functions */
void vUart1Transmit(uint8_t* pcString);
void vSpi1Write(uint8_t ucAddress, uint8_t ucData);
uint8_t ucSpi1Read(uint8_t ucAddress);
uint8_t ucReadFifo(void);
void vWriteFifo(uint8_t ucData);
void vLongRangeModeInit(uint8_t ucLongRangeMode);
void vAccessSharedRegInit(uint8_t ucAccessSharedReg);
void vLowFrequencyModeOnInit(uint8_t ucLowFrequencyModeOn);
void vModeInit(uint8_t ucMode);
void vFrfInit(unsigned int uiFrf);
void vPaSelectInit(uint8_t ucPaSelect);
void vMaxPowerInit(uint8_t ucMaxPower);
void vOutputPowerInit(uint8_t ucOutputPower);
void vPaRampInit(uint8_t ucRegPaRamp);
void vOcpOnInit(uint8_t ucOcpOn);
void vOcpTrim(uint8_t ucOcpTrim);
void vLnaGainInit(uint8_t ucLnaGain);
void vLnaBoostLfInit(uint8_t ucLnaBoostLf);
void vLnaBoostHfInit(uint8_t ucLnaBoostHf);
uint8_t ucFifoAddrPtrRead(void);
void vFifoAddrPtrWrite(uint8_t ucFifoAddrPtr);
void vFifoTxBaseAddrInit(uint8_t ucFifoTxBaseAddr);
void vFifoRxBaseAddrInit(uint8_t ucFifoRxBaseAddr);
uint8_t ucFifoRxCurrentAddrRead(void);
// void vRxTimeoutMaskInit(uint8_t ucRxTimeoutMask);
// void vRxDoneMaskInit(uint8_t ucRxDoneMask);
// void vPayloadCrcErrorMaskInit(uint8_t ucPayloadCrcErrorMask);
// void vValidHeaderMaskInit(uint8_t ucValidHeaderMask);
// void vTxDoneMaskInit(uint8_t ucTxDoneMask);
// void vCadDoneMaskInit(uint8_t ucCadDoneMask);
// void vFhssChangeChannelMaskInit(uint8_t ucFhssChangeChannelMask);
// void vCadDetectedMaskInit(uint8_t ucCadDetectedMask);
void vIrqFlagsMaskInit(uint8_t ucIrqFlagsMask);
uint8_t ucIrqFlagsRead(void);
void vIrqFlagsClear(uint8_t ucIrqFlags);
uint8_t ucFifoRxBytesNbRead(void);
uint16_t usValidHeaderCntRead(void);
uint16_t usValidPacketCntRead(void);
uint8_t ucRxCodingRateRead(void);
uint8_t ucModemStatusRead(void);
uint8_t ucPacketRssiRead(void);
uint8_t ucRssiRead(void);
uint8_t ucPllTimeoutRead(void);
uint8_t ucCrcOnPayloadread(void);
uint8_t ucFhssPresentChannelRead(void);
void vBandWidthInit(uint8_t ucBandWidth);
void vCodingRateInit(uint8_t ucCodingRate);
void vImplicitHeaderModeOnInit(uint8_t ucHeaderMode);
void vSpreadingFactor(uint8_t ucSpreadingFactor);
void vTxContinuousModeInit(uint8_t ucTxContinuousMode);
void vRxPayloadCrcOnInit(uint8_t ucRxPayloadCrcOn);
void vSymbTimeoutInit(uint16_t ucSymbTimeout);
void vPreambleLengthInit(uint16_t ucPreambleLength);
void vPayloadLengthInit(uint8_t ucPayloadLength);
void vPayloadMaxLengthInit(uint8_t ucPayloadMaxLength);
void vFreqHoppingPeriodInit(uint8_t ucFreqHoppingPeriod);
uint8_t ucFifoRxByteAddrPtr(void);
void vLowDataRateOptimizeInit(uint8_t ucLowDataRateOptimize);
void vAgcAutoOnInit(uint8_t ucAgcAutoOn);
unsigned int uiFreqError(void);
uint8_t ucRssiWidebandInit(void);
void vDetectionOptimizeInit(uint8_t ucDetectionOptimize);
void vInvertIQInit(uint8_t ucInvertIQ);
void vDetectionThresholdInit(uint8_t ucDetectionThreshold);
void vSyncWordInit(uint8_t ucSyncWord);
void vDio0MappingInit(uint8_t ucDio0Mapping);
void vDio1MappingInit(uint8_t ucDio1Mapping);
void vDio2MappingInit(uint8_t ucDio2Mapping);
void vDio3MappingInit(uint8_t ucDio3Mapping);
void vDio4MappingInit(uint8_t ucDio4Mapping);
void vDio5MappingInit(uint8_t ucDio5Mapping);
void vMapPreambleDetectInit(uint8_t ucMapPreambleDetect);
uint8_t ucVersionRead(void);
void vTcxoInputOnInit(uint8_t ucTcxoInputOn);
void vPaDacInit(uint8_t ucPaDac);
void vFormerTempInit(uint8_t ucFormerTemp);
void vAgcReferenceLevelInit(uint8_t ucAgcReferenceLevel);
void vAgcStep1Init(uint8_t ucAgcStep1);
void vAgcStep2Init(uint8_t ucAgcStep2);
void vAgcStep3Init(uint8_t ucAgcStep3);
void vAgcStep4Init(uint8_t ucAgcStep4);
void vAgcStep5Init(uint8_t ucAgcStep5);
void vRegPllInit(uint8_t ucRegPll);

/************************** End private functions ****************************/
#endif /* !_LORA_H_ */
/****************************** END OF FILE **********************************/

