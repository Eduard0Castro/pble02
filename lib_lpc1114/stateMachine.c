#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "stateMachine.h"
#include "event.h"
#include "output.h"
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
            if(evento == EV_PROTOCOL){
				setAlarmLevel(atoi(getProtocolMSG()));
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

            if(evento == EV_PROTOCOL){
				setTime(atoi(getProtocolMSG()));
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

            if(evento == EV_PROTOCOL){
            	char *language = getProtocolMSG();
            	if(strcmp(language, "EN")==0)setLanguage(1);
            	else if(strcmp(language, "PT") == 0) setLanguage(0);
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_HORA);
                flushTime();
            }
            break;

        case STATE_HORA:
            //execucao de atividade
            if (evento == EV_RIGHT) {
                setHora(getHora()+1);
            }
            if (evento == EV_LEFT) {
                setHora(getHora()-1);
            }
            if(evento == EV_PROTOCOL){
				setHora(atoi(getProtocolMSG()));
            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_MINUTO);
            }
            break;

        case STATE_MINUTO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setMinuto(getMinuto()+ 1);
            }
            if (evento == EV_LEFT) {
                setMinuto(getMinuto()- 1);
            }
            if(evento == EV_PROTOCOL){
				setMinuto(atoi(getProtocolMSG()));

            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_SEGUNDO);
            }
            break;

        case STATE_SEGUNDO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setSegundo(getSegundo()+ 1);
            }
            if (evento == EV_LEFT) {
                setSegundo(getSegundo()- 1);
            }
            if(evento == EV_PROTOCOL){
				setSegundo(atoi(getProtocolMSG()));
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setHorario();
                setState(STATE_HORARIO);
            }

            if(evento == EV_UP) setState(STATE_HORARIO);

            break;

        case STATE_HORARIO:

            if (evento == EV_ENTER) {
                setState(STATE_ALARME);
            }
            if(evento == EV_PROTOCOL){
				char * horario = getProtocolMSG();
				char hora[2] = {horario[0], horario[1]};
				char minuto[2] = {horario[2], horario[3]};
				char segundo[2] = {horario[4], horario[5]};
				setHora(atoi(hora));
				setMinuto(atoi(minuto));
				setSegundo(atoi(segundo));
				setHorario();
            }

            break;

    }
    outputPrint(getState(), getLanguage());
}
