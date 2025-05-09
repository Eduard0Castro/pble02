/*
**
** Main.c
**
**
**********************************************************************/
/*
Programa de exemplo de integração com visor LCD de 16x2 JHD162A
em modo padrão de 8 bits
Mapa de pinos para o LCD e o LPC1114FN28:
- D0-D7 <-> GPIO1_0-GPIO1_7
- E <-> GPIO0_8
- RS <-> GPIO0_9
- R/W <-> GND
O programa exibe a seguinte mensagem no visor:
ELT031
TESTE DO LCD
Universidade Federal de Itajubá
Prof. Rodrigo de Paula Rodrigues
2015-04-19
**********************************************************************/
#include "LPC11xx.h" /* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
// Definições de pinos e portas
#define LCD_PORTA_CTR LPC_GPIO0   // Utilizar pinos da porta GPIO0 para controlar o LCD (pinos RS e E)
#define LCD_PORTA_DADOS LPC_GPIO1 // Utilizar pinos da porta GPIO1 para enviar dados para o LCD (barramento D0 - D7)
#define LCD_PINO_CTR_E 8          // pino 8 da porta GPIO0
#define LCD_PINO_CTR_RS 9         // pino 9 da porta GPIO0
#define PINO_LED 3                // pino 3 da porta GPIO0
// Protótipos locais
void bit_set(volatile uint32_t *p_iValor, unsigned char v_ucBit);
void bit_clr(volatile uint32_t *p_iValor, unsigned char v_ucBit);
void delay_ms(unsigned int v_uiTempo);
void LCD_iniciar();
void LCD_comando(char v_cComando);
void LCD_caractere(char v_cCaractere);