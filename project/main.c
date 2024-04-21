#include "stm8l10x.h"

#define RF_RST_PROT     GPIOC
#define RF_RST_PINS     GPIO_Pin_0
#define RF_RST_LOW      GPIO_ResetBits(RF_RST_PROT, RF_RST_PINS)
#define RF_RST_HIGH     GPIO_SetBits(RF_RST_PROT, RF_RST_PINS)

#define NSS_PROT        GPIOB
#define NSS_PINS        GPIO_Pin_4
#define NSS_LOW         GPIO_ResetBits(NSS_PROT, NSS_PINS)
#define NSS_HIGH        GPIO_SetBits(NSS_PROT, NSS_PINS)

#define SCK_PROT        GPIOB
#define SCK_PINS        GPIO_Pin_5
#define SCK_LOW         GPIO_ResetBits(SCK_PROT, SCK_PINS)
#define SCK_HIGH        GPIO_SetBits(SCK_PROT, SCK_PINS)

#define MOSI_PROT       GPIOB
#define MOSI_PINS       GPIO_Pin_6
#define MOSI_LOW        GPIO_ResetBits(SCK_PROT, SCK_PINS)
#define MOSI_HIGH       GPIO_SetBits(SCK_PROT, SCK_PINS)

#define MISO_PROT       GPIOB
#define MISO_PINS       GPIO_Pin_7
#define MISO_DATABIT    GPIO_ReadInputDataBit(MISO_PROT, MISO_PINS);

#define RF_IRQ_DIO0_PROT        GPIOB
#define RF_IRQ_DIO0_PINS        GPIO_Pin_0
#define RF_IRQ_DIO0_DATABIT     GPIO_ReadInputDataBit(RF_IRQ_DIO0_PROT, RF_IRQ_DIO0_PINS);

unsigned char SPI_Inout(unsigned char outdata)
{
  unsigned char Rx_Data = 0;
  while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  SPI_SendData(outdata);
  while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  Rx_Data = SPI_ReceiveData();
  return Rx_Data;
}

void SPI_Sx1278_ReadBuffer(unsigned char addr,unsigned char *buffer,unsigned char size)
{
  unsigned char i;
  NSS_LOW;
  SPI_Inout(addr&0x7F);
  for(i=0;i<size;i++)
  {
    buffer[i]=SPI_Inout(0);
  }
  NSS_HIGH;
}

void SPI_Sx1278_WriteBuffer(unsigned char addr,unsigned char *buffer,unsigned char size)
{
  unsigned char i;
  NSS_LOW;
  SPI_Inout(addr | 0x80);
  for(i=0;i<size;i++)
  {
    SPI_Inout(buffer[i]);
  }
  NSS_HIGH;
}

static void HAL_SPI_SX1278_Config(void)
{
  GPIO_Init(RF_RST_PROT,RF_RST_PINS,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SCK_PROT,SCK_PINS,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(MISO_PROT,MISO_PINS,GPIO_Mode_In_PU_No_IT);
  GPIO_Init(MOSI_PROT,MOSI_PINS,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(NSS_PROT,NSS_PINS,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(RF_IRQ_DIO0_PROT,RF_IRQ_DIO0_PINS,GPIO_Mode_In_PU_No_IT);
  NSS_HIGH;
  RF_RST_HIGH;
}

void HAL_SPI_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI,ENABLE);
  HAL_SPI_SX1278_Config();
  SPI_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);
  SPI_Cmd(ENABLE);
}





void main(void)
{
    static uint8_t version_value = 0;
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
    HAL_SPI_Config();
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);       
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);
    
    SPI_Sx1278_ReadBuffer(0x01,&version_value,1);
    version_value = 8;
    SPI_Sx1278_WriteBuffer(0x01,&version_value,1);      
    SPI_Sx1278_ReadBuffer(0x01,&version_value,1);
    SPI_Sx1278_ReadBuffer(0x01,&version_value,1);
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);
    SPI_Sx1278_ReadBuffer(0x42,&version_value,1);
    while (1)
    {
      
    }
}








