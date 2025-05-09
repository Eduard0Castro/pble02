#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define PINO_LED1_4 	0 		// GPIO3[0]
#define PINO_LED3 		1	  	// GPIO1[1]
#define PINO_LED2 		2	  	// GPIO1[2]

void configureLED();
void apagaLEDS();
void acendeLEDS();
void toggleLEDS();
void apagaLEDgpio(int, int);
void apagaLED(int);
void acendeLED(int);
void toggleLED(int);
