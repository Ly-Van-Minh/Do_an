/*
 * lora.h
 *
 *  Created on: Mar 16, 2021
 *      Author: Ly Van Minh
 */

/* -------------------------------------------------------------------------- */
/*                                   LORA.H                                   */
/* -------------------------------------------------------------------------- */
#ifndef __LORA_H
#define __LORA_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "misc.h"
#include "main.h"

/* Define Long Range Mode*/
#define USE_LORA_MODE               (1u)

#define LORA_MAX_DELAY              (0xFFFF)

#define DELAY_SPI                   3u
#define SPI1_READ                   0x7Fu
#define SPI1_WRITE                  0x80u

#define LORA_GET_REGISTER(req)                                                                \
    do                                                                                        \
    {                                                                                         \
        u8 ret = ucSpi1Read(req);                                                        \
        STM_LOGV("LORA", "%s: 0x%.2x - " BYTE_TO_BINARY_PATTERN, #req, ret, BYTE_TO_BINARY(ret)); \
    } while (0)

#define LORA_SYNC_WORD              0x12u
#define PREAMBLE_LENGTH             0x0008u

#define TX_NORMAL_MODE              0u
#define RX_TIMEOUT                  0x0064u
#define PAYLOAD_LENGTH              10u   /* Payload Lenght */
#define PAYLOAD_MAX_LENGTH          0xFFu
#define FREQ_HOPPING_PERIOD         0u
#define AGC_AUTO                    0u
#define LOW_DATA_RATE_OPTIMIZE      1u
#define LORA_DETECTION_OPTIMIZE     5u
#define INVERT_IQ                   0u
#define LORA_DETECTION_THRESHOLD    0x0Cu 
#define AGC_REFERENCE               0x19u
#define AGC_STEP1                   0x0Cu
#define AGC_STEP2                   0x04u
#define AGC_STEP3                   0x0Bu
#define AGC_STEP4                   0x0Cu
#define AGC_STEP5                   0x0Cu
#define PLL_BANDWIDTH               0x03u
#define PREAMBBLE_DETECT_INTERRUPT  1u
#define RX_DONE                     0u
#define TX_DONE                     1u
#define CAD_DONE                    2u
#define PA_DAC                      0x07u

#define ACCESS_LORA_REGISTERS       0u
#define ACCESS_LOW_FREQUENCY_MODE   1u
#define RF_FREQUENCY                0x6C8000u
#define PA_BOOST                    1u   /* Selects PA output pin:  PA_BOOST pin */
#define MAX_POWER                   7u
#define OUTPUT_POWER                15u
#define PA_RAMP                     8u
#define OCP_ON                      1u   /* Enables overload current protection (OCP) for PA */
#define OCP_TRIM                    27u  /* Trimming of OCP current */
#define G1                          1u   /* LNA gain setting: G1 = maximum gain */
#define LNA_BOOST_LF                0u   /* Default LNA current */
#define LNA_BOOST_HF                3u   /* Boost on, 150% LNA current */
#define FIFO_TX_BASE_ADDR           0x80u /* Base address in FIFO data buffer for TX modulator */
#define FIFO_RX_BASE_ADDR           0u  /* Base address in FIFO data buffer for RX demodulator */
#define IRQ_FLAGS_MASK              0u
#define RX_TIMEOUT_FLAG             0x80u
#define RX_DONE_FLAG                0x40u
#define PAYLOAD_CRC_ERROR_FLAG      0x20u
#define VALID_HEADER_FLAG           0x10u
#define TX_DONE_FLAG                0x08u
#define CAD_DONE_FLAG               0x04u
#define FHSS_CHANGE_CHANNEL_FLAG    0x02u
#define CAD_DETECTED_FLAG           0x01u

#define BIT_VALUE_1   (0b1)
#define BIT_VALUE_2   (0b11)
#define BIT_VALUE_3   (0b111)
#define BIT_VALUE_4   (0b1111)
#define BIT_VALUE_5   (0b11111)
#define BIT_VALUE_6   (0b111111)
#define BIT_VALUE_7   (0b1111111)
#define BIT_VALUE_8   (0b11111111)


/* -------------------------------------------------------------------------- */
/*                               LoRa Structure                               */
/* -------------------------------------------------------------------------- */
//typedef struct{
//    LoRaModulationModeTypeDef_t mode;
//    u32 frequency;
//    u16 preamble;
//    LoRaCodingRateTypeDef_t codingRate;
//    LoRaHeaderTypeDef_t headerType;
//    LoRaSpredingFactorTypeDef_t spreadingFactor;
//    LoRaCrcModeTypeDef_t crcMode;
//    LoRaOsscSourceTypeDef_t osscSource;
//} LoRaInitTypeDef_t;

/* -------------------------------------------------------------------------- */
/*                             Bit Mask Definition                            */
/* -------------------------------------------------------------------------- */
/* -------------------------------- RegOpMode ------------------------------- */
#define LONG_RANGE_MODE_MskPos         (7u)
#define LONG_RANGE_MODE_Msk            (BIT_VALUE_1 << LONG_RANGE_MODE_MskPos)
#define ACCESS_SHARED_REG_MskPos       (6u)
#define ACCESS_SHARED_REG_Msk          (BIT_VALUE_1 << ACCESS_SHARED_REG_MskPos)
#define LOW_FREQUENCY_MODE_ON_MskPos   (3u)
#define LOW_FREQUENCY_MODE_ON_Msk      (BIT_VALUE_1 << LOW_FREQUENCY_MODE_ON_MskPos)
#define MODE_MskPos                    (0u)
#define MODE_Msk                       (BIT_VALUE_3 << MODE_MskPos)
/* -------------------------------- RegFrMsb -------------------------------- */
#define FREQUENCY_MSB_MskPos           (0u)
#define FREQUENCY_MSB_Msk              (BIT_VALUE_8 << FREQUENCY_MSB_MskPos))
/* -------------------------------- RegFrMid -------------------------------- */
#define FREQUENCY_MID_MskPos           (0u)
#define FREQUENCY_MID_Msk              (BIT_VALUE_8 << FREQUENCY_MID_MskPos))
/* -------------------------------- RegFrLsb -------------------------------- */
#define FREQUENCY_LSB_MskPos           (0u)
#define FREQUENCY_LSB_Msk              (BIT_VALUE_8 << FREQUENCY_LSB_MskPos))
/* ------------------------------- RegIrqFlags ------------------------------ */
#define RX_TIMEOUT_MskPos              (7u)
#define RX_TIMEOUT_Msk                 (BIT_VALUE_1 << RX_TIMEOUT_MskPos)
#define RX_DONE_MskPos                 (6u)
#define RX_DONE_Msk                    (BIT_VALUE_1 << RX_DONE_MskPos)
#define PAYLOAD_CRC_ERROR_MskPos       (5u)
#define PAYLOAD_CRC_ERROR_Msk          (BIT_VALUE_1 << PAYLOAD_CRC_ERROR_MskPos)
#define VALID_HEADER_MskPos            (4u)
#define VALID_HEADER_Msk               (BIT_VALUE_1 << VALID_HEADER_MskPos)
#define TX_DONE_MskPos                 (3u)
#define TX_DONE_Msk                    (BIT_VALUE_1 << TX_DONE_MskPos)
#define CAD_DONE_MskPos                (2u)
#define CAD_DONE_Msk                   (BIT_VALUE_1 << CAD_DONE_MskPos)
#define FHSS_CHANGE_CHANNEL_MskPos     (1u)
#define FHSS_CHANGE_CHANNEL_Msk        (BIT_VALUE_1 << FHSS_CHANGE_CHANNEL_MskPos)
#define CAD_DETECTED_MskPos            (0u)
#define CAD_DETECTED_Msk               (BIT_VALUE_1 << CAD_DETECTED_MskPos)
/* ----------------------------- RegPktRssiValue ---------------------------- */
#define PACKKET_RSSI_MskPos            (0u)
#define PACKKET_RSSI_Msk               (BIT_VALUE_8 << PACKKET_RSSI_MskPos)
/* ------------------------------ RegRssiValue ------------------------------ */
#define RSSI_MskPos                    (0u)
#define RSSI_Msk                       (BIT_VALUE_8 << RSSI_MskPos)
/* ----------------------------- RegModemConfig1 ---------------------------- */
#define BANDWIDTH_MskPos               (4u)
#define BANDWIDTH_Msk                  (BIT_VALUE_4 << BANDWIDTH_MskPos)
#define CODING_RATE_MskPos             (1u)
#define CODING_RATE_Msk                (BIT_VALUE_3 << CODING_RATE_MskPos)
#define IMPLICIT_HEADER_MODE_ON_MskPos (0u)
#define IMPLICIT_HEADER_MODE_ON_Msk    (BIT_VALUE_1 << IMPLICIT_HEADER_MODE_ON_MskPos)
/* ----------------------------- RegModemConfig2 ---------------------------- */
#define SPREADING_FACTOR_MskPos        (4u)
#define SPREADING_FACTOR_Msk           (BIT_VALUE_4 << SPREADING_FACTOR_MskPos)
#define TX_CONTINUOUS_MODE_MskPos      (3u)
#define TX_CONTINUOUS_MODE_Msk         (BIT_VALUE_1 << TX_CONTINUOUS_MODE_MskPos)
#define RX_PAYLOAD_CRC_ON_MskPos       (2u)
#define RX_PAYLOAD_CRC_ON_Msk          (BIT_VALUE_1 << RX_PAYLOAD_CRC_ON_MskPos)
#define SYMB_TIMEOUT_9_8_MskPos        (0)
#define SYMB_TIMEOUT_9_8_Msk           (BIT_VALUE_2 << SYMB_TIMEOUT_9_8_MskPos)
/* ---------------------------- RegSymbTimeoutLsb --------------------------- */
#define SYMB_TIMEOUT_7_0_MskPos        (0u)
#define SYMB_TIMEOUT_7_0_Msk           (BIT_VALUE_8 << SYMB_TIMEOUT_7_0_MskPos)
/* ----------------------------- RegPreambleMsb ----------------------------- */
#define PREAMBLE_LENGTH_MSB_MskPos     (0u)
#define PREAMBLE_LENGTH_MSB_Msk        (BIT_VALUE_8<< PREAMBLE_LENGTH_MSB_MskPos)
/* ----------------------------- RegPreambleLsb ----------------------------- */
#define PREAMBLE_LENGTH_LSB_MskPos     (0u)
#define PREAMBLE_LENGTH_LSB_Msk        (BIT_VALUE_8 << PREAMBLE_LENGTH_LSB_MskPos)
/* ---------------------------- RegPayloadLength ---------------------------- */
#define PAYLOAD_LENGTH_MskPos          (0u)
#define PAYLOAD_LENGTH_Msk             (BIT_VALUE_8 << PAYLOAD_LENGTH_MskPos)
/* --------------------------- RegMaxPayloadLength -------------------------- */
#define PAYLOAD_MAX_LENGTH_MksPos      (0u)
#define PAYLOAD_MAX_LENGTH_Mks         (BIT_VALUE_8 << PAYLOAD_MAX_LENGTH_MksPos)

/* Define Device Bandwidth */
typedef enum MODE
{
    FSK_OOK_MODE = 0u,
    LORA_MODE = 1u,
} LoRaModulationModeTypeDef_t;

enum BANDWIDTH
{
    BANDWIDTH_7K8   = 0u,
    BANDWIDTH_10K4  = 1u,
    BANDWIDTH_15K6  = 2u,
    BANDWIDTH_20K8  = 3u,
    BANDWIDTH_31K25 = 4u,
    BANDWIDTH_41K7  = 5u,
    BANDWIDTH_62K5  = 6u,
    BANDWIDTH_125K  = 7u, /* DEFAULT */
    BANDWIDTH_250K  = 8u,
    BANDWIDTH_500K  = 9u,
};

/* Define Device Coding rate */
typedef enum CODING_RATE
{
    CODING_RATE_4_5 = 1u, /* DEFAULT */
    CODING_RATE_4_6 = 2u,
    CODING_RATE_4_7 = 3u,
    CODING_RATE_4_8 = 4u,
} LoRaCodingRateTypeDef_t;

/* Define Device Header type */
typedef enum HEADER_TYPE
{
    EXPLICIT_HEADER = 0u, /* DEFAULT */
    IMPLICIT_HEADER = 1u,
} LoRaHeaderTypeDef_t;

/* Define Device Spreding Factor */
typedef enum SPREADING_FACTOR
{
    SPREADING_FACTOR_6_64    = 6u,
    SPREADING_FACTOR_7_128   = 7u, /* DEFAULT */
    SPREADING_FACTOR_8_256   = 8u,
    SPREADING_FACTOR_9_512   = 9u,
    SPREADING_FACTOR_10_1024 = 10u,
    SPREADING_FACTOR_11_2048 = 11u,
    SPREADING_FACTOR_12_4096 = 12u,
} LoRaSpredingFactorTypeDef_t;

/* Define Device Mode */
typedef enum 
{
    SLEEP_MODE        = 0u,           /* Sleep */
    STDBY_MODE        = 1u,           /* Standby */
    FSTX_MODE         = 2u,           /* Frequency synthesis TX */
    TX_MODE           = 3u,           /* Transmit */
    FSRX_MODE         = 4u,           /* Frequency synthesis RX */
    RXCONTINUOUS_MODE = 5u,           /* Receive continuous */
    RXSINGLE_MODE     = 6u,           /* Receive single */
    CAD_MODE          = 7u,           /* Channel activity detection */
    UNKNOWN,
} LoRaWorkingMode_t;

/* Define Device Receive Mode */
typedef enum CRC_MODE
{
    CRC_ENABLE = 1u,
    CRC_DISABLE = 0u,
} LoRaCrcModeTypeDef_t;

typedef enum OSCILLATOR_SOURCE
{
    XTAL_INPUT = 0u,
    TCXO_INPUT = 1u,
} LoRaOsscSourceTypeDef_t;

/* -------------------------------------------------------------------------- */
/*                    Begin define registers of module Lora                   */
/* -------------------------------------------------------------------------- */

/* ------------- Registers use general for LORA and FSK/OOK Mode ------------ */
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

/* ------------------------- Registers for LORA Mode ------------------------ */
#if USE_LORA_MODE   
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
/* ----------------------- /Registers for FSK/OOK Mode ---------------------- */
#else   
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
/* ------------------- End define registers of modele Lora ------------------ */

/* -------------------------------------------------------------------------- */
/*                             Function Prototype                             */
/* -------------------------------------------------------------------------- */
void vSpi1Write(u8 ucAddress, u8 ucData);
u8 ucSpi1Read(u8 ucAddress);
// u8 ucReadFifo(void);
// void vWriteFifo(u8 ucData);
void vLongRangeModeInit(u8 ucLongRangeMode);
void vAccessSharedRegInit(u8 ucAccessSharedReg);
void vLowFrequencyModeOnInit(u8 ucLowFrequencyModeOn);
void vModeInit(u8 ucMode);
void vFrfInit(unsigned int uiFrf);
void vPaSelectInit(u8 ucPaSelect);
void vMaxPowerInit(u8 ucMaxPower);
void vOutputPowerInit(u8 ucOutputPower);
void vPaRampInit(u8 ucRegPaRamp);
void vOcpOnInit(u8 ucOcpOn);
void vOcpTrimInit(u8 ucOcpTrim);
void vLnaGainInit(u8 ucLnaGain);
void vLnaBoostLfInit(u8 ucLnaBoostLf);
void vLnaBoostHfInit(u8 ucLnaBoostHf);
// u8 ucFifoAddrPtrRead(void);
// void vFifoAddrPtrWrite(u8 ucFifoAddrPtr);
void vFifoTxBaseAddrInit(u8 ucFifoTxBaseAddr);
void vFifoRxBaseAddrInit(u8 ucFifoRxBaseAddr);
u8 ucFifoRxCurrentAddrRead(void);
// void vRxTimeoutMaskInit(u8 ucRxTimeoutMask);
// void vRxDoneMaskInit(u8 ucRxDoneMask);
// void vPayloadCrcErrorMaskInit(u8 ucPayloadCrcErrorMask);
// void vValidHeaderMaskInit(u8 ucValidHeaderMask);
// void vTxDoneMaskInit(u8 ucTxDoneMask);
// void vCadDoneMaskInit(u8 ucCadDoneMask);
// void vFhssChangeChannelMaskInit(u8 ucFhssChangeChannelMask);
// void vCadDetectedMaskInit(u8 ucCadDetectedMask);
void vIrqFlagsMaskInit(u8 ucIrqFlagsMask);
u8 ucIrqFlagsRead(void);
void vIrqFlagsClear(u8 ucIrqFlags);
u8 ucFifoRxBytesNbRead(void);
u16 usValidHeaderCntRead(void);
u16 usValidPacketCntRead(void);
u8 ucRxCodingRateRead(void);
u8 ucModemStatusRead(void);
u16 ucPacketRssiRead(void);
u8 ucRssiRead(void);
u8 ucPllTimeoutRead(void);
u8 ucCrcOnPayloadread(void);
u8 ucFhssPresentChannelRead(void);
void vBandWidthInit(u8 ucBandWidth);
void vCodingRateInit(u8 ucCodingRate);
void vImplicitHeaderModeOnInit(u8 ucHeaderMode);
void vSpreadingFactorInit(u8 ucSpreadingFactor);
void vTxContinuousModeInit(u8 ucTxContinuousMode);
void vRxPayloadCrcOnInit(u8 ucRxPayloadCrcOn);
void vSymbTimeoutInit(u16 ucSymbTimeout);
void vPreambleLengthInit(u16 ucPreambleLength);
void vPayloadLengthInit(u8 ucPayloadLength);
void vPayloadMaxLengthInit(u8 ucPayloadMaxLength);
void vFreqHoppingPeriodInit(u8 ucFreqHoppingPeriod);
u8 ucFifoRxByteAddrPtr(void);
void vLowDataRateOptimizeInit(u8 ucLowDataRateOptimize);
void vAgcAutoOnInit(u8 ucAgcAutoOn);
unsigned int uiFreqError(void);
u8 ucRssiWidebandInit(void);
void vDetectionOptimizeInit(u8 ucDetectionOptimize);
void vInvertIQInit(u8 ucInvertIQ);
void vDetectionThresholdInit(u8 ucDetectionThreshold);
void vSyncWordInit(u8 ucSyncWord);
void vDio0MappingInit(u8 ucDio0Mapping);
void vDio1MappingInit(u8 ucDio1Mapping);
void vDio2MappingInit(u8 ucDio2Mapping);
void vDio3MappingInit(u8 ucDio3Mapping);
void vDio4MappingInit(u8 ucDio4Mapping);
void vDio5MappingInit(u8 ucDio5Mapping);
void vMapPreambleDetectInit(u8 ucMapPreambleDetect);
u8 ucVersionRead(void);
void vTcxoInputOnInit(u8 ucTcxoInputOn);
void vPaDacInit(u8 ucPaDac);
void vFormerTempInit(u8 ucFormerTemp);
void vAgcReferenceLevelInit(u8 ucAgcReferenceLevel);
void vAgcStep1Init(u8 ucAgcStep1);
void vAgcStep2Init(u8 ucAgcStep2);
void vAgcStep3Init(u8 ucAgcStep3);
void vAgcStep4Init(u8 ucAgcStep4);
void vAgcStep5Init(u8 ucAgcStep5);
void vPllBandwidth(u8 ucPllBandwidth);
void vLoraInit(void);
void vLoraTransmit(u8 *pcTxBuffer, bool isRepeat);
void vLoraReceive(u8 *pcRxBuffer, bool isRepeat);
u16 usLoRaGetPreamble(void);
u8 usLoRaGetBandwidth(void);
u8 usLoRaGetCodingRate(void);
u8 usLoRaGetHeaderMode(void);
u8 usLoraGetSpreadingFactor(void);
void LoRaTransmit(u8 *data, u8 size, u32 timeoutMs);
void LoRaReceiveCont(u8 *outData, u8 size, u32 timeoutMs);
void vNodeTransmitResponse(void);
void vGateWayTransmitRequest(u32 Timeout);
void vReceiveFifoData(void);

/* -------------------------- End private functions ------------------------- */

#endif /* !_LORA_H_ */

/* -------------------------------------------------------------------------- */
/*                                 END OF FILE                                */
/* -------------------------------------------------------------------------- */
