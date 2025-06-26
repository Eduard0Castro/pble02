#include "peripherics.h"
#include "event.h"
#include "serial.h"

static unsigned char key_ant;

void eventInit(void) {
    configureButtons();
    key_ant = 0;
}

unsigned int eventRead(void) {
    char key, data;
    int ev = EV_NOEVENT;
    key = readButtons();
    if (key != key_ant) {
        if (key == 'r') ev = EV_RIGHT;
        if (key == 'l') ev =  EV_LEFT;
        if (key == 's') ev = EV_ENTER;
        if (key == 'u') ev =    EV_UP;
        if (key == 'd') ev =  EV_DOWN;
    }

    data = readSerial();
	if (data == 'r') ev = EV_RIGHT;
	if (data == 'l') ev =  EV_LEFT;
	if (data == 's') ev = EV_ENTER;
	if (data == 'u') ev =    EV_UP;
	if (data == 'd') ev =  EV_DOWN;

    key_ant = key;
    return ev;
}
