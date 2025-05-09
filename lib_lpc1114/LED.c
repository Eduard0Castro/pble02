#include "LED.h"

void configureLED()
{
	// habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

	// Configurar GPIO3[0] (P0_3) como saída (UM10398, seção 12.3.2)
	LPC_GPIO3->DIR |= (1 << PINO_LED1_4);
	LPC_GPIO1->DIR |= (1 << PINO_LED2);
	LPC_GPIO1->DIR |= (1 << PINO_LED3);
	LPC_GPIO1->DIR |= (1 << PINO_LED1_4);

	// habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);

	LPC_IOCON->R_PIO1_2 = 0x081;
	LPC_IOCON->R_PIO1_1 = 0x081;
	LPC_IOCON->R_PIO1_0 = 0x081;
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

// Inverte TODOS os LEDS
void toggleLEDS()
{
	LPC_GPIO3->DATA ^= (1 << PINO_LED1_4);
	LPC_GPIO1->DATA ^= (1 << PINO_LED2);
	LPC_GPIO1->DATA ^= (1 << PINO_LED3);
	LPC_GPIO1->DATA ^= (1 << PINO_LED1_4);
}

// Considera o pino do LED e a GPIO
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