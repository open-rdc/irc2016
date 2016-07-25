#include "mbed.h"

Serial servo(D10, D2);
unsigned char tx1_[13][4];

void ServoAng(unsigned char id, int ang)
{
    servo.baud(115200);
    servo.format(9, Serial::Even,1);
    int i;
    tx1_[id][0] = 0x80 | id;
    tx1_[id][1] = (ang >> 7 & 0x7F);
    tx1_[id][2] = (ang & 0x7F);
    
    for (i = 0; i < 3; i++){ servo.putc(tx1_[id][i]);}
}

int main()
{
    while(1){
    ServoAng(0,10000);
    wait(1.0);
    ServoAng(0,3000);
    wait(1.0);
    ServoAng(0,7500);
    wait(1.0);
    }
}