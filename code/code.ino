// INSERT ALL YOUR MAIN LOOP CODE HERE :)

#include <MeMCore.h>

//Define the Me things
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);
MeLineFollower line(PORT_1);

//Some constants
#define V 150
#define period 2600
#define frontSpeed 15.73
#define T 6244

enum robotState{
  moving,
  solving,
  turning,
  idle
};

//Global variables
long start = -1;
long elapsedTime = 0;
float previous = 0;
float unit = 1;
robotState state = idle;

void rotation(float angle){
  float t = (period/360) * angle;
  float v = t > 0 ? V : -V;
  t = t > 0 ? t : -t;
  motor1.run(v);
  motor2.run(v);
  delay(t);
  motor1.stop();
  motor2.stop();
}

float getDistance(){
  return ultraSensor.distanceCm();
}

void moveRobot(float distance){
    float t = distance / frontSpeed;
    float v = distance > 0 ? V : -V;
    t = t > 0 ? t : -t;
    motor1.run(-v);
    motor2.run(v);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}

void setup() {
    Serial.begin(9600);
    state = moving;
}

void loop() {
  int sensorState = line.readSensors();
  switch(sensorState){
    case S1_IN_S2_IN: state = idle; break;
    default: state = moving; break;
  }
  switch(state){
    case moving:
    moveRobot(unit);
    break;
    case solving:
    break;
    case turning:
    break;
    case idle:
    break;
  }
  //Time measure
  long currentTime = 0;
  currentTime = micros();
  elapsedTime = start > 0 ? micros() - start : 0;
  start = currentTime;
}
