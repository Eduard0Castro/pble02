#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "i2c_rtc.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void)
{

    clocks_config();
    configureLED();
    LCD_pin_config();
    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    I2C_Config();

    char tempo[3];
    char config = *timeConfig(0x23, 0x53, 0x45);

    acendeLEDS();
    LCD_escreve("Horario: ");

    while (1)
    {

        getRTCData(tempo, &config);
        delay_ms(500);
        toggleLEDS();

        // Limpo o último bit
        char segundos = tempo[0] & 0x7F;
        char minutos  = tempo[1] & 0x7F;
        char horas    = tempo[2] & 0x3F; //Hora usa até o bit 5

        LCD_set_cursor(1, 0);
        LCD_escreve(convertBCD_ASCII(horas));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(minutos));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(segundos));


    }
    return 0;
}

/*
#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "i2c_rtc.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void)
{

    clocks_config();
    configureLED();
    LCD_pin_config();
    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    I2C_Config();

    char date[3];
    char config = *dateConfig(0x25, 0x06, 0x25);

    *timeConfig(0x23, 0x59, 0x55);

    acendeLEDS();
    LCD_escreve("Data: ");

    while (1)
    {

        getRTCData(date, &config);
        delay_ms(500);
        toggleLEDS();

        // Limpo o último bit
        char dia = date[0];
        char mes = date[1];
        char ano = date[2];

        LCD_set_cursor(1, 0);
        LCD_escreve(convertBCD_ASCII(dia));
        LCD_escreve("/");
        LCD_escreve(convertBCD_ASCII(mes));
        LCD_escreve("/");
        LCD_escreve(convertBCD_ASCII(ano));


    }
    return 0;
}
*/