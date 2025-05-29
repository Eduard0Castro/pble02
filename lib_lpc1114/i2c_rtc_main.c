#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "i2c_rtc.h"
#include "LCD.h"
#include "clocks.h"
#include "serial.h"
#include "LED.h"

#define K_ENDERECO_MCP7940 0x6F // Endereço em 7 bits 654 3210

int main(void)
{

    clocks_config();
    configureLED();
    configureSerial();
    LCD_pin_config();
    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    I2C_Config();


    unsigned int i;
    char dados[2];

    dados[0] = 0x00; // registro de segundos (MCP7940, tabela 5.1)
    dados[1] = 0x80; // 10 segundos (BCD) e CH em 0 para habilitar o clock(MCP7940, tabela 5.1)

    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char *)&dados[0], 2);
    acendeLEDS();

    while (1)
    {
        // Posicionar o RTC em seu registro de segundos (MCP7940, tabela 5.1)
        dados[0] = 0x00;
        I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char *)&dados[0], 1);

        // Ler o valor corrente do registro de segundos do RTC (em formato BCD) para a variável 'dados[1]'
        I2C_Receber(K_ENDERECO_MCP7940, (unsigned char *)&dados[1], 1);
        for (i = 0; i < 0xFFFFF; i++)
            ; // gerar um atraso
		toggleLEDS();
        writeSerial(dados[1] & 0x7F);
    }
    return 0;
}
