// INSERT ALL YOUR MAIN LOOP CODE HERE :)
#include <Arduino.h>
#include <MeMCore.h>

//Define the Me things
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);
MeLineFollower line(PORT_1);
MeBuzzer buzzer;

//Some constants
#define V 150
#define PERIOD 2550
#define SPEED 15.73
#define WIDTH 20
#define BG_IR 940

#define A A0
#define B A1
#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0

//ENUMS
enum robotState {
  moving,
  solving,
  turning,
  idle
} state = idle;

enum LDRColors {
  LDR_OFF,
  LDR_RED,
  LDR_GREEN,
  LDR_BLUE
};

COLORTYPES color = BLACK;

#define UNIT 1.0
#define ALPHA 0.0

//Range Scalar for LDR
float black[3] = {527, 776, 764}; // (in RGB)
float delta[3] = {416, 199, 205}; // (in RGB)

void playMusic() {
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
}

void writeLDR(long b, long a) {
  analogWrite(B, b);
  analogWrite(A, a);
}

void LDRColor(LDRColors color) {
  switch(color) {
    case LDR_OFF:
      writeLDR(Lo, Lo); break;
    case LDR_RED:
      writeLDR(Hi, Hi); break;
    case LDR_GREEN:
      writeLDR(Lo, Hi); break;
    case LDR_BLUE:
      writeLDR(Hi, Lo); break;
  }
}

float measureLDR(LDRColors color, long duration) {
  LDRColor(color);
  delay(duration);
  float ret = analogRead(LDR); // Read takes ~ 3 seconds
  LDRColor(LDR_OFF);
  return ret;
}

float adjust(float reading, int clr) {
  return constrain(ceil(255 * (reading - black[clr]) / delta[clr]), 0, 255);
}

void getColor(){
    float r = adjust(measureLDR(LDR_RED, 1000), 0);
    float g = adjust(measureLDR(LDR_GREEN, 1000), 1);
    float b = adjust(measureLDR(LDR_BLUE, 1000), 2);

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

void rotation(float angle){
  float t = (PERIOD/360) * angle;
  float v = t > 0 ? V : -V;
  t = t > 0 ? t : -t;
  motor1.run(v);
  motor2.run(v);
  delay(t);
  motor1.stop();
  motor2.stop();
}

void move(float distance){
    float t = 1000 * distance / SPEED;
    float v = distance > 0 ? V : -V;
    t = t > 0 ? t : -t;
    motor1.run(-v);
    motor2.run(v);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}

float getUlDist(){
  return ultraSensor.distanceCm();
}

void solve(){
  switch(color){
    case WHITE: playMusic(); break;
    case RED: rotation(90); break;
    case GREEN: rotation(-90); break;
    case ORANGE:
      //Determine which direction to rotate to prevent collision with wall
      if (getUlDist() > 13) rotation(180);
      else rotation(-180);
      break;
    case BLUE:
      rotation(-90);
      move(WIDTH);
      rotation(-90);
      break;
    case PURPLE:
      rotation(90);
      move(WIDTH);
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
  float UlRead = getUlDist();
  
  bool UlWall = UlRead =< WIDTH;
  bool IRWall = IRread < BG_IR;
  
  //State handler
  if (sensorState == S1_IN_S2_IN) state = solving;
  else if (UlWall){
    if (UlRead =< 20 && UlRead > 13){
      state = turning;
      alpha = 1;
    }
    else if (UlRead < 6.5){
      state = turning;
      alpha = -1;
    }
  }
  else if(IRWall && IRread > 930){
    state = turning;
    alpha = -1;
  }
  else state = moving;

  switch(state){
    case moving: move(UNIT); break;
    case solving:
      getColor();
      solve();
      break;
    case turning: rotation(ALPHA); break;
    case idle: break;
  }
}
