#include <MeMCore.h>

#define V 200
#define period 1799
#define corr 1600

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

void setup(){
  motor1.run(V);
  motor2.run(V);
  Serial.begin(9600);
}

void rotation(float angle){
  unsigned long t = (period/360) * angle;
  float v = t > 0 ? V : -V;
  t = t > 0 ? t : -t;
  motor1.run(v);
  motor2.run(v);
  delay(t);
  delayMicroseconds(corr);
  motor1.stop();
  motor2.stop();
}

void loop(){
  rotation(90);
  delay(1000);
}
