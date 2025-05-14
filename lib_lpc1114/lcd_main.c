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
void delay_ms(unsigned int tempo);
void bit_set(volatile uint32_t *end, uint8_t bit);
void bit_clr(volatile uint32_t *end, uint8_t bit);
void envia_nibble(uint8_t nibble);
void LCD_envia_byte(uint8_t dado, int comando);
void LCD_iniciar(void);
void LCD_escreve(const char *texto);
void LCD_cmd(uint8_t cmd);
void pulso_enable();

// === FUNÇÕES AUXILIARES ===
void bit_set(volatile uint32_t *end, uint8_t bit) {
    (*end) |= (1 << bit);
}

void bit_clr(volatile uint32_t *end, uint8_t bit) {
    (*end) &= ~(1 << bit);
}

void delay_ms(unsigned int tempo) {
    for (unsigned int i = 0; i < tempo * 3000; i++) __NOP();
}

// === ENVIA 4 BITS PARA O LCD ===
void envia_nibble(uint8_t nibble) {
    (nibble & 0x01) ? bit_set(&LCD_D4_PORT->DATA, LCD_D4_PIN) : bit_clr(&LCD_D4_PORT->DATA, LCD_D4_PIN);
    (nibble & 0x02) ? bit_set(&LCD_D5_PORT->DATA, LCD_D5_PIN) : bit_clr(&LCD_D5_PORT->DATA, LCD_D5_PIN);
    (nibble & 0x04) ? bit_set(&LCD_D6_PORT->DATA, LCD_D6_PIN) : bit_clr(&LCD_D6_PORT->DATA, LCD_D6_PIN);
    (nibble & 0x08) ? bit_set(&LCD_D7_PORT->DATA, LCD_D7_PIN) : bit_clr(&LCD_D7_PORT->DATA, LCD_D7_PIN);
}

// === ENVIA UM COMANDO OU CARACTERE COMPLETO (8 BITS) ===
void LCD_envia_byte(uint8_t dado, int comando) {
    if (comando)
        bit_clr(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // comando
    else
        bit_set(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // caractere

    envia_nibble(dado >> 4); // parte alta
    bit_set(&LCD_E_PORT->DATA, LCD_E_PIN);
    delay_ms(1);
    bit_clr(&LCD_E_PORT->DATA, LCD_E_PIN);

    envia_nibble(dado & 0x0F); // parte baixa
    bit_set(&LCD_E_PORT->DATA, LCD_E_PIN);
    delay_ms(1);
    bit_clr(&LCD_E_PORT->DATA, LCD_E_PIN);

    delay_ms(2);
}

// === ENVIA UM COMANDO DIRETAMENTE ===
void LCD_cmd(uint8_t cmd) {
    LCD_envia_byte(cmd, 1);
}

// === ESCREVE UMA STRING NO DISPLAY ===
void LCD_escreve(const char *texto) {
    while (*texto) {
        LCD_envia_byte(*texto++, 0);
    }
}

// === CONFIGURAÇÃO INICIAL DO LCD ===
void LCD_iniciar()
{
    bit_clr(&LCD_E_PORT->DATA,  LCD_E_PIN);
    bit_clr(&LCD_RS_PORT->DATA, LCD_RS_PIN);

    delay_ms(100);

    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x02); pulso_enable(); delay_ms(5);  // Inicializa em 4 bits

    LCD_cmd(0x28); delay_ms(5); // 4 bits, 2 linhas, fonte 5x8
    LCD_cmd(0x0C); delay_ms(5); // Display ON, cursor OFF
    LCD_cmd(0x06); delay_ms(5); // incremento automático
    LCD_cmd(0x01); delay_ms(5); // limpa display
}

void pulso_enable() {
    bit_set(&LCD_E_PORT->DATA, LCD_E_PIN);
    delay_ms(1);
    bit_clr(&LCD_E_PORT->DATA, LCD_E_PIN);
}



// === FUNÇÃO PRINCIPAL ===
int main(void) {
    // CLOCK GPIO e IOCON
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);   // GPIO
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);  // IOCON

    // DIREÇÃO DOS PINOS
    LCD_RS_PORT->DIR |= (1 << LCD_RS_PIN);
    LCD_E_PORT->DIR  |= (1 << LCD_E_PIN);
    LCD_D4_PORT->DIR |= (1 << LCD_D4_PIN);
    LCD_D5_PORT->DIR |= (1 << LCD_D5_PIN);
    LCD_D6_PORT->DIR |= (1 << LCD_D6_PIN);
    LCD_D7_PORT->DIR |= (1 << LCD_D7_PIN);

    // CONFIGURAÇÃO IOCON (sem função alternativa)
    LPC_IOCON->PIO3_1  = 0x00; // RS
    LPC_IOCON->PIO1_4  = 0x80; // E
    LPC_IOCON->PIO1_11 = 0x80; // D4
    LPC_IOCON->PIO3_2  = 0x00; // D5
    LPC_IOCON->PIO1_5  = 0x00; // D6
    LPC_IOCON->PIO3_3  = 0x00; // D7

    LPC_GPIO2->DIR |= (1 << 3);        // RW como saída
    LPC_IOCON->PIO2_3 = 0x00;          // função GPIO padrão
    LPC_GPIO2->DATA &= ~(1 << 3);      // força RW = 0 (escrita)


    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    LCD_escreve("Du, Dudu e Glauber");
    LCD_cmd(0xC0);
    LCD_escreve("er");



    while (1); // Loop infinito
}
