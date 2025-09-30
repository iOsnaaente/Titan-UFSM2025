#include <AS5048A.h>

#define SS1 15
#define SS2 17
#define SS3 23

AS5048A angleSensor1(SS1, true);
AS5048A angleSensor2(SS2, true);
AS5048A angleSensor3(SS3, true);


void setup() {
  Serial.begin(115200);
  angleSensor1.begin();
  angleSensor2.begin();
  angleSensor3.begin();
}

void loop() {
  Serial.println( angleSensor1.getRotationInDegrees());
  Serial.println( angleSensor2.getRotationInDegrees());
  Serial.println( angleSensor3.getRotationInDegrees());
  delay(250);
}
