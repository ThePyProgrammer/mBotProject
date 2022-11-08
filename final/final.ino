// Import the relevant MakeBlock Libraries
#include <MeMCore.h>

// Set up all the different components (Motors, Ultrasonic Sensor, MeLineFollower and Buzzer)
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);
MeLineFollower line(PORT_1);
MeBuzzer buzzer;

// Constants
#define V 200 // Speed allocated to the motors (they DO NOT follow this)
#define period 1800 // period of rotation of a motor (empirically derived)
#define SPEED 13 // forward speed of bot in cm/s
#define WIDTH 20 // width of each square box in the given set-up in cm

#define A A0 // input 0 for the decoder
#define B A1 // input 1 for the decoder
#define IR A2 // location of the IR
#define LDR A3 // location of the LDR

// General Hi/Lo definitions
#define Hi 255
#define Lo 0

// |=================================================================================|
// |====================================| ENUMS |====================================|
// |=================================================================================|

/**
 * enum robotState
 * Options:
 *     moving: moving forward/backward
 *     solving: solving where to go (LDR colour sensor)
 *     turning: rotating / correcting
 *     idle: not doing anything (initial state)
 */
enum robotState
{
    moving,
    solving,
    turning,
    idle
} state = idle;

/**
 * enum LDRColors
 * Options:
 *     LDR_OFF: set LED array to off
 *     LDR_RED: switches on Red LED
 *     LDR_GREEN: switches on Green LED
 *     LDR_BLUE: switches on Blue LED
 */
enum LDRColors
{
    LDR_OFF,
    LDR_RED,
    LDR_GREEN,
    LDR_BLUE
};

// We use the pre-implemented "COLORTYPES" enum to denote color detected
COLORTYPES color = BLACK;

// Alpha is the minor course correction angle
float alpha = 0;

// Range Scalar for LDR
float black[3] = {498, 756, 771}; // (in RGB)
float delta[3] = {437, 214, 197}; // (in RGB)

// Denotes whether the motor is running (for stopping the motor)
bool motorRunning = false;

// |=================================================================================|
// |================================| COLOUR SENSOR |================================|
// |=================================================================================|
/**
 * writeLDR: Writes to the LED Array
 *
 * @param b : Writes this value (Hi/Lo) to port B (A1)
 * @param a : Writes this value (Hi/Lo) to port A (A0)
 */
void writeLDR(long b, long a)
{
    analogWrite(B, b);
    analogWrite(A, a);
}

/**
 * LDRColor: Colors LED Array based on Input Signal
 * 
 * @param color : input signal from enum, 
 *                tunes colors based on input
 */
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

/**
 * measureLDR: turns on specific LED for some duration
 * 
 * @param color : Color of LED to turn on (Input Signal)
 * @param duration : Duration for LED to turn on
 * @return float : LDR Reading
 */
float measureLDR(LDRColors color, long duration)
{
    LDRColor(color);
    delay(duration);
    float ret = analogRead(LDR);
    LDRColor(LDR_OFF);
    return ret;
}

/**
 * adjust: adjusts value based on calibrated constraints,
 *         fits values between 1 and 255
 *         small helper function to cut down code cost
 * 
 * @param reading : LDR Reading
 * @param clr : Color index (0 - 2)
 * @return float : adjusted value
 */
float adjust(float reading, int clr)
{
    return constrain(ceil(255 * (reading - black[clr]) / delta[clr]), 0, 255);
}

/**
 * getColor: retrieve the color, set color as such
 * r, g, b values determined via measureLDR, adjust functions
 * specific bounds have been set to determine each color
 * these bounds have been determined based on multi-environmental testing
 * 
 */
void getColor()
{
    // r, g, b values determined via measureLDR, adjust functions
    float r = adjust(measureLDR(LDR_RED, 1000), 0);
    float g = adjust(measureLDR(LDR_GREEN, 1000), 1);
    float b = adjust(measureLDR(LDR_BLUE, 1000), 2);

    // specific bounds have been set to determine each color
    // these bounds have been determined based on multi - environmental testing
    if (r >= 250 && g >= 250 && b >= 250)
            color = WHITE;
    else if (r > 250 && g < 150 && b < 130)
        color = RED;
    else if (r > 250 && g > 150 && b < 150)
        color = ORANGE;
    else if (r < 200 && g > 200 && b > 230)
        color = BLUE;
    else if (r < 105 && g > 150 && b > 100)
        color = GREEN;
    else if (r > 199 && g < 200 && b > 180)
        color = PURPLE;
    else
        color = BLACK;
}

// |=================================================================================|
// |================================| MBOT MOVEMENT |================================|
// |=================================================================================|

/**
 * rotation: Rotates based on given angle
 * 
 * @param angle : Angle (in degrees) leftward
 */
