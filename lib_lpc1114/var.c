#include "var.h"

//variaveis a serem armazenadas
static char state;
static char language;
static int time;
static int alarmLevel;

void varInit(void) {
    state = 0;
    time = 1000;
    alarmLevel = 512;
}


char getState(void) {
    return state;
}
void setState(char newState) {
    state = newState;
}


int getTime(void) {
    return time;
}
void setTime(int newTime) {
    time = newTime;
}


int getAlarmLevel(void) {
    return alarmLevel;
}
void setAlarmLevel(int newAlarmLevel) {
    alarmLevel = newAlarmLevel;
}


char getLanguage(void){
    return language;
}
void setLanguage(char newLanguage){
    //so tem 2 linguas
    //usando resto pra evitar colocar valor errado
    language = newLanguage%2;
}