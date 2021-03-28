#ifndef __LORA_REWORK_H
#define __LORA_REWORK_H

#define LORA_WORKING_FREQUENCY (433E6)

HAL_StatusTypeDef LoRaInit(long freq);
HAL_StatusTypeDef LoRaReadRegister(uint8_t address, uint8_t *retData);
HAL_StatusTypeDef LoRaSingleTransfer(uint8_t address, uint8_t sendData, uint8_t *retData);

#endif /* !__LORA_REWORK_H */