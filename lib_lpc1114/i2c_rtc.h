#include "LPC11xx.h"
#include "system_LPC11xx.h"

#define K_ENDERECO_MCP7940 0x6F // Endereço em 7 bits 654 3210

void I2C_Config();
void I2C_Transmitir(unsigned char endereco, unsigned char *valor, unsigned char qtd);
void I2C_Receber(unsigned char endereco, unsigned char *valor, unsigned char qtd);