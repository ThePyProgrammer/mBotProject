#include "Arduino.h"
#include <MeMCore.h>

#define MOTOR_SPEED 100

// INSERT ALL YOUR MAIN LOOP CODE HERE :)
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

// Helper Functions
void rotate(float angle)
{
    long time = (long) (1000 * angle / 48);

    // Note that this obeys the formula 
    motor1.run(MOTOR_SPEED);
    motor2.run(MOTOR_SPEED);
    delay(time);
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
    rotate(90.0);
    delay(1000);
}

