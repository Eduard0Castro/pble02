#include "output.h"
#include "LCD.h"
#include "stateMachine.h"
#include "var.h"

#define NUM_IDIOMAS 2

//msgs com 16 caracteres
//1 msg por estado (apenas linha de cima)
static char * msgs[STATE_FIM][NUM_IDIOMAS] = {
    {"Alterar alarme ", "Change alarm   "},
    {"Alterar tempo  ", "Change timer   "},
    {"Alterar idioma ", "Change language"},
    {"Alterar hora   ", "Change hour    "},
    {"Alterar minuto ", "Change minute  "},
    {"Alterar segundo", "Change second  "},
    {"Horario atual  ", "Current time   "}


};

void outputInit(void) {
    LCD_iniciar();
}

void outputPrint(int numTela, int idioma) {
    LCD_cmd(0x80);
    LCD_escreve(msgs[numTela][idioma]);
    LCD_cmd(0xC0);

    switch (numTela){

        case STATE_TEMPO:
            LCD_Int(getTime());
            LCD_escreve("           ");//para apagar os textos depois do numero
            break;

        case STATE_ALARME:
            LCD_Int(getAlarmLevel());
            LCD_escreve("           ");
            break;

        case STATE_IDIOMA:
            if (getLanguage() == 0) {
                LCD_escreve("Portugues       ");
            }
            if (getLanguage() == 1) {
                LCD_escreve("English         ");
            }
            break;

        case STATE_HORA:
            LCD_Int(getHora());
            LCD_escreve("           ");

            break;

        case STATE_MINUTO:
            LCD_Int(getMinuto());
            LCD_escreve("           ");

            break;

        case STATE_SEGUNDO:
            LCD_Int(getSegundo());
            LCD_escreve("           ");

            break;

        case STATE_HORARIO:
            updateHorario();
            LCD_escreve(convertBCD_ASCII(getHora()));
            LCD_escreve(":");
            LCD_escreve(convertBCD_ASCII(getMinuto()));
            LCD_escreve(":");
            LCD_escreve(convertBCD_ASCII(getSegundo()));

            break;

        default: break;
    }
}

