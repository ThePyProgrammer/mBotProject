#include "Arduino.h"
#include <MeMCore.h>

#define MOTOR_SPEED 100

// INSERT ALL YOUR MAIN LOOP CODE HERE :)
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

// UNIT TESTS

void moveM1() {
    motor1.run(MOTOR_SPEED);
    delay(1000);
    motor1.stop();
}

void moveM2() {
    motor2.run(MOTOR_SPEED);
    delay(1000);
    motor2.stop();
}

void forward() {
    motor1.run(MOTOR_SPEED);
    motor2.run(-MOTOR_SPEED);
    delay(1000);
    motor1.stop();
    motor2.stop();
}

void rotate() {
    motor1.run(MOTOR_SPEED);
    motor2.run(MOTOR_SPEED);
    delay(1000);
    motor1.stop();
    motor2.stop();
}

void setup()
{
    // put your setup code here, to run once:
}

void loop()
{
    // put your main code here, to run repeatedly:
    moveM1();
    delay(3000);
    moveM2();
    delay(3000);
    forward();
    delay(3000);
    rotate();
    delay(3000);
}
