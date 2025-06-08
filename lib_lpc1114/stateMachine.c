#include "var.h"
#include "stateMachine.h"
#include "event.h"
#include "output.h"
#include "LCD.h"
#include "peripherics.h"

void smInit(void) {
    setState(STATE_TEMPO);
}

void smLoop(void) {
    unsigned char evento;

    //maquina de estados
    evento = eventRead();

    switch (getState()) {
        case STATE_ALARME:
            //execucao de atividade
            if (evento == EV_RIGHT) {
                setAlarmLevel(getAlarmLevel() + 1);
            }
            if (evento == EV_LEFT) {
                setAlarmLevel(getAlarmLevel() - 1);
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_TEMPO);
            }
            break;
        case STATE_TEMPO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setTime(getTime() + 1);
            }
            if (evento == EV_LEFT) {
                setTime(getTime() - 1);
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_IDIOMA);
            }
            break;

        case STATE_IDIOMA:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setLanguage(getLanguage() + 1);
            }
            if (evento == EV_LEFT) {
                setLanguage(getLanguage() - 1);
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_ALARME);
            }
            break;

    }
    outputPrint(getState(), getLanguage());
}
