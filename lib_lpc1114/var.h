#ifndef VAR_H
#define	VAR_H

void varInit(void);

char getState(void);
void setState(char newState);
int getTime(void);
void setTime(int newTime);
int getAlarmLevel(void);
void setAlarmLevel(int newAlarmLevel);
void flushTime();
char getHora(void);
void setHora(char newHora);
char getMinuto(void);
void setMinuto(char newMinuto);
char getSegundo(void);
void setSegundo(char newSegundo);
void setHorario();
void updateHorario();
char getLanguage(void);
void setLanguage(char newLanguage);
#endif	/* VAR_H */
