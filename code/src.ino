// INSERT ALL YOUR MAIN LOOP CODE HERE :)
#include "MeOrion.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

#define MOTOR_SPEED 100

void rotate(float);



void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  rotate(6.9);
}


//Helper Functions
void rotate(float angle)
{
  motor1.run(MOTOR_SPEED);
  motor2.run(-MOTOR_SPEED);
  delay(2000);
  motor1.stop();
  motor2.stop();
}
