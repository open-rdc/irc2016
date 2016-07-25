#include "Function.h"
#include "QEI.h"

const float PI = 3.1415926;
const float InnerGearRatio = 4.4;        //内側ギヤ
const float OutSideGearRatio = 5.9375;     //外側ギヤ
const float WheelRadius = 25.5;       //タイヤ半径[mm]
const float Thred = 135.0;        //トレッド[mm]未定？
const int MaxSpeed = 1000;       //1[m/s]未定？
const int PPR = 100;        //Pulse Per Revolution in encoder
const float KP_R = 3.0;
const float KI_R = 0.1;
const float KD_R = 0.1;
const float KP_L = 3.0;
const float KI_L = 0.1;
const float KD_L = 0.1;
const float PwmCtrlPeriod = 0.00005;    //20[kHz]
const float CtrlPeriod = 0.02;       //2[ms]?
const float QEI_X2_ENCODING = 2.0;

/*pin割り当て*/
PwmOut MotorRight(D6);
PwmOut MotorLeft(D5);
DigitalOut HBridgeRight(D7);
DigitalOut HBridgeLeft(D4);

/*QEI*/
QEI Pulse_r(D2, D3, NC, PPR, QEI::X2_ENCODING);
QEI Pulse_l(D13,D12, NC, PPR, QEI::X2_ENCODING);

/*グローバル関数*/
float theta_R, theta_L;                     //角度(タイヤ)
float theta_old_R, theta_old_L;             //一個前の角度（タイヤ）
float omega, omega_R, omega_L;              //角速度(車体)
float velocity, velocity_R, velocity_L;     //速度
float x0, x1, y0, y1, theta0, theta1;
float timer;

/*PID制御右モーター*/
void PIDCtrlRight(float target_speed_r,float velocity_R){
    static float e0 = 0, e1 = 0;
    static float ei = 0, ed = 0;
    float CalcVelocity;
    
    HBridgeRight = 1;
    
    e1 = velocity_R - target_speed_r;
    ei += e1;
    ed = e1 - e0;
    e0 = e1;
    if(target_speed_r == 0) ei = 0, ed = 0;
    CalcVelocity = (e1 * KP_R + ei * KI_R + ed * KD_R);
    if(CalcVelocity > MaxSpeed)CalcVelocity = MaxSpeed;
    //if(CalcVelocity > 0)
    if(CalcVelocity < -MaxSpeed)CalcVelocity = -MaxSpeed;
    //if(CalcVelocity < 0)HBridgeRight = 0;
}

/*PID制御左モーター*/
void PIDCtrlLeft(float target_speed_l, float velocity_L){
    static float e0 = 0, e1 = 0;
    static float ei = 0, ed = 0;
    float CalcVelocity;
    
    e1 = velocity_L - target_speed_l;
    ei += e1;
    ed = e1 - e0;
    e0 = e1;
    if(target_speed_l == 0)ei = 0,ed = 0;    
    CalcVelocity =(e1 * KP_L + ei * KI_L + ed * KD_L);
    if(CalcVelocity > MaxSpeed)CalcVelocity = MaxSpeed;
    if(CalcVelocity > 0)HBridgeLeft = 1;        //前に進む
    if(CalcVelocity < -MaxSpeed)CalcVelocity = -MaxSpeed;
    //if(CalcVelocity < 0)HBridgeLeft = 0;        //後ろ
}


void MotorCtrl(){
    theta_R = (Pulse_r.getPulses() * 2 * PI) / ( PPR * InnerGearRatio * OutSideGearRatio * QEI_X2_ENCODING);
    theta_L = (Pulse_l.getPulses() * 2 * PI) / ( PPR * InnerGearRatio * OutSideGearRatio * QEI_X2_ENCODING);
    omega_R = (theta_R - theta_old_R / PwmCtrlPeriod);
    omega_L = (theta_L - theta_old_L / PwmCtrlPeriod);
    velocity_R = omega_R * WheelRadius;
    velocity_L = omega_L * WheelRadius;
    
    /*オドメトリー計算*/
    velocity = (WheelRadius / 2) * (omega_R + omega_L);
    omega = (WheelRadius / Thred) * (omega_R - omega_L);
    theta1 = theta0 + omega * CtrlPeriod;
    x0 = x1; y0 = y1; theta0 = theta1;
    x1 = x0 + velocity * CtrlPeriod * cos(theta1);
    y1 = y0 + velocity * CtrlPeriod * sin(theta1);
    
    PIDCtrlRight(target_speed_r, velocity_R);
    PIDCtrlLeft(target_speed_l, velocity_L);
    
    timer += CtrlPeriod;
}

Ticker TimerInterrupt;

void SwitchMotorCtrl(int flag){
    if(flag == ON){
        TimerInterrupt.attach(&MotorCtrl, CtrlPeriod);
        Pulse_r.reset();
        Pulse_l.reset();
    }else{
        TimerInterrupt.detach();
        Pulse_r.reset();
        Pulse_l.reset();
    }
}