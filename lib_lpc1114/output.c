#include "output.h"
#include "LCD.h"
#include "stateMachine.h"
#include "var.h"

#define NUM_IDIOMAS 2

//msgs com 16 caracteres
//1 msg por estado (apenas linha de cima)
static char * msgs[STATE_FIM][NUM_IDIOMAS] = {
    {"Alterar alarme ", "Change alarm   "},
    {"Alterar tempo  ", "Change time    "},
    {"Alterar idioma ", "Change language"}
};

void outputInit(void) {
    LCD_iniciar();
}

void outputPrint(int numTela, int idioma) {

    if (numTela == STATE_TEMPO) {
        LCD_cmd(0x80);
        LCD_escreve(msgs[numTela][idioma]);
        LCD_cmd(0xC0);
        LCD_Int(getTime());
        LCD_escreve("           ");//para apagar os textos depois do numero
    }
    if (numTela == STATE_ALARME) {
        LCD_cmd(0x80);
        LCD_escreve(msgs[numTela][idioma]);
        LCD_cmd(0xC0);
        LCD_Int(getAlarmLevel());
        LCD_escreve("           ");//para apagar os textos depois do numero
    }
    if (numTela == STATE_IDIOMA) {
        LCD_cmd(0x80);
        LCD_escreve(msgs[numTela][idioma]);
        LCD_cmd(0xC0);
        if (getLanguage() == 0) {
            LCD_escreve("Portugues       ");
        }
        if (getLanguage() == 1) {
            LCD_escreve("English         ");
        }

    }

}

