#include <Arduino.h>
#include <MeMCore.h>

#define A A0
#define B A1
#define LDR A3

#define Hi 255
#define Lo 0

enum LDRColors
{
    LDR_OFF,
    LDR_RED,
    LDR_GREEN,
    LDR_BLUE
} state = LDR_OFF;

LDRColors colors[3] = {LDR_RED, LDR_GREEN, LDR_BLUE};
String names[3] = {"RED", "GREEN", "BLUE"};

int colorState = 0;

long time = millis(), start = millis();

void writeLDR(long b, long a)
{
    analogWrite(B, b); // Write to the MSB
    analogWrite(A, a); // Write ot the LSB
}

void LDRColor(LDRColors color)
{
    switch (color)
    {
    case LDR_OFF:
        writeLDR(Lo, Lo); // 00 is OFF
        break;
    case LDR_RED:
        writeLDR(Hi, Hi); // 11 is RED
        break;
    case LDR_GREEN:
        writeLDR(Lo, Hi); // 01 is GREEN
        break;
    case LDR_BLUE:
        writeLDR(Hi, Lo); // 10 is BLUE
        break;
    }
}


void setup(){
    Serial.begin(9600);
}

void loop()
{
    int stateButton = analogRead(A7);
    if (stateButton == 0) {
        if(state == LDR_OFF) {
            state = colors[colorState];
            Serial.print("Start of ");
            Serial.println(names[colorState]);
            start = millis();
        } else {
            state = LDR_OFF;
            Serial.print("End of ");
            Serial.println(names[colorState]);
            colorState = ((colorState+1) % 3);
        }
        LDRColor(state);
    }
    time = millis();
    if(state != LDR_OFF) {
        Serial.print(time - start);
        Serial.print(",");
        Serial.println(analogRead(LDR));
        delay(1);
    }
}
