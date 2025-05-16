#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void) {
    // CLOCK GPIO e IOCON

	clocks_config();

	configureLED();
	apagaLEDS();
    LCD_pin_config();
    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    LCD_escreve("Du, Dudu e Glauber");
	acendeLEDS();

    while (1){toggleLEDS(); delay_ms(50);}; // Loop infinito
}
