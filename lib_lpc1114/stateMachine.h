#ifndef STATEMACHINE_H
#define	STATEMACHINE_H


//estados da maquina de Estados

enum {
    STATE_ALARME,
    STATE_TEMPO,
    STATE_IDIOMA,
    STATE_FIM
};


void smInit(void);
void smLoop(void);


#endif	/* STATEMACHINE_H */

