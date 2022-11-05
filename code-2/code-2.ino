// INSERT ALL YOUR MAIN LOOP CODE HERE :)

#include <MeMCore.h>

//Define the Me things
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);
MeLineFollower line(PORT_1);
MeBuzzer buzzer;

//Some constants
#define V 200
#define period 1800
#define frontSpeed 13
#define T 6244
#define W 20
#define bgIR 940

#define A A0
#define B A1
#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0

//ENUMS
enum robotState{
  moving,
  solving,
  turning,
  idle
};

COLORTYPES color = BLACK;

//Global variables
long start = -1;
long elapsedTime = 0;
float previous = 0;
float unit = 1;
float alpha = 0;

//Range Scalar for LDR
float black[3] = {498, 756, 771}; //RGB
float delta[3] = {437, 214, 197}; //RGB

bool motorRunning = false;

long n = 0;
robotState state = idle;

void playMusic(){
//    buzzer.tone(440, 500);
//    buzzer.tone(440, 500);
//    buzzer.tone(440, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(523, 126);
}

//LDR Things

void off(){
  analogWrite(B, Lo);
  analogWrite(A, Lo);
}

float green(long duration){
  analogWrite(B,Lo);
  analogWrite(A,Hi);
  delay(duration);
  float ret = analogRead(LDR);
  off();
  return ret;
}

float red(long duration){
  analogWrite(B,Hi);
  analogWrite(A,Hi);
  delay(duration);
  float ret = analogRead(LDR);
  off();
  return ret;
}

float blue(long duration){
  analogWrite(B,Hi);
  analogWrite(A,Lo);
  delay(duration);
  float ret = analogRead(LDR);
  off();
  return ret;
}

void decideColor(float r, float g, float b) {
  r = constrain(ceil(r), 0, 255);
  g = constrain(ceil(g), 0, 255);
  b = constrain(ceil(b), 0, 255);

  if (r >= 250 && g >= 250 &&  b >= 250) color = WHITE;
  else if (r > 250 && g < 150 && b < 150) color = RED;
  else if (r > 250 && g > 150 && b < 150) color = ORANGE;
  else if (r < 200 && g > 200 && b > 230) color = BLUE;
  else if (r < 105 && g > 150 && b > 100) color = GREEN;
  else if (r > 199 && g < 200 && b > 180) color = PURPLE;
  else color = BLACK;
}


void getColor(){
    //Red
    float r = 255 * (red(1000) - black[0])/delta[0];
    //Green
    float g = 255 * (green(1000) - black[1])/delta[1];
    //Blue
    float b = 255 * (blue(1000) - black[2])/delta[2];
    decideColor(r, g, b);
}
//End LDR Things

//Movement things
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

void runMotor(){
  if (!motorRunning){
    motorRunning = true;
    motor1.run(-V);
    motor2.run(V); 
  }
}

void stopMotor(){
  if (motorRunning){
    motorRunning = false;
    motor1.stop();
    motor2.stop(); 
  }
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
//End Movement Things

//Ultrasonic sensor function
float getDistance(){
  return ultraSensor.distanceCm();
}
//End Ultrasonic sensor function

//Solver things
void solve(){
  switch(color){
    case WHITE: playMusic(); break;
    case RED: rotation(90); break;
    case GREEN: rotation(-90); break;
    case ORANGE:
      //Determine which direction to rotate to prevent collision with wall
      if (getDistance() > 13) rotation(180);
      else rotation(-180);
      break;
    case BLUE:
      rotation(-90);
      moveRobot(W);
      rotation(-90);
      break;
    case PURPLE:
      rotation(90);
      moveRobot(W);
      rotation(90);
      break;
    default: break;
  }
}
//End solver things

void setup() {
    Serial.begin(9600);
    off();
    state = moving;
}

void loop() {
  //MeLine Follower
  int sensorState = line.readSensors();

  float IRread = analogRead(IR);
  float UlRead = getDistance();
  
  //State handler
  if (sensorState == S1_IN_S2_IN) state = solving;
  else if (UlRead < 20 && UlRead > 13){
    state = turning;
    alpha = 1;
  }
  else if (UlRead < 6.5){
    state = turning;
    alpha = -1;
  }
  else if (UlRead > 20 && IRread > 930){
    state = turning;
    alpha = 1;
  }
  else state = moving;

  switch(state){
    case moving: runMotor(); break;
    case solving:
      stopMotor();
      getColor();
      solve();
      break;
    case turning: stopMotor(); rotation(alpha); break;
    case idle: stopMotor(); break;
  }
}
