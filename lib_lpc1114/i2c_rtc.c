#include "i2c_rtc.h"

void I2C_Config(){
    // configurar o pino como SCL em modo I2C padrão (I2CMODE=00) (UM10398, 7.4.11)
    LPC_IOCON->PIO0_4 = 0x01;
    // configurar o pino como SDA em modo I2C padrão (I2CMODE=00) (UM10398, 7.4.12)
    LPC_IOCON->PIO0_5 = 0x01;

    // garantir que o periférico I2C não esteja em estado de reset (UM10398, 3.5.2)
    LPC_SYSCON->PRESETCTRL |= (1 << 1);

    // Estipular o clock I2C para 100kHz (UM10398, 15.7.5.1)
    LPC_I2C->SCLH = 240;
    LPC_I2C->SCLL = 240;

    // habilitar o periférico I2C como mestre (UM10398, 15.7.1 / 15.8.1)
    LPC_I2C->CONSET |= (1 << 6);
}

void I2C_Transmitir(unsigned char endereco, unsigned char *valor, unsigned char qtd){
    unsigned char c;
    // requisitar o evento de START (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 5);

    // esperar pelo pelo fim do evento de START (bit SI, UM10398, 15.7.1)
    while (LPC_I2C->STAT != 0x08);

    // enviar o endereço do dispositivo escravo e espeficicação de escrita (R/W=0) (UM10398, 15.7.3)
    LPC_I2C->DAT = endereco << 1;
    
    // limpar a sinalizações SI e START (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (5 << 3);
    
    // esperar por um estado 0x18 (ACK recebido) (UM10398, 15.10.1, tabela 236)
    while (LPC_I2C->STAT != 0x18);

    // Enviar os dados requisitados
    for (c = 0; c < qtd; c++)
    {
        // transmitir o dado (UM10398, 15.7.3)
        LPC_I2C->DAT = valor[c];
        // limpar o bit SI para transmitir um byte e esperar por um ACK (UM10398, 15.7.6 e tabela 236)
        LPC_I2C->CONCLR = (1 << 3);
        // esperar por um estado 0x28 (ACK recebido) (UM10398, 15.10.1, tabela 236)
        while (LPC_I2C->STAT != 0x28);
    }
    // requisitar o evento de STOP (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 4);
    // limpar o bit SI (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (1 << 3);
}

void I2C_Receber(unsigned char endereco, unsigned char *valor, unsigned char qtd){
    unsigned char c;
    // requisitar o evento de START (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 5);
    // esperar pelo pelo fim do evento de START (bit SI, UM10398, 15.7.1)
    while (LPC_I2C->STAT != 0x08);

    // enviar o endereço do dispositivo escravo e espeficicação de leitura (R/W=1) (UM10398, 15.7.3)
    LPC_I2C->DAT = (endereco << 1) + 1;
    // limpar a sinalizações SI e START (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (5 << 3);
    // esperar pela transmissão do endereço e recebimento de um ACK (estado 0x40) (UM10398, 15.10.1, tabela 237)
    while (LPC_I2C->STAT != 0x40);
    
    // Receber os dados requisitados
    for (c = 0; c < qtd; c++)
    {
        // Quando for receber o último byte, deve-se gerar
        // um NACK (Not Ack), caso contrário, um ACK
        
        // limpar os bits SI, START e AA para gerar um NACK (UM10398, 15.7.6 e tabela 237)
        LPC_I2C->CONCLR = 0x2C;
        if (c != (qtd - 1))
        {
            // estipular o AA para gerar um ACK (UM10398, 15.7.1 e tabela 2.3.7)
            LPC_I2C->CONSET = 0x4;
        }

        // esperar pelo recebimento de um byte (o estado deve assumir 0x50 ou 0x58) (UM10398, 15.10.2, tabela 237)
        while ((LPC_I2C->STAT != 0x50) && (LPC_I2C->STAT != 0x58));

        // recuperar e armazenar o byte recebido na lista de valores
        valor[c] = LPC_I2C->DAT;
    }
    // requisitar o evento de STOP (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 4);
    // limpar o bit SI (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (1 << 3);
}

char * timeConfig(char hora, char minuto, char segundo){

    static char config[6];

    // Posicionar o RTC em seu registro de segundos (MCP7940, tabela 5.1)
    config[0] = 0x00;            // endereço segundos
    config[1] = 0x80 | segundo;            // 00s com ST ligado
    config[2] = 0x01;            // endereço minutos
    config[3] = minuto;            // 30 min
    config[4] = 0x02;            // endereço horas
    config[5] = hora;            // 17h

    // Sinalizar operação de escrita a partir dos endereçoes apontador:

    //Escreva o conteudo de config[1] em config[0]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[0], 2);
    //Escreva o conteudo de config[3] em config[2]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[2], 2);
    //Escreva o conteudo de config[5] em config[4]
    I2C_Transmitir(K_ENDERECO_MCP7940, (unsigned char*)&config[4], 2);

    return config;

}

void getRTCTime(char * time, char * config){

	// Sinaliza operação a ser feito nos endereços previamente especificados:
	I2C_Transmitir(K_ENDERECO_MCP7940,  (unsigned char*)config, 1);

	// Recebe no vetor tempo o conteúdo de 0x00, 0x01 e 0x02 (segundos, minutos e hora)
	I2C_Receber(K_ENDERECO_MCP7940,  (unsigned char*)time, 3);
}
