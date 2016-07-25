#ifndef _MOTOR_H_
#define _MOTOR_H_

void PIDCtrlRight(float target_speed_r, float velocity_R);
void PIDCtrlLeft(float target_speed_l, float velocity_L);
void Turn(float Angle, float Speed);
void MotorCtrl();
void SwitchMotorCtrl(int flag);

#endif //_MOTOR_H_