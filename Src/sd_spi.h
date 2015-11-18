#ifndef _SD_SPI_H
#define _SD_SPI_H

#include "stm32f4xx_hal.h"

#define SD_DUMMY_BYTE            0xFF    
#define SD_NO_RESPONSE_EXPECTED  0x80
#define SPIx_TIMEOUT_MAX 1000

#define SD_CS_PIN                                 GPIO_PIN_4
#define SD_CS_GPIO_PORT                           GPIOA

#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)


void                      SD_IO_Init(void);
// HAL_StatusTypeDef         SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
// HAL_StatusTypeDef         SD_IO_WaitResponse(uint8_t Response);

uint8_t SD_IO_WaitResponse(uint8_t Response);
uint8_t SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);

void                      SD_IO_WriteDummy(void);
void                      SD_IO_WriteByte(uint8_t Data);
uint8_t                   SD_IO_ReadByte(void);

#endif
