#include "LPC11xx.h"
#include "system_LPC11xx.h"

// === DEFINIÇÕES DE PINOS ===
#define LCD_RS_PORT LPC_GPIO3
#define LCD_E_PORT  LPC_GPIO1
#define LCD_D4_PORT LPC_GPIO1
#define LCD_D5_PORT LPC_GPIO3
#define LCD_D6_PORT LPC_GPIO1
#define LCD_D7_PORT LPC_GPIO3

#define LCD_RS_PIN  1  // PIO3_1
#define LCD_E_PIN   4  // PIO1_4
#define LCD_D4_PIN 11  // PIO1_11
#define LCD_D5_PIN  2  // PIO3_2
#define LCD_D6_PIN  5  // PIO1_5
#define LCD_D7_PIN  3  // PIO3_3

// === PROTÓTIPOS ===
void LCD_pin_config();
void delay_ms(unsigned int tempo);
void bit_set(volatile uint32_t *end, uint8_t bit);
void bit_clr(volatile uint32_t *end, uint8_t bit);
void envia_nibble(uint8_t nibble);
void LCD_envia_byte(uint8_t dado, int comando);
void LCD_iniciar(void);
void LCD_escreve(const char *texto);
void LCD_cmd(uint8_t cmd);
void LCD_caractere(const char *caractere);
void pulso_enable();
void LCD_set_cursor(uint8_t linha, uint8_t coluna);
char * convertBCD_ASCII(char);
