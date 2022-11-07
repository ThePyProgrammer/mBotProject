#include <MeMCore.h>

MeUltrasonicSensor ultraSensor(PORT_2);

void setup(){
  Serial.begin(9600);  
}

void loop(){
  Serial.println(ultraSensor.distanceCm());
}
