// INSERT ALL YOUR MAIN LOOP CODE HERE :)

#include <MeMCore.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);

#define V 150
#define period 2600
#define frontSpeed 15.73
#define T 6244

long start = -1;
long elapsedTime = 0;
float previous = 0;
long n = 0;

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

float getUlCorAngle(float d, float prev, float dt){
  float dy = d - prev;
  float ds = frontSpeed * dt * 0.001;
  return asin(dy/ds);
}

void moveRobot(float distance){
    float t = distance / frontSpeed;
    float v = distance > 0 ? V : -V;
    t = t > 0 ? t : -t;
    motor1.run(v);
    motor2.run(-v);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}

void setup() {
  // put your setup code here, to run once: 
    Serial.begin(9600);
//  motor1.run(-V);
//  motor2.run(V);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Rotations
//  rotation(-90);
//  delay(1000);
    
  //Distance
  float d = getDistance();
//  Serial.println(d);


 //Time measure
 long currentTime = 0;
 currentTime = micros();
 elapsedTime = start > 0 ? micros() - start : 0;
 start = currentTime;
// Serial.println(elapsedTime);

 //Correction Angle
 if (n % 10000){
  Serial.println(d - previous);
  previous = d;
 }
// Serial.println(getUlCorAngle(d, previous, elapsedTime));
// moveRobot(10);
  n++;
}
