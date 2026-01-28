#include "DEV_Config.h"

/**
 * GPIO
**/
int EPD_RST_PIN;
int EPD_DC_PIN;
int EPD_CS_PIN;
int EPD_BUSY_PIN;
int EPD_PWR_PIN;
int EPD_MOSI_PIN;
int EPD_SCLK_PIN;

void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    digitalWrite(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    return digitalRead(Pin);
}

void DEV_SPI_WriteByte(UBYTE Value)
{
    SPI.transfer(Value);
}



void DEV_Delay_ms(UDOUBLE xms)
{
    delay(xms);
}



void DEV_GPIO_Init(void)
{
    //On e-Paper Driver HAT 
    // Display Config in position B
    // Interface Config in position 0 

    // set pin numbers 
    EPD_RST_PIN     = 4;
	EPD_DC_PIN      = 5; 
	EPD_CS_PIN      = 6; // SPI2 (HSPI)
    EPD_PWR_PIN     = 7;
	EPD_BUSY_PIN    = 15;
    EPD_MOSI_PIN    = 11; // SPI2 (HSPI) DIN
	EPD_SCLK_PIN    = 12; // SPI2 (HSPI)

    pinMode(EPD_BUSY_PIN, INPUT);
    pinMode(EPD_RST_PIN, OUTPUT);
    pinMode(EPD_DC_PIN, OUTPUT);
    pinMode(EPD_CS_PIN, OUTPUT);
    pinMode(EPD_PWR_PIN, OUTPUT);

    DEV_Digital_Write(EPD_CS_PIN, HIGH);
    DEV_Digital_Write(EPD_PWR_PIN, HIGH);
}

UBYTE DEV_Module_Init(void)
{
    // printf("/***************** Module Init ******************/ \r\n");

    DEV_GPIO_Init();

    SPI.begin(EPD_SCLK_PIN, -1, EPD_MOSI_PIN, EPD_CS_PIN);  //SCK,MISO,MOSI,SS

    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); //10MHz

    // printf("/***************** Module Init END ******************/ \r\n");
    return 0;

}

void DEV_Module_Exit(void)
{
    // Serial.println("/***************** Module Exit ******************/");
    DEV_Digital_Write(EPD_CS_PIN, LOW);
    DEV_Digital_Write(EPD_PWR_PIN, LOW);
    DEV_Digital_Write(EPD_DC_PIN, LOW);
    DEV_Digital_Write(EPD_RST_PIN, LOW);
    SPI.endTransaction();
    SPI.end();
    // Serial.println("/***************** Module Exit END ******************/");
}