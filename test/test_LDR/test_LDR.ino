#define A A0
#define B A1

#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0

//Range Scalar for LDR
float black[3] = {498, 756, 771}; //RGB
float delta[3] = {437, 214, 197}; //RGB

enum Color {
  WHITE,
  RED,
  GREEN,
  BLUE,
  ORANGE,
  PURPLE,
  BLACK
} color = BLACK;

void off(){
  analogWrite(B, Lo);
  analogWrite(A, Lo);
}

void setup(){
  off();
  Serial.begin(9600);
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
    Serial.print("Red: ");
    Serial.println(r);
    //Green
    float g = 255 * (green(1000) - black[1])/delta[1];
    Serial.print("Green: ");
    Serial.println(g);
    //Blue
    float b = 255 * (blue(1000) - black[2])/delta[2];
    Serial.print("Blue: ");
    Serial.println(b);
    decideColor(r, g, b);
    Serial.println(color);
}

void calibrateBlack(){
  //Black
  Serial.println("Show Black");
  delay(3000);
  Serial.println("Reading Black");
  black[0] = red(5000);
  Serial.println(black[0]);
  black[1] = green(5000);
  Serial.println(black[1]);
  black[2] = blue(5000);
  Serial.println(black[2]);
  Serial.println("Done Reading Black");
}

void calibrateWhite(){
  //White
  Serial.println("Show White");
  delay(5000);
  Serial.println("Reading White");
  delta[0] = red(5000) - black[0];
  Serial.println(delta[0]);
  delta[1] = green(5000) - black[1];
  Serial.println(delta[1]);
  delta[2] = blue(5000) - black[2];
  Serial.println(delta[2]);
  Serial.println("Done Reading White");
}

void loop(){
  while(Serial.available() == 0){
    
  }
  String command = Serial.readString();
  if (command == "calibrateB\n") calibrateBlack();
  else if (command == "calibrateW\n") calibrateWhite();
  else if (command == "getColor\n") getColor();
}
