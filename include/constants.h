#include <Arduino.h>
#include <MeMCore.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);

#define A A0
#define B A1

#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0

#define MOTOR_SPEED 150
#define PERIOD 2600
#define SPEED 15.73
#define dt 6244


// ============================= READ FUNCTIONS ================================
/**
 * Get ultrasound sensor distance reading
 *
 * @return distance reading in cm
 */
float ultraSoundDistance()
{
    return ultraSensor.distanceCm();
}

/**
 * Get IR Voltage
 * 
 * @return in Volts
 */
int readIR() {
    return analogRead(IR);
}

// ============================= ACTION FUNCTIONS ==============================
/**
 * Rotates the mBot about the axis perpendicular upward from the midpoint of the two wheels by some angle
 *
 * @param angle in degrees
 */
void rotate(float angle) {
    float t = (PERIOD / 360) * angle;
    float v = t > 0 ? MOTOR_SPEED : -MOTOR_SPEED;
    t = t > 0 ? t : -t;
    motor1.run(v);
    motor2.run(v);
    delay(t);
    motor1.stop();
    motor2.stop();
}

/**
 * Moves the mBot forward by some distance
 * 
 * @param distance in cm
 */
void forward(float distance) {
    float t = distance / SPEED;
    float v = distance > 0 ? MOTOR_SPEED : -MOTOR_SPEED;
    t = t > 0 ? t : -t;
    motor1.run(v);
    motor2.run(-v);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}


