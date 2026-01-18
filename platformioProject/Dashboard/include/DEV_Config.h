#ifndef __DEV_CONFIG_H
#define __DEV_CONFIG_H

#include "Arduino.h"
#include <SPI.h>

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIOI config
**/
extern int EPD_RST_PIN;
extern int EPD_DC_PIN;
extern int EPD_CS_PIN;
extern int EPD_BUSY_PIN;
extern int EPD_PWR_PIN;
extern int EPD_MOSI_PIN;
extern int EPD_SCLK_PIN;

void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);

void DEV_SPI_WriteByte(UBYTE Value);
// void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len);
void DEV_Delay_ms(UDOUBLE xms);

// void DEV_SPI_SendData(UBYTE Reg);
// void DEV_SPI_SendnData(UBYTE *Reg);
// UBYTE DEV_SPI_ReadData();

UBYTE DEV_Module_Init(void);
void DEV_Module_Exit(void);

#endif // __DEV_CONFIG_H