void rotation(float angle)
{
    // computes time elapsed based on angle given
    float t = (period / 360) * angle;
    // helper functions for absolute value in case angle < 0
    float v = t > 0 ? V : -V; // final speed of motor
    t = t > 0 ? t : -t; // absolute value of time elapsed
    motor1.run(v);
    motor2.run(v);
    delay(t);
    motor1.stop();
    motor2.stop();
}

/**
 * runMotor: Given motorRunning is false, the motor is run
 */
void runMotor()
{
    if (!motorRunning)
    {
        motorRunning = true; // Reset value for the sake of consistency
        motor1.run(-(V + 55)); // Moves a bit faster than the original speed (to speed up the mBot)
        motor2.run((V + 55)....);
    }
}

/**
 * stopMotor: Given motorRunning is true, the motor is stopped
 */
void stopMotor()
{
    if (motorRunning)
    {
        motorRunning = false; // Reset value for the sake of consistency
        motor1.stop();
        motor2.stop();
    }
}

/**
 * move: Move the robot by some distance in cm
 * 
 * @param distance : distance to be travelled in cm
 */
void move(float distance)
{
    float t = distance / SPEED;
    float v = distance > 0 ? V : -V;
    t = t > 0 ? t : -t;
    motor1.run(-v);
    motor2.run(v);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}

// |=================================================================================|
// |===============================| RANDOM FUNCTIONS |==============================|
// |=================================================================================|
/**
 * playMusic: Plays Victory Music (Imperial March)
 */
void playMusic()
{
    buzzer.tone(440, 500); // Note A4
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(349, 376); // Note F4
    buzzer.tone(523, 126); // Note C5
}

/**
 * getUlDist: Get Distance from Ultrasonic Sensor
 *
 * @return float Distance Measured by Ultrasonic Sensor
 */
float getUlDist()
{
    return ultraSensor.distanceCm();
}

// |=================================================================================|
// |===============================| SOLVER FUNCTIONS |==============================|
// |=================================================================================|
void solve()
{
    switch (color)
    {
    case WHITE: // White Tile
        playMusic();
        break;
    case RED: // Red Tile
        rotation(90); // Rotates Left By 90 degrees
        break;
    case GREEN: // Green Tile
        rotation(-90); // Rotates Right by 90 degrees
        break;
    case ORANGE: // Orange Tile
        // Determine which direction to rotate to prevent collision with wall
        if (getUlDist() > 13)
            rotation(180); // Turns from the left (side with the Ultrasonic Sensor) since the Ultrasonic distance is very large
        else
            rotation(-180); // Turns from the right (side with the IR Sensor) since the Ultrasonic distance is smaller
        break;
    case BLUE: // Light Blue Tile
        rotation(-90); // Rotates Right by 90 degrees
        move(WIDTH - 7); // Moves forward (does not completely cover the width as rotation is not about the centre)
        rotation(-90); // Rotates Right by 90 degrees
        break;
    case PURPLE: // Purple Tile
        rotation(90); // Rotates Left By 90 degrees
        move(WIDTH - 7); // Moves forward (does not completely cover the width as rotation is not about the centre)
        rotation(90); // Rotates Left By 90 degrees
        break;
    default: // Undefined Color (ideally "BLACK")
        break;
    }
}

void setup()
{
    LDRColor(LDR_OFF); // switch off the LED Array
    state = moving; // shift to movement state
}

void loop()
{
    // Read from MeLine Follower, IR and Ultrasonic Sensor
    int sensorState = line.readSensors();
    float IRread = analogRead(IR);
    float UlRead = getUlDist();

    // State handler
    if (sensorState == S1_IN_S2_IN)
        state = solving; // just wait for it to solve
    else if (UlRead < 20 && UlRead > 13)
    {
        state = turning; // note that it's turning
        alpha = 1;       // moving towards ultrasonic sensor (large distance observed)
    }
    else if (UlRead < 6.5)
    {
        state = turning; // note that it's turning
        alpha = -1;      // moving towards IR sensor (small distance observed)
    }
    else if (UlRead > 20 && IRread > 970)
    {
        // (large distance observed from ultrasonic sensor, IR sensor is very close to wall)
        state = turning; // note that it's turning
        alpha = 1;       // moving towards ultrasonic sensor
    }
    else
        state = moving; // not correcting course

    switch (state)
    {
    case moving:
        runMotor(); // just let it keep running
        break;
    case solving:
        stopMotor(); // stop to solve
        getColor(); // get color and store in color variable
        solve(); // solve for what action to do
        break;
    case turning:
        stopMotor(); // stop to course correct
        rotation(alpha); // rotate by a small angle (1 degree)
        break;
    case idle:
        stopMotor(); // just let it stop
        break;
    }
}
