#include "LED.h"
#include "peripherics.h"
#include "serial.h"

int main(void)
{
    configureButtons();
    configureLED();
	configureSerial();

    char button, serial;

    apagaLEDS();
    while (1)
    {
    	button = readButtons();

    	if(button == 'u') toggleLED(1);
    	if(button == 'l') toggleLED(2);
    	if(button == 'd') toggleLED(3);
    	if(button == 'r') toggleLED(4);
    	if(button == 's') {
    		serial = readSerial();
    		if(serial == '1') toggleLED(1);
    		if(serial == '2') toggleLED(2);
    		if(serial == '3') toggleLED(3);
    		if(serial == '4') toggleLED(4);
    		if(serial == 'a') acendeLEDS();
    	}

    }
    return 0;
}
