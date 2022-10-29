#define A A0
#define B A1

#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0


void setup(){
//  analogWrite(B, Lo);
//  analogWrite(A, Hi);
  Serial.begin(9600);
}

void loop(){
  while(Serial.available() == 0){
    
  }
  String command = Serial.readString();
  Serial.println(command == "getColor\n");
  if (command == "getColor\n"){
    Serial.println("Reading Color");
    //Green
    analogWrite(B,Lo);
    analogWrite(A,Hi);
    delay(3000);
    float green = analogRead(LDR);
    Serial.println(green);
    //Blue
    analogWrite(B,Hi);
    analogWrite(A,Lo);
    delay(3000);
    float blue = analogRead(LDR);
    Serial.println(blue);
    //Red
    analogWrite(B,Hi);
    analogWrite(A,Hi);
    delay(3000);
    float red = analogRead(LDR);
    Serial.println(red);
    if (green > red && green > blue) Serial.println("Green");
    if (red > green && red > blue) Serial.println("Red");
    if (blue > green && blue > red) Serial.println("Blue");
  }
}
