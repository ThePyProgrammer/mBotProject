// INSERT ALL YOUR MAIN LOOP CODE HERE :)
#include <MeMCore.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

uint8_t motorSpeed = 100;

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
  motor1.run(motorSpeed);
  motor2.run(-motorSpeed);
  delay(2000);
  motor1.stop();
  motor2.stop();
}
