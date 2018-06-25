#include "mbed.h"
//biblioteca do display
#include "Adafruit_SSD1306.h"
//biblioteca de controle de tempo
#include "millis.h"


class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk) {
        format(8,3);
        frequency(2000000);
    };
};


//umidade
AnalogIn moisture(p15);
//ldr
AnalogIn photocell(p18);
//bluetooth
Serial rn42(p13,p14);
//led do bluetooth
DigitalOut ledbluetooth(p19);
//led do motor e ligamento do motor por saida digital
DigitalOut ledmotor(p20);
//led de luz
DigitalOut ledlux(p12);
//entrada de presença
DigitalIn presence(p16);
SPIPreInit gSpi(p5,NC,p7);
//display
Adafruit_SSD1306_Spi gOled1(gSpi,p8,p6,p11);


//função para piscar o led do bluetooth quando ele é acionado
void piscaledbluetooth(void)
{
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
    ledbluetooth = 1;
    wait(0.2);
    ledbluetooth = 0;
    wait(0.2);
}
void Rx_interrupt();


void Rx_interrupt()
{
    gOled1.setTextCursor(0,0);
    gOled1.printf("INTERROMPI!!!!!!");
    gOled1.display();
    gOled1.clearDisplay();
    return;

}


int main(void)
{
    //contagem do tempo de funcionamento
    millisStart();
    rn42.baud(9600);
    //variavel de controle da senha
    int x1 = 0;
    
    //variaveis de tempo
    unsigned long tempo = 0;
    unsigned long t1=millis();
    
    //limpando o display
    gOled1.clearDisplay();
    //setando o display para imprimir no lugar correto
    gOled1.setTextCursor(0,0);
    //texto do display 
    gOled1.printf("PNCRO 2000\n");
    gOled1.printf("Ver. 107.1.a \n");
    gOled1.printf("Booting...\n");
    //para printar no display corretamente precisa utilizar nessa ordem
    
    
    //iniciando o sistema no bluetooth, pedindo a senha para o usuário
    gOled1.display();
    gOled1.clearDisplay();
    wait(2);
    rn42.printf("Sistema iniciado!\n");
    rn42.printf("-------------INFORMACAO!--------------\n");
    rn42.printf("PNCRO 2000\n");
    rn42.printf("Sistema OK!\n");
    rn42.printf("Senha Necessaria! \r");
    rn42.printf("Digite a senha para acessar as informacoes \n");
    rn42.printf("--------------------------------------\n");

    //iniciando o sistema no display
    gOled1.printf("Sistema iniciado!\r");
    gOled1.display();
    gOled1.clearDisplay();
    wait(1);
    gOled1.setTextCursor(0,0);
    gOled1.printf("PNCRO 2000\n");
    gOled1.printf("Sistema OK!\n");
    gOled1.printf("Bluetooth Desconectado \r");
    gOled1.display();
    gOled1.clearDisplay();


    while(1) {

        float value = 0.0f;
        float valuelux = 0.0f;
        //leitura da umidade
        value = moisture.read();
        //leitura da luz
        valuelux = photocell.read();
        wait(0.5);
        
        //caso a umidade esteja baixa, a planta será regada
        if (value>0.95) {
            gOled1.setTextCursor(0,0);
            gOled1.printf("Baixa umidade!\n");
            gOled1.printf("Umidade: %2.2f\n", value);
            gOled1.printf("Regando \r");
            gOled1.display();
            gOled1.clearDisplay();

            wait(1);
            gOled1.setTextCursor(0,0);
            
            //funcionamento do motor, ligando o led quando ele estiver em funcionamento
            ledmotor = 1;

            //caso o sistema esteja com a senha correta no bluetooth, irá printar no bluetooth
            if (x1==1) {
                rn42.printf("-----------Baixa umidade! -----------\n");
                rn42.printf("Leitura de umidade: %2.2f\n", value);
                rn42.printf("Regando...\n");
                rn42.printf("Leitura de luminosidade: %2.2f\n", valuelux);
                rn42.printf("--------------------------------------\n");
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Conectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            //se não imprime que o bluetooth está desconectado no display
            } else {
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Desconectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            }
            //se estiver escuro o ledlux irá acender
            if (valuelux<0.1) {
                ledlux =1;
            //se não ele ficará apagado
            } else {
                ledlux = 0;
            }
            //o motor irá funcionar por um tempo e será desligado
            wait(1);
            ledmotor = 0;
        }
        
        //caso o tempo tenha passado de 5 min, irá imprimir algumas informações
        //se tiver conectado no bluetooth irá imprimir no celular
        //se não vai imprimir no display que o bluetooth não está conectado
        if(millis()-t1>30000) {

            gOled1.setTextCursor(0,0);
            gOled1.printf("TEMPO! Umidade:\n %2.2f\r", value);
            gOled1.display();
            gOled1.clearDisplay();
            wait(1);

            if (x1==1) {
                rn42.printf("--------------- TEMPO! ---------------\n");
                rn42.printf("Umidade: %2.2f\n", value);
                rn42.printf("Luminosidade: %2.2f\n", valuelux);
                rn42.printf("--------------------------------------\n");
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Conectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            } else {
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Desconectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            }
            //contagem do tempo
            t1=millis();
        }



        //se for detectado presença irá imprimir informações da planta no bluetooth
        //(caso esteja conectado com a senha correta)
        // e no display
        if (presence) {

            gOled1.setTextCursor(0,0);
            gOled1.printf("Presenca Detectada!\n");
            gOled1.printf("Umidade: %2.2f\r", value);
            gOled1.printf("Luminosidade: %2.2f\n", valuelux);
            gOled1.display();
            gOled1.clearDisplay();
            wait(1);

            wait(1);
            if (x1==1) {
                rn42.printf("------------- Presenca! --------------\n");
                rn42.printf("Umidade: %2.2f\n", value);
                rn42.printf("--------------------------------------\n");
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Conectado \r");
                gOled1.display();
                gOled1.clearDisplay();

            } else {
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Desconectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            }
        }
        if (x1==0) {
            if (rn42.readable()) {
                //digitar senha do bluetooth
                char senha = rn42.getc();
                // a senha é o caracter 'l', caso correta, imprime que o sistema 
                //está conectado
                
                if (senha=='l') {
                    gOled1.setTextCursor(0,0);
                    gOled1.printf("Senha correta\r");
                    gOled1.display();
                    gOled1.clearDisplay();
                    rn42.printf("-------------CONECTADO!--------------\n");
                    rn42.printf("PNCRO 2000\n");
                    rn42.printf("Sistema OK!\n");
                    rn42.printf("Bluetooth Conectado \r");
                    rn42.printf("Bem Vindo! \n");
                    rn42.printf("--------------------------------------\n");
                    wait(1);
                    //seta a variavel de verificação de senha para 1 
                    x1 = 1;
                    //pisca led quando conectado com a senha correta
                    piscaledbluetooth();
                //caso a senha esteja incorreta, avisa o usuário e permite que
                //ele digite a senha de novo 
                } else {
                    rn42.printf("----------------ERRO!-----------------\n");
                    rn42.printf("Senha incorreta!!! Verifique a senha\n");
                    rn42.printf("--------------------------------------\n");
                    gOled1.setTextCursor(0,0);
                    gOled1.printf("Senha incorreta! \n Tente Novamente.\r");
                    gOled1.display();
                    gOled1.clearDisplay();
                    char key = rn42.getc();
                }

            }
        }
    }
}

