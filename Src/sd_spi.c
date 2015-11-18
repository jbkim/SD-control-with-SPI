
#include "sd_spi.h"

extern SPI_HandleTypeDef hspi1;

uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;        /*<! Value of Timeout when SPI communication fails */

extern void MX_SPI1_Init(void);

// static void               SPIx_Init(void);
static void               SPIx_Write(uint8_t Value);
static uint8_t           SPIx_Read(void);
static void               SPIx_Error (void);

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @retval None
  */
void SD_IO_Init(void)
{
  uint8_t counter = 0;

// GPIO init is performed in main
#if 0
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* SD_CS_GPIO Periph clock enable */
  SD_CS_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  gpioinitstruct.Pin    = SD_CS_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_PULLUP;
//  gpioinitstruct.Speed  = GPIO_SPEED_HIGH;
  gpioinitstruct.Speed  = GPIO_SPEED_FAST;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);
#endif

  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
// SPIx_init is performed in main
  // SPIx_Init();

  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }

}

/**
  * @brief  Writes a byte on the SD.
  * @param  Data: byte to send.
  * @retval None
  */
void SD_IO_WriteByte(uint8_t Data)
{
  /* Send the byte */
  SPIx_Write(Data);
}
 
/**
  * @brief  Reads a byte from the SD.
  * @retval The received byte.
  */
uint8_t SD_IO_ReadByte(void)
  {
  uint8_t data = 0;
  
  /* Get the received data */
  data = SPIx_Read();

  /* Return the shifted data */
  return data;
}

#if 0
HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
  uint32_t counter = 0x00;
  uint8_t frame[6] = {0};

  /* Prepare Frame to send */
  frame[0] = (Cmd | 0x40);         /* Construct byte 1 */
  frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  frame[3] = (uint8_t)(Arg >> 8);  /* Construct byte 4 */
  frame[4] = (uint8_t)(Arg);       /* Construct byte 5 */
  frame[5] = (Crc);                /* Construct byte 6 */
  
  /* SD chip select low */
  SD_CS_LOW();
    
  /* Send Frame */
  for (counter = 0; counter < 6; counter++)
  {
    SD_IO_WriteByte(frame[counter]); /* Send the Cmd bytes */
  }

  if(Response != SD_NO_RESPONSE_EXPECTED)
  {
    return SD_IO_WaitResponse(Response);
  }  
  
  return HAL_OK;
}

#else
/**
  * @brief  Sends 5 bytes command to the SD card and get response
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @param  Response: Expected response from the SD card
  * @retval HAL_StatusTypeDef HAL Status
  */

// HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
uint8_t SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
  uint32_t counter = 0x00;
  uint8_t frame[6] = {0};
  uint8_t ret;

// Add delay routine 
  SD_CS_HIGH();
  SD_IO_WaitResponse(0xFF);
  SD_IO_WaitResponse(0xFF);
  SD_IO_WaitResponse(0xFF);
// 

  /* Prepare Frame to send */	
  frame[0] = (Cmd | 0x40);         /* Construct byte 1 */
  frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  frame[3] = (uint8_t)(Arg >> 8);  /* Construct byte 4 */
  frame[4] = (uint8_t)(Arg);       /* Construct byte 5 */
  frame[5] = (Crc);                /* Construct byte 6 */
  
  /* SD chip select low */
  SD_CS_LOW();

  /* Send Frame */
  for (counter = 0; counter < 6; counter++)
  {
    SD_IO_WriteByte(frame[counter]); /* Send the Cmd bytes */
  }

  if(Response != SD_NO_RESPONSE_EXPECTED)
  {
    // wait for the response
    ret = SD_IO_WaitResponse(Response);
  }  

  return ret;
}
#endif

/**
  * @brief  Waits response from the SD card
  * @param  Response: Expected response from the SD card
  * @retval HAL_StatusTypeDef HAL Status
  */
#if 0  
HAL_StatusTypeDef SD_IO_WaitResponse(uint8_t Response)
{
  uint32_t timeout = 0xFFFF;

  /* Check if response is got or a timeout is happen */
  while ((SD_IO_ReadByte() != Response) && timeout)
  {
    timeout--;
  }

  if (timeout == 0)
  {
    /* After time out */
    return HAL_TIMEOUT;
  }   
  else
  {
    /* Right response got */
    return HAL_OK;
  }
}
#else
// Change return value 
uint8_t SD_IO_WaitResponse(uint8_t Response)
{
  uint32_t timeout = 0xFFFF;
  uint8_t ret;

  /* Check if response is got or a timeout is happen */
  while (timeout)
  {
    timeout--;
    ret = SD_IO_ReadByte();
    if (ret != 0xff) break;
  }

  if (timeout == 0)
  {
    /* After time out */
    return HAL_TIMEOUT;
  }   
  else
  {
    /* Right response got */
    return ret;
  }
}

#endif  


/**
  * @brief  Sends dummy byte with CS High
  * @retval None
  */
void SD_IO_WriteDummy(void)
{
  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send Dummy byte 0xFF */
  SD_IO_WriteByte(SD_DUMMY_BYTE);
}


/**
  * @brief  SPI Read 4 bytes from device
  * @retval Read data
*/
#if 0
static uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }

  return readvalue;
}

#else

static uint8_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t readvalue = 0;
  uint8_t writevalue = 0xFF;
  
  status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }

  return readvalue;
}

#endif

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hspi1, (uint8_t*) &Value, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hspi1);

  printf("\r\n\r\n SPIx_Error");
  /* Re-Initiaize the SPI communication BUS */
  // SPIx_Init();
  MX_SPI1_Init();
}

