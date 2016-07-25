#include "Function.h"

AnalogIn fr_LL(A0), fr_L(A1), fr_R(A2), fr_RR(A3);      //フォトリフレクタ
float target_speed_r, target_speed_l;

void LineTrace(int cntXline){
    float ll = fr_LL.read();
    float l = fr_L.read();
    float r = fr_R.read();
    float rr = fr_RR.read();
}