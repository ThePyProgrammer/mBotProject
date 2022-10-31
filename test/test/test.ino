#define A A0
#define B A1

#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0

//Range Scalar for LDR
float black[3] = {0, 0, 0}; //RGB
float white[3] = {1, 1, 1}; //RGB

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
    float r = (red(3000) - black[0])/white[0];
    Serial.print("Red: ");
    Serial.println(r);
    //Green
    float g = (green(3000) - black[1])/white[1];
    Serial.print("Green: ");
    Serial.println(g);
    //Blue
    float b = (blue(3000) - black[2])/white[2];
    Serial.print("Blue: ");
    Serial.println(b);
    if (g > r && g > b) Serial.println("Green");
    if (r > g && r > b) Serial.println("Red");
    if (b > g && b > r) Serial.println("Blue");
}

void calibrate(){
  //Black
  Serial.println("Show Black");
  delay(3000);
  Serial.println("Reading Black");
  black[0] = red(3000);
  Serial.println(black[0]);
  black[1] = green(3000);
  Serial.println(black[1]);
  black[2] = blue(3000);
  Serial.println(black[2]);
  Serial.println("Done Reading Black");

  //White
  Serial.println("Show White");
  delay(3000);
  Serial.println("Reading White");
  white[0] = red(3000);
  Serial.println(white[0]);
  white[1] = green(3000);
  Serial.println(white[1]);
  white[2] = blue(3000);
  Serial.println(white[2]);
  Serial.println("Done Reading White");
}

void loop(){
  while(Serial.available() == 0){
    
  }
  String command = Serial.readString();
  if (command == "calibrate\n") calibrate();
  else if (command == "getColor\n") getColor();
}
