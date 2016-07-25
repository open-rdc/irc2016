#include "Function.h"

Serial servo(D8,PA_12);
unsigned char tx[13][4];

void ServoAng(unsigned char id, int ang)
{
    servo.baud(115200);
    servo.format(9,Serial::Even,1);
    int i;
    
    tx[id][0] = 0x80 | id;
    tx[id][1] = ang >> 7 & 0x7F;
    tx[id][2] = ang & 0x7F;
    
    for(i=0;i<3;i++){
        servo.putc(tx[id][i]);
    }
}   