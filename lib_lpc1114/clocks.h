#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

void clock_gpio(){
    // habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
}
void clock_iocon(){
    // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
}
void clock_i2c(){
	// habilitar o clock para o bloco I2C (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);
}
void clocks_config(){
	clock_gpio();
	clock_iocon();
	clock_i2c();
}

