#include "var.h"
#include "i2c_rtc.h"

//variaveis a serem armazenadas
static char state;
static char language;
static int time;
static int alarmLevel;
static char hora, minuto, segundo;
static char config;

void varInit(void) {
    state = 0;
    time = 1000;
    alarmLevel = 512;
    hora = 12;
    minuto = 30;
    segundo = 30;
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

void flushTime(){

    hora = 12;
    minuto = 30;
    segundo = 30;
}

char getHora(void){return hora;}
void setHora(char newHora){
    char realNewHora =  newHora < 24 && 0 <= newHora ? newHora: 0;
    hora = realNewHora;}

char getMinuto(void){return minuto;}
void setMinuto(char newMinuto){
    char realNewMinuto = newMinuto < 60 && 0 <= newMinuto ? newMinuto:  0;
    minuto = realNewMinuto;}

char getSegundo(void){return segundo;}
void setSegundo(char newSegundo){
    char realNewSegundo = newSegundo < 60 && 0 <= newSegundo ? newSegundo: 0;
    segundo = realNewSegundo;}

uint32_t dec2bcd(uint16_t dec)
{
    uint32_t result = 0;
    int shift = 0;

    while (dec)
    {
        result +=  (dec % 10) << shift;
        dec = dec / 10;
        shift += 4;
    }
    return result;
}

void setHorario(){
    uint32_t hora_ = dec2bcd(hora);
    uint32_t minuto_ = dec2bcd(minuto);
    uint32_t segundo_ = dec2bcd(segundo);

    config = *RTCConfig(0, hora_, minuto_, segundo_);
}

void updateHorario(){

    char tempo[3];
    getRTCData(tempo, &config);
    // Limpo o último bit
    segundo = tempo[0] & 0x7F;
    minuto  = tempo[1] & 0x7F;
    hora    = tempo[2] & 0x3F; //Hora usa até o bit 5

}
char getLanguage(void){
    return language;
}
void setLanguage(char newLanguage){
    //so tem 2 linguas
    //usando resto pra evitar colocar valor errado
    language = newLanguage%2;
}
