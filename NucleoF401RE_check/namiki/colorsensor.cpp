#include "mbed.h"

I2C colorSensor(D14,D15);
Serial pc(USBTX,USBRX);
int R,G,B,IR;       //赤，緑,青，赤外

#define BLUE 1
#define YELLO 2
#define RED 3
#define NO_BALL 4

//途中にあるwaitはおそらく最低1秒はほしいかなー
//短いと値取得が不可能だった

//カラーセンサの値を取得する関数
void  get_color_value() {
    char data[8];
    int tmp;
    char com[2];
    int address = 0x2A<<1;
    /*//start condition
    colorSensor.start();
    //device address
    colorSensor.write(0x54);
    //controll register
    colorSensor.write(0x00);
    //ADC reset enable
    colorSensor.write(0x84);

     //manual timing register 01
     colorSensor.write(0x01);
        
     //値変えれば変わるはず・・・（多分）
     //set manual timing reister 01
     colorSensor.write(0x03);
     //manual timing register 02
     colorSensor.write(0x02);
     //set manual timing register 02
     colorSensor.write(0xC0);
                
    //start condition
    colorSensor.start();
    //device address
    colorSensor.write(0x54);
    //controll register
    colorSensor.write(0x00);
    //ADC reset disable
    colorSensor.write(0x04);
    //stop condition
    colorSensor.stop();
    
    //ここのwait最低何秒だろー？
    wait(1.0);
        
    //start condition
    colorSensor.start();
    //device address
    colorSensor.write(0x54);
    //output databyte set
    colorSensor.write(0x03);
            
    //read sensor
    colorSensor.read(0x55, data, 8);*/
    
    com[0]=0x00;
    com[1]=0x84;
    colorSensor.write(address,com,2,true);
    com[0]=0x01;
    com[1]=0x0C;
    colorSensor.write(address,com,2,true);
    com[0]=0x02;
    com[1]=0x30;
    colorSensor.write(address,com,2,true);
    com[0]=0x00;
    com[1]=0x04;
    colorSensor.write(address,com,2);
    wait(2.5);
    
    com[0]=0x03;
    colorSensor.write(address & 0xFE,com,1,true);
    //read data
    colorSensor.read(address,data,8,true);
    for(int i=0; i<8; i=i+2){
        tmp = data[i] << 8;  //16bitで出すため8bitシフト
        tmp += data[i+1];  
        if(i== 0){
            R = tmp;
        }else if(i == 2){
            G = tmp;
        }else if(i == 4){
            B = tmp;
        }else {
            IR = tmp;
        }
    }
    colorSensor.stop();
}

//センサで取得した値から何色かを識別する関数
int what_color(){
    get_color_value();
    if(IR > 40){
        //ボールを持っているかの確認
        return NO_BALL;
       
    }
    else{
        if(R > 50 && G > 30 && B < 50){
            //赤色だよー
           // printf("\tred\n");
            return RED;
        }
        
        //ここの閾値がなんかなー
        else if(R > 90 && G > 100 && B > 40){       // if(R > 90 && G > 100 && B > 40)
            //それ以外は黄色じゃね？
            //printf("\tyello\n");
            return YELLO;
        }
        else {      //if( R < 50 && G > 30 && B > 50)
            //青色だよー
            //printf("\tblue\n");
            return BLUE;
        }
    }
}
int main(){
    int ball_color;
    colorSensor.frequency(100000);
    while(1){
        ball_color = what_color();
        pc.printf("BallColor:%d R:%d G:%d B:%d IR:%d\r\n",ball_color,R,G,B,IR);
    }
}
