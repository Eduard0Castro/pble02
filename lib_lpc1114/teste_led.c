#include "LPC11xx.h"         // LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define PINO_LED1_4 0        // GPIO3[0]
#define PINO_LED3   1        // GPIO1[1]
#define PINO_LED2   2        // GPIO1[2]

#define UP          1  	     // DSR|GPIO2[1]
#define LEFT        3        // GPIO0[3]
#define DOWN        7  	     // GIPO2[7]
#define RIGHT       8        // GIPO2[8]
#define SELECT      2	     // GPIO0[2]

#define TX          6	     // GPIO1_6
#define RX          7	     // GPIO1_7

void 	setLEDS(char);
void 	controlLED(char, char);
void 	config_gpio();
void 	config_serial();
void 	IOCONFIG();

int main(void) {

	char c;

	config_gpio();
	config_serial();
	IOCONFIG();
	setLEDS(0);

    while (1) {

        controlLED(43, ((LPC_GPIO0->DATA) & (1 << SELECT)) ? 1:0);
        controlLED(1, ((LPC_GPIO2->DATA) & (1 << UP))     ? 1:0);
        controlLED(2,  ((LPC_GPIO0->DATA) & (1 << LEFT))   ? 1:0);
        controlLED(3,  ((LPC_GPIO2->DATA) & (1 << DOWN))   ? 1:0);
        controlLED(4, ((LPC_GPIO2->DATA) & (1 << RIGHT))  ? 1:0);


        if ( LPC_UART->LSR & 0x01 )
        {
			// obter o dado recebido (UM10398, 13.5.1)
			c = LPC_UART->RBR;
			// Esperar pelo fim da transmissão atual (bit TEMT, UM10398, 13.5.9)
			while ( ( LPC_UART->LSR & ( 1 << 6 ) ) == 0 );
			// Transmitir o caracter recebido (UM10398, 13.5.2)
			LPC_UART->THR = c;
			LPC_UART->THR = (LPC_IOCON->R_PIO1_2  & 0xff);
        }

    }

}

void config_gpio(){

	// habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |=  (1<<6);

    // Configurar GPIO3[0] (P0_3) como saída (UM10398, seção 12.3.2)
    LPC_GPIO3->DIR |= (1 << PINO_LED1_4);
    LPC_GPIO1->DIR |= (1 <<   PINO_LED2);
    LPC_GPIO1->DIR |= (1 <<   PINO_LED3);
    LPC_GPIO1->DIR |= (1 << PINO_LED1_4);

    LPC_GPIO2->DIR |=          (1 << 10);
    LPC_GPIO2->DIR |=           (1 << 2);

    LPC_GPIO0->DIR &=        (0 << LEFT);
    LPC_GPIO0->DIR &=      (0 << SELECT);
    LPC_GPIO2->DIR &=          (0 << UP);
    LPC_GPIO2->DIR &=        (0 << DOWN);
    LPC_GPIO2->DIR &=       (0 << RIGHT);

}

void config_serial(){

    // Comunicacao serial
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
    LPC_IOCON ->PIO1_7        |=    0x01; // configurar o pino UART TXD (UM10398, 7.4.41)
    LPC_IOCON ->PIO1_6        |=    0x01; // configurar o pino UART RXD (UM10398, 7.4.41)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12); // habilitar o clock para o bloco UART (UM10398, 3.5.14)
    LPC_SYSCON->UARTCLKDIV    |=    0x9B; // estipular o clock do módulo UART (divisor de 0x9B) para gerar BR de 19.2K (UM10398, 3.5.16)
    LPC_UART  ->FCR           |=    0x01; // habiltiar o FIFO (necessário para operar) (UM10398, 13.5.6)
    LPC_UART  ->LCR           |=    0x03; // estipular um tabamho de palavra de 8 bits (UM10398, 13.5.7)
    LPC_UART  ->TER           |=    0x80; // habilitar a transmissão (UM10398, 13.5.16)

}

void IOCONFIG(){
	//Configurações do IOCONFIG para pinos marcados como "Reserved"
	LPC_IOCON->R_PIO1_2 =  0x081;
	LPC_IOCON->R_PIO1_1 =  0x081;
	LPC_IOCON->R_PIO1_0 =  0x081;
	LPC_IOCON->PIO2_8   = 0x0000;
}

void setLEDS(char value){
	LPC_GPIO3->DATA = (value << PINO_LED1_4);
	LPC_GPIO1->DATA =      value ? 111 : 000;
}

void controlLED(char led, char turn){

	switch(led){
		case  1: LPC_GPIO3->DATA = (turn << PINO_LED1_4); break;
        case  2: LPC_GPIO1->DATA = (turn <<   PINO_LED2); break;
		case  3: LPC_GPIO1->DATA = (turn <<   PINO_LED3); break;
		case  4: LPC_GPIO1->DATA = (turn << PINO_LED1_4); break;
		case 43: setLEDS(turn); break;
		default: return;
	}
}
