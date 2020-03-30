#include "mbed.h"

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

DigitalIn  Switch(SW3);

DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);

int sample = 1024;
int i;
float data[1024];
int peaknum = 0;
int T[2];
int freq;
int seg_show[3];


int main(){
    wait(5);
    greenLED = 0;
    redLED = 1;
    for(i = 0; i < sample; i++){
        data[i] = Ain;
        wait(1./sample);
    }
    for(i = 1; i < sample-1 && peaknum < 2; i++){
        if((data[i] > data[i-1]) && data[i] > data[i+1]){
            T[peaknum] = i;
            peaknum++;
        }
    }
    freq = sample/(T[1] - T[0]);
    seg_show[0] = int(freq/100);
    seg_show[1] = int((freq - seg_show[0]*100)/10);
    seg_show[2] = freq - seg_show[0]*100 - seg_show[1]*10;
    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", data[i]);
        wait(0.01);
    }
    //pc.printf("freq = %d %d %d %d\r\n", freq, seg_show[0], seg_show[1], seg_show[2]);
    while(1){
        if(Switch == 1){
            greenLED = 0;
            redLED = 1;
            display = 0x00;
            for(float k = 0; k < 2; k += 0.02){
                Aout = (1 + sin(k*3.14159))/3.3;
                wait(1./freq/100);
            }
        }
        else{
            while(Switch == 0){
                greenLED = 1;
                redLED = 0;
                for(int j = 0; j < 3; j++){
                    if(j == 2)
                        display = table[seg_show[j]] + 0x80;
                    else
                        display = table[seg_show[j]];
                    wait(1);
                }
            }
        }
    }
    return 0;
}