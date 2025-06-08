#include "peripherics.h" // LPC11xx Peripheral Registers

void configureButtons()
{
    // Configurar os pinos dos botoes com entrada
    LPC_GPIO0->DIR &= ~(1 << LEFT);
    LPC_GPIO0->DIR &= ~(1 << SELECT);
    LPC_GPIO2->DIR &= ~(1 << UP);
    LPC_GPIO2->DIR &= ~(1 << DOWN);
    LPC_GPIO2->DIR &= ~(1 << RIGHT);
}

char readButtons()
{
    int b_s, b_u, b_d, b_l, b_r;

    if (!(LPC_GPIO0->DATA & buttonS))
    {
        b_s = 0x01;
    }
    if (!(LPC_GPIO0->DATA & buttonL))
    {
        b_l = 0x01;
    }
    if (!(LPC_GPIO2->DATA & buttonU))
    {
        b_u = 0x01;
    }
    if (!(LPC_GPIO2->DATA & buttonD))
    {
        b_d = 0x01;
    }
    if (!(LPC_GPIO2->DATA & buttonR))
    {
        b_r = 0x01;
    }

    if ((LPC_GPIO0->DATA & buttonS) && (b_s))
    {
        b_s = 0x00;
        return 's';
    }
    else if ((LPC_GPIO0->DATA & buttonL) && (b_l))
    {
        b_l = 0x00;
        return 'l';
    }
    else if ((LPC_GPIO2->DATA & buttonU) && (b_u))
    {
        b_u = 0x00;
        return 'u';
    }
    else if ((LPC_GPIO2->DATA & buttonD) && (b_d))
    {
        b_d = 0x00;
        return 'd';
    }
    else if ((LPC_GPIO2->DATA & buttonR) && (b_r))
    {
        b_r = 0x00;
        return 'r';
    }
    else
        return ' ';
}
