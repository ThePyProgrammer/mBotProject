#include <Arduino.h>
#include <MeMCore.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

#define V 200
#define period 1800
#define WIDTH 20

enum rotating
{
    ROTATING,
    IDLE
} state = IDLE;

void rotate(float angle)
{
    float t = (period / 360) * angle;
    float v = t > 0 ? V : -V;
    t = t > 0 ? t : -t;
    state = ROTATING;
    motor1.run(v);
    motor2.run(v);
    delay(t);
    motor1.stop();
    motor2.stop();
    state = IDLE;
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int stateButton = analogRead(A7);
    if (stateButton == 0 && state == IDLE)
        rotate(90);
    // 1023 is the other state cuz why not bro
}
