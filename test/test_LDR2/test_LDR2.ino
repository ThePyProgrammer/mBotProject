#define A A0
#define B A1

#define LDR A3

#define Hi 255
#define Lo 0

//Range Scalar for LDR
float black[3] = {0, 0, 0}; //RGB
float delta[3] = {1, 1, 1}; //RGB

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

void getColor(){
    //Red
    float r = 255 * (red(5000) - black[0])/delta[0];
    Serial.print("Red: ");
    Serial.println(r);
    //Green
    float g = 255 * (green(5000) - black[1])/delta[1];
    Serial.print("Green: ");
    Serial.println(g);
    //Blue
    float b = 255 * (blue(5000) - black[2])/delta[2];
    Serial.print("Blue: ");
    Serial.println(b);
    decideColor(r, g, b);
    Serial.println(color);
//    if (g > r && g > b) Serial.println("Green");
//    if (r > g && r > b) Serial.println("Red");
//    if (b > g && b > r) Serial.println("Blue");
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
void calibrateWhite() {
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

void decideColor(float redColor, float greenColor, float blueColor) {
  redColor = constrain(redColor, 0, 255);
  greenColor = constrain(greenColor, 0, 255);
  blueColor = constrain(blueColor, 0, 255);
  
  int maxVal = max(redColor, blueColor);
  maxVal = max(maxVal, greenColor);
  
  redColor = constrain(map(redColor, 0, maxVal, 0, 255), 0, 255);
  greenColor = constrain(map(greenColor, 0, maxVal, 0, 255), 0, 255);
  blueColor = constrain(map(blueColor, 0, maxVal, 0, 255), 0, 255);
  
   if (redColor > 250 && greenColor > 250 && blueColor > 250) color = WHITE;//white
   else if (redColor < 25 && greenColor < 25 && blueColor < 25) color = BLACK;//black
   else if (redColor > 200 &&  greenColor > 25 && blueColor < 100) color = ORANGE;//orange
   else if (redColor > 200 &&  greenColor < 100 && blueColor > 200) color = PURPLE;//purple
   else if (redColor > 250 && greenColor < 200 && blueColor < 200) color = RED;//red
   else if (redColor < 200 && greenColor > 250 && blueColor < 200) color = GREEN;//green
   else if (redColor < 200 /*&& greenColor < 200*/ && blueColor > 250) color = BLUE;//blue
   else color = BLACK;//unknown
}

void loop(){
  while(Serial.available() == 0){}
  String command = Serial.readString();
  Serial.println("You typed: "+command);
  if (command == "calibrateWhite\n") calibrateWhite();
  if (command == "calibrateBlack\n") calibrateBlack();
  else if (command == "getColor\n") getColor();
}
