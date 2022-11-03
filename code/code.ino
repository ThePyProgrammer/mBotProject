// INSERT ALL YOUR MAIN LOOP CODE HERE :)

#include <MeMCore.h>

//Define the Me things
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_2);
MeLineFollower line(PORT_1);
MeBuzzer buzzer;

//Some constants
#define V 150
#define period 2550
#define frontSpeed 15.73
#define T 6244

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
float black[3] = {527, 776, 764}; //RGB
float delta[3] = {416, 199, 205}; //RGB

long n = 0;
robotState state = idle;

void playMusic(){
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
//
//    buzzer.tone(440, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(523, 126);
//    buzzer.tone(440, 1000);
//
//    buzzer.tone(659, 500);
//    buzzer.tone(659, 500);
//    buzzer.tone(659, 500);
//    buzzer.tone(698, 376);
//    buzzer.tone(523, 126);
//
//    buzzer.tone(415, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(523, 126);
//    buzzer.tone(440, 1000);
//
//    buzzer.tone(880, 500);
//    buzzer.tone(440, 376);
//    buzzer.tone(440, 126);
//    buzzer.tone(880, 500);
//    buzzer.tone(831, 376);
//    buzzer.tone(784, 126);
//
//    buzzer.tone(740, 166);
//    buzzer.tone(698, 166);
//    buzzer.tone(740, 166);
//    delay(250);
//    buzzer.tone(466, 250);
//    buzzer.tone(622, 500);
//    buzzer.tone(587, 376);
//    buzzer.tone(554, 126);
//
//    buzzer.tone(523, 166);
//    buzzer.tone(494, 166);
//    buzzer.tone(523, 166);
//    delay(250);
//    buzzer.tone(349, 250);
//    buzzer.tone(415, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(415, 126);
//
//    buzzer.tone(523, 500);
//    buzzer.tone(440, 376);
//    buzzer.tone(523, 126);
//    buzzer.tone(659, 1000);
//
//    buzzer.tone(880, 500);
//    buzzer.tone(440, 376);
//    buzzer.tone(440, 126);
//    buzzer.tone(880, 500);
//    buzzer.tone(831, 376);
//    buzzer.tone(784, 126);
//
//    buzzer.tone(740, 166);
//    buzzer.tone(698, 166);
//    buzzer.tone(740, 166);
//    delay(250);
//    buzzer.tone(466, 250);
//    buzzer.tone(622, 500);
//    buzzer.tone(587, 376);
//    buzzer.tone(554, 126);
//
//    buzzer.tone(523, 166);
//    buzzer.tone(494, 166);
//    buzzer.tone(523, 166);
//    delay(250);
//    buzzer.tone(349, 250);
//    buzzer.tone(415, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(523, 126);
//
//    buzzer.tone(440, 500);
//    buzzer.tone(349, 376);
//    buzzer.tone(523, 126);
//    buzzer.tone(440, 1000);
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
  else if (r > 250 && g < 150 && b < 130) color = RED;
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
    motor1.run(-v*1.25);
    motor2.run(v*1.25);
    delay(1000 * t);
    motor1.stop();
    motor2.stop();
}

void setup() {
    Serial.begin(9600);
    off();
    state = moving;
}

void solve(){
  switch(color){
    case WHITE:
    //Play music
    playMusic();
    break;
    case RED:
    rotation(90);
    break;
    case GREEN:
    rotation(-90);
    break;
    case ORANGE:
    if (getDistance() > 13) rotation(180);
    else rotation(-180);
    break;
    case BLUE:
    rotation(-90);
    moveRobot(19);
    rotation(-90);
    break;
    case PURPLE:
    rotation(90);
    moveRobot(18);
    rotation(90);
    break;
    default:
    break;
  }
}

void loop() {
  //MeLine Follower
  int sensorState = line.readSensors();

  float IRread = analogRead(IR);
  float UlRead = getDistance();

  //State handler
  if (sensorState == S1_IN_S2_IN) state = solving;
  else if (UlRead > 13 && UlRead < 20){
    state = turning;
    alpha = 5;
  }
  else if (UlRead > 20 && (IRread > 930 && IRread < 940)) {
    state = turning;
    alpha = 1;
  }
  else if (UlRead < 6.5){
    state = turning;
    alpha = -1;
  }
  else state = moving;

  switch(state){
    case moving:
    moveRobot(unit);
    break;
    case solving:
    getColor();
    solve();
    break;
    case turning:
    rotation(alpha);
    break;
    case idle:
    break;
  }
}
