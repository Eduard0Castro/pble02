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

    char config[6];
    char tempo[3];

    // Posicionar o RTC em seu registro de segundos (MCP7940, tabela 5.1)
    config[0] = 0x00;            // endereço segundos
    config[1] = 0x80;            // 00s com ST ligado
    config[2] = 0x01;            // endereço minutos
    config[3] = 0x23;            // 23 min
    config[4] = 0x02;            // endereço horas
    config[5] = 0x01;            // 01h

    
    // Sinalizar operação de escrita a partir dos endereçoes apontador:

    //Escreva o conteudo de config[1] em config[0]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[0], 2);
    //Escreva o conteudo de config[3] em config[2]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[2], 2);
    //Escreva o conteudo de config[5] em config[4]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[4], 2);

    acendeLEDS();


    while (1)
    {

    	// Sinaliza operação a ser feito nos endereços previamente especificados:
    	I2C_Transmitir(K_ENDERECO_MCP7940,  (unsigned char*)&config, 1);

    	// Recebe no vetor tempo o conteúdo de 0x00, 0x01 e 0x02 (segundos, minutos e hora)
    	I2C_Receber(K_ENDERECO_MCP7940,  (unsigned char*)&tempo, 3);
        delay_ms(500);

        toggleLEDS();

        // Limpo o último bit
        char segundos = tempo[0] & 0x7F;
        char minutos  = tempo[1] & 0x7F;
        char horas    = tempo[2] & 0x3F; //Hora usa até o bit 5

        writeSerial(segundos);

        LCD_set_cursor(0, 0);
        LCD_escreve(convertBCD_ASCII(horas));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(minutos));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(segundos));


    }
    return 0;
}
