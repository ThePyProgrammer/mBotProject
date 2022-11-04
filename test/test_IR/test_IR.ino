#include <MeMCore.h>

#define IR A2
#define varIR 15
#define avgDel 10

float prev = -1;
float curr = 0;
float delta = 0;
long n = 0;

void setup(){
  Serial.begin(9600);
}

float readAvg(int n, long del){
  float sum = 0;
  for (int i = 0; i < n; i++){
    sum += analogRead(IR);
    delay(del);
  }
  return sum/n;
}

void loop(){
  curr = analogRead(IR);
  Serial.println(curr);
  n++;
}
