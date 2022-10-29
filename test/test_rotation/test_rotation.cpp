#include "Arduino.h"
#include <MeMCore.h>
#include <unity.h>

#define MOTOR_SPEED 200

// INSERT ALL YOUR MAIN LOOP CODE HERE :)
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

// UNIT TESTS

void moveM1()
{ // move the left motor, i.e. turn right
    motor1.run(MOTOR_SPEED);
    delay(1000);
    motor1.stop();
}

void moveM2() { // move the right motor, i.e. turn left
    motor2.run(MOTOR_SPEED);
    delay(1000);
    motor2.stop();
}

void forward() {
    motor1.run(-MOTOR_SPEED);
    motor2.run(MOTOR_SPEED);
    delay(1000);
    motor1.stop();
    motor2.stop();
}

void backward()
{
    motor1.run(MOTOR_SPEED);
    motor2.run(-MOTOR_SPEED);
    delay(1000);
    motor1.stop();
    motor2.stop();
}

void left() {
    float period = 1567.33333f;
    float time = period / 4.0;
    motor1.run(MOTOR_SPEED);
    motor2.run(MOTOR_SPEED);
    delay(1000);
    motor1.stop();
    motor2.stop();
}

void setup()
{
    // put your setup code here, to run once:
    UNITY_BEGIN();
    moveM1();
    delay(3000);
    moveM2();
    delay(3000);
    forward();
    delay(3000);
    backward();
    delay(3000);
    for(int i = 0; i < 5; i++) {
        left();
        delay(3000);
    }
    UNITY_END();
}

void loop()
{
    // put your main code here, to run repeatedly:
}
