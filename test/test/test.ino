#define A A0
#define B A1

#define IR A2
#define LDR A3

#define Hi 255
#define Lo 0


void setup(){
  analogWrite(B, Hi);
  analogWrite(A, Hi);
  Serial.begin(9600);
}

void loop(){
  Serial.println(analogRead(IR));
}
