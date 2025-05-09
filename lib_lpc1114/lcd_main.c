/*
Programa de exemplo de integração com visor LCD de 16x2 JHD162A
em modo padrão de 8 bits
Mapa de pinos para o LCD e o LPC1114FN28:
- D0-D7 <-> GPIO1_0-GPIO1_7
- E <-> GPIO1_4
- RS <-> GPIO3_1
- R/W <-> GND
O programa exibe a seguinte mensagem no visor:
ELT031
TESTE DO LCD
Universidade Federal de Itajubá
Prof. Rodrigo de Paula Rodrigues
2015-04-19
**********************************************************************/
/* LPC11xx Peripheral Registers */
#include "LPC11xx.h"
#include "system_LPC11xx.h"
// Definições de pinos e portas
// Utilizar pinos da porta GPIO0 para controlar o LCD (pinos RS e E)

#define LCD_PORTA_CTR_E LPC_GPIO1
#define LCD_PORTA_CTR_RS LPC_GPIO3

// Utilizar pinos da porta GPIO1 para enviar dados para o LCD (barramento D0 - D7)
#define LCD_PINO_CTR_E 4          // pino 3 da porta GPIO1
#define LCD_PINO_CTR_RS 1         // pino 1 da porta GPIO3
#define LCD_PINO_D4 11			//gpio1_11
#define LCD_PINO_D5 2			// gpio3_2
#define LCD_PINO_D6 5			// gpio1_5
#define LCD_PINO_D7 3			// gpio3_3
#define PINO_LED 3                // pino 3 da porta GPIO0
// Protótipos locais
void bit_set(volatile uint32_t *p_iValor, unsigned char v_ucBit);
void bit_clr(volatile uint32_t *p_iValor, unsigned char v_ucBit);
void delay_ms(unsigned int v_uiTempo);
void LCD_iniciar();
void LCD_comando(char v_cComando);
void LCD_caractere(char v_cCaractere);
// Código principal
int main(void)
{
    //
    // O procedimento SystemInit (CMSIS) configura o CLOCK principal
    // do dispositivo para 48 MHz a partir de um clock externo de
    // 12 MHz (cristal) e de seu circuito PLL interno.
    //
    // Para estipular outra configuração do clock principal,
    // altere as constantes SYSOSCCTRL_Val, WDTOSCCTRL_Val,
    // SYSPLLCTRL_Val, SYSPLLCLKSEL_Val, MAINCLKSEL_Val e
    // SYSAHBCLKDIV_Val presentes no arquivo system_LPC11xx.c
    //
    //
    // Configurar os demais periféricos
    //

	// habilitar o clock para o módulo GPIO(UM10398, 3.5.14)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

    // habilitar o clock para o módulo IOCON(UM10398, 3.5.14)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);

    LCD_PORTA_CTR_E->DIR |= (1 << LCD_PINO_CTR_E);
    LPC_GPIO1->DIR |= (1 << LCD_PINO_D6);
    LPC_GPIO1->DIR |= (1 << LCD_PINO_D4);

    LCD_PORTA_CTR_RS->DIR |= (1 << LCD_PINO_CTR_RS);
    LPC_GPIO3->DIR |= (1 << LCD_PINO_D5);
    LPC_GPIO3->DIR |= (1 << LCD_PINO_D7);

    LPC_IOCON->PIO1_4 = 0x80; // E
    LPC_IOCON->PIO1_5 = 0x00; // D6
    LPC_IOCON->PIO1_11 = 0x80; //D4
    LPC_IOCON->PIO2_3 = 0x00; // RW
    LPC_IOCON->PIO3_1 = 0x00; // RS
    LPC_IOCON->PIO3_2 = 0x00; // D5
    LPC_IOCON->PIO3_3 = 0x00; // D7

    while (1)
    {

    };
    return 0;
}
//
// Implementações locais
//
// bit_set
void bit_set(volatile uint32_t *p_uiValor, unsigned char v_ucBit)
{
    (*p_uiValor) |= (1 << v_ucBit);
}
// bit_clr
void bit_clr(volatile uint32_t *p_uiValor, unsigned char v_ucBit)
{
    (*p_uiValor) &= ~(1 << v_ucBit);
}
//
// delay_ms
//
// Rotina de atraso que leva em consideração um clock de 48 MHz
// para o núcleo
//
void delay_ms(unsigned int v_uiTempo)
{
    unsigned int vuiI1;
    do
    {
        // Gerar um atraso aproximado a 1 milissegundo
        for (vuiI1 = 0; vuiI1 < 3000; vuiI1++)
            ;
        v_uiTempo--;
    } while (v_uiTempo > 0);
}
//
// LCD_iniciar
//
// Configurar a operação do LCD
//
void LCD_iniciar()
{
    bit_clr(&LCD_PORTA_CTR_E->DATA, LCD_PINO_CTR_E);
    bit_clr(&LCD_PORTA_CTR_RS->DATA, LCD_PINO_CTR_RS);
    // Esperar pela inicialização do visor
    delay_ms(1000);
    // Enviar a sequência 0b0011****
    LCD_comando(0x30);
    delay_ms(5);
    // Enviar a sequência 0b0011***
    LCD_comando(0x30);
    delay_ms(5);
    // Enviar a sequência 0b0011****
    LCD_comando(0x30);
    delay_ms(5);
    // Requisitar modo de 8 bits, 2 linhas e fonte 5x10
    LCD_comando(0x3C);
    delay_ms(20);
    // Ligar o visor, ligar o cursor e colocá-lo piscando
    LCD_comando(0x0F);
    delay_ms(20);
    // Limpar o visor
    LCD_comando(0x01);
    delay_ms(20);
    // I/D = 1, S=0
    LCD_comando(0x06);
    delay_ms(20);
}
//
// LCD_comando
//
// Enviar um comando ao LCD
//
void LCD_comando(char v_cComando)
{
    // Sinalizar o envio de um comando
    bit_clr(&LCD_PORTA_CTR_RS->DATA, LCD_PINO_CTR_RS);
    // Enviar o comando aos pinos de dados D0 a D7
    LCD_PORTA_DADOS->DATA = v_cComando;
    delay_ms(1);
    // Enviar o sinal de dados válidos (pulso positivo no pino E)
    bit_set(&LCD_PORTA_CTR_E->DATA, LCD_PINO_CTR_E);
    delay_ms(1);
    bit_clr(&LCD_PORTA_CTR_E->DATA, LCD_PINO_CTR_E);
}
//
// LCD_caractere
//
// Enviar um caractere ao LCD
//
void LCD_caractere(char v_cCaractere)
{
    // Sinalizar o envio de um caractere
    bit_set(&LCD_PORTA_CTR_RS->DATA, LCD_PINO_CTR_RS);
    // Enviar o caractere aos pinos de dados D0 a D7
    LCD_PORTA_DADOS->DATA = v_cCaractere;
    delay_ms(1);
    // Enviar o sinal de dados válidos (pulso positivo no pino E)
    bit_set(&LCD_PORTA_CTR_E->DATA, LCD_PINO_CTR_E);
    delay_ms(1);
    bit_clr(&LCD_PORTA_CTR_E->DATA, LCD_PINO_CTR_E);
}

void sendLCD(char com)
{
    if (0x80 & com)
    {
        LPC_GPIO3->DATA |= (1 << LCD_PINO_D7);
    }else
    {
        LPC_GPIO3->DATA &= ~(1 << LCD_PINO_D7);
    }

    if (0x40 & com)
    {
        LPC_GPIO1->DATA |= (1 << LCD_PINO_D6);
    }

    
}