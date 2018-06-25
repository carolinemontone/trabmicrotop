#include "mbed.h"
#include "Motor.h"
#include "Adafruit_SSD1306.h"
#include "millis.h"


class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk) {
        format(8,3);
        frequency(2000000);
    };
};


Motor m(p23, p6, p5); // pwm, fwd, rev
AnalogIn moisture(p15);
AnalogIn photocell(p18);
Serial rn42(p13,p14);
DigitalOut ledbluetooth(p19);
DigitalOut ledmotor(p20);
DigitalOut ledlux(p12);
DigitalIn presence(p16);
SPIPreInit gSpi(p5,NC,p7);
Adafruit_SSD1306_Spi gOled1(gSpi,p8,p6,p11);



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

    millisStart();
    rn42.baud(9600);
    int x1 = 0;
    unsigned long tempo = 0;
    unsigned long t1=millis();
    gOled1.clearDisplay();
    gOled1.setTextCursor(0,0);
    gOled1.printf("PNCRO 2000\n");
    gOled1.printf("Ver. 107.1.a \n");
    gOled1.printf("Booting...\n");
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

//rn42.attach(&Rx_interrupt, Serial::RxIrq);

    while(1) {

        float value = 0.0f;
        float valuelux = 0.0f;
        value = moisture.read();
        valuelux = photocell.read();
        wait(0.5);
        if (value>0.95) {
            gOled1.setTextCursor(0,0);
            gOled1.printf("Baixa umidade!\n");
            gOled1.printf("Umidade: %2.2f\n", value);
            gOled1.printf("Regando \r");
            gOled1.display();
            gOled1.clearDisplay();

            wait(1);
            gOled1.setTextCursor(0,0);
            //m.speed(50);
            ledmotor = 1;
            //wait(1);
            //m.speed(0);
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
            } else {
                gOled1.setTextCursor(0,0);
                gOled1.printf("PNCRO 2000\n");
                gOled1.printf("Sistema OK!\n");
                gOled1.printf("Bluetooth Desconectado \r");
                gOled1.display();
                gOled1.clearDisplay();
            }
            if (valuelux<0.1) {
                ledlux =1;
            } else {
                ledlux = 0;
            }
            wait(1);
            ledmotor = 0;
        }

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

            t1=millis();
        }




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

                char senha = rn42.getc();
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
                    x1 = 1;
                    // PISCA LED VERDE
                    piscaledbluetooth();

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

