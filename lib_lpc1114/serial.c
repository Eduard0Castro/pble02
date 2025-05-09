#include "serial.h"

void configureSerial(){
    // Comunicacao serial
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)

    // habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

    // Configurar GPIO1[6] e GPIO1[7] como saída (UM10398, seção 12.3.2)
    LPC_GPIO1->DIR |= (1 << TX);
    LPC_GPIO1->DIR |= (1 << RX);
    
    LPC_IOCON->PIO1_7 |= 0x01;				// configurar o pino UART TXD (UM10398, 7.4.41)
    LPC_IOCON->PIO1_6 |= 0x01;				// configurar o pino UART RXD (UM10398, 7.4.41)

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); // habilitar o clock para o bloco UART (UM10398, 3.5.14)
    LPC_SYSCON->UARTCLKDIV |= 0x9B;			// estipular o clock do módulo UART (divisor de 0x9B) para gerar BR de 19.2K (UM10398, 3.5.16)
    LPC_UART->FCR |= 0x01;					// habiltiar o FIFO (necessário para operar) (UM10398, 13.5.6)
    LPC_UART->LCR |= 0x03;					// estipular um tabamho de palavra de 8 bits (UM10398, 13.5.7)
    LPC_UART->TER |= 0x80;					// habilitar a transmissão (UM10398, 13.5.16)
}

char readSerial(){
	char c;
    // Ler o caracter recebido (UM10398, 13.5.2)
    while (!(LPC_UART->LSR & 0x01)); // espera até que o FIFO de recepção não esteja vazio
    c = LPC_UART->RBR; // lê o FIFO de recepção (UM10398, 13.5.2)

    // Esperar pelo fim da transmissão atual (bit TEMT, UM10398, 13.5.9)
    // espera até que o FIFO de transmissão esteja vazio
    while ((LPC_UART->LSR & (1 << 6)) == 0);
    writeSerial(c);
    return c;
}

void writeSerial(char c){
    // Enviar o caracter (UM10398, 13.5.2)
    while (!(LPC_UART->LSR & 0x20)); // espera até que o FIFO de transmissão não esteja cheio
    // escreve no FIFO de transmissão (UM10398, 13.5.2)
    LPC_UART->THR = c;

    // Esperar pelo fim da transmissão atual (bit TEMT, UM10398, 13.5.9)
    // espera até que o FIFO de transmissão esteja vazio
    while ((LPC_UART->LSR & (1 << 6)) == 0);
}
