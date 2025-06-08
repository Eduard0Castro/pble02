#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "LCD.h"
#include "clocks.h"
#include "serial.h"
#include "LED.h"
#include "event.h"
#include "var.h"
#include "stateMachine.h"
#include "output.h"

#define K_ENDERECO_MCP7940 0x6F // Endereço em 7 bits 654 3210

int main(void)
{

	//  init das bibliotecas
    clocks_config();
    configureLED();
    apagaLEDS();
    configureSerial();
    LCD_pin_config();
	outputInit();
	eventInit();
	varInit();
    //timerInit();

    for (;;) {
        //timerReset(getTime());
        
        //state machine:
        smLoop();
        //timerWait();
    }
    return 0;
}
