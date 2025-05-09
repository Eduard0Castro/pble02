#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define PINO_LED1_4 	0 		// GPIO3[0]
#define PINO_LED3 		1	  	// GPIO1[1]
#define PINO_LED2 		2	  	// GPIO1[2]

#define UP 				1		// DSR|GPIO2[1]
#define LEFT			3	 	// GPIO0[3]
#define DOWN			7	 	// GIPO2[7]
#define RIGHT 			8	 	// GIPO2[8]
#define SELECT 			2 		// GPIO0[2]

#define TX 				6 		// GPIO1_6
#define RX 				7 		// GPIO1_7

void configuraLED();
void apagaLEDS();
void acendeLEDS();
void apagaLEDgpio(int, int);
void apagaLED(int);
void acendeLED(int);
void toggleLED(int);

int main(void)
{

	char c;

	// habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

	// Configurar GPIO3[0] (P0_3) como saída (UM10398, seção 12.3.2)
	LPC_GPIO3->DIR |= (1 << PINO_LED1_4);
	LPC_GPIO1->DIR |= (1 << PINO_LED2);
	LPC_GPIO1->DIR |= (1 << PINO_LED3);
	LPC_GPIO1->DIR |= (1 << PINO_LED1_4);

	LPC_GPIO2->DIR |= (1 << 10);
	LPC_GPIO2->DIR |= (1 << 2);

	LPC_GPIO0->DIR &= (0 << LEFT);
	LPC_GPIO0->DIR &= (0 << SELECT);
	LPC_GPIO2->DIR &= (0 << UP);
	LPC_GPIO2->DIR &= (0 << DOWN);
	LPC_GPIO2->DIR &= (0 << RIGHT);

	// Comunicacao serial
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
	LPC_IOCON->PIO1_7 |= 0x01;				// configurar o pino UART TXD (UM10398, 7.4.41)
	LPC_IOCON->PIO1_6 |= 0x01;				// configurar o pino UART RXD (UM10398, 7.4.41)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); // habilitar o clock para o bloco UART (UM10398, 3.5.14)
	LPC_SYSCON->UARTCLKDIV |= 0x9B;			// estipular o clock do módulo UART (divisor de 0x9B) para gerar BR de 19.2K (UM10398, 3.5.16)
	LPC_UART->FCR |= 0x01;					// habilitar o FIFO (necessário para operar) (UM10398, 13.5.6)
	LPC_UART->LCR |= 0x03;					// estipular um tamanho de palavra de 8 bits (UM10398, 13.5.7)
	LPC_UART->TER |= 0x80;					// habilitar a transmissão (UM10398, 13.5.16)

	LPC_IOCON->R_PIO1_2 = 0x081;
	LPC_IOCON->R_PIO1_1 = 0x081;
	LPC_IOCON->R_PIO1_0 = 0x081;
	LPC_IOCON->PIO2_8 = 0x0000;

	apagaLEDS();
	while (1)
	{
		toggleLED(1);
		toggleLED(2);
		toggleLED(3);
		toggleLED(4);
		for(int i = 0; i < 123456; i++);
	}
}

// Apaga TODOS os LEDS
void apagaLEDS()
{
	LPC_GPIO3->DATA &= (0 << PINO_LED1_4);
	LPC_GPIO1->DATA &= (0 << PINO_LED2);
	LPC_GPIO1->DATA &= (0 << PINO_LED3);
	LPC_GPIO1->DATA &= (0 << PINO_LED1_4);
}

// Acende TODOS os LEDS
void acendeLEDS()
{
	LPC_GPIO3->DATA |= (1 << PINO_LED1_4);
	LPC_GPIO1->DATA |= (1 << PINO_LED2);
	LPC_GPIO1->DATA |= (1 << PINO_LED3);
	LPC_GPIO1->DATA |= (1 << PINO_LED1_4);
}

void apagaLEDgpio(int gpio, int led)
{
	switch (gpio)
	{
	case 0:
		LPC_GPIO0->DATA &= (0 << led);
		break;
	case 1:
		LPC_GPIO1->DATA &= (0 << led);
		break;
	case 2:
		LPC_GPIO2->DATA &= (0 << led);
		break;
	case 3:
		LPC_GPIO3->DATA &= (0 << led);
		break;
	default:
		return;
	}
}

// Considera o numero do LED e nao o pino que esta conectado
void apagaLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA &= (0 << PINO_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA &= (0 << PINO_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA &= (0 << PINO_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA &= (0 << PINO_LED1_4);
		break;
	default:
		return;
	}
}

// Considera o numero do LED e nao o pino que esta conectado
void acendeLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA |= (1 << PINO_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA |= (1 << PINO_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA |= (1 << PINO_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA |= (1 << PINO_LED1_4);
		break;
	default:
		return;
	}
}

// Considera o numero do LED e nao o pino que esta conectado
void toggleLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA ^= (1 << PINO_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA ^= (1 << PINO_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA ^= (1 << PINO_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA ^= (1 << PINO_LED1_4);
		break;
	default:
		return;
	}
}
