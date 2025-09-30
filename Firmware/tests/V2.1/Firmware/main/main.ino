// Debug mode
#define DEBUG true

#include "pinout.h"

void setup() {
  Serial.begin(115200);

  // Configure motor control pins as PWM
  ledcAttachPin(MOTOR_RIGHT_ENB_GPIO, 1);
  ledcAttachPin(MOTOR_MIDDLE_ENB_GPIO, 0);
  ledcAttachPin(MOTOR_LEFT_ENB_GPIO, 2);

  // Setup PWM frequency and resolution (10-bit)
  ledcSetup(0, 1000, 10);
  ledcSetup(1, 1000, 10);
  ledcSetup(2, 1000, 10);

  // Configure motor control pins as outputs
  pinMode(MOTOR_RIGHT_INA_GPIO, OUTPUT);
  pinMode(MOTOR_RIGHT_INB_GPIO, OUTPUT);
  pinMode(MOTOR_MIDDLE_INA_GPIO, OUTPUT);
  pinMode(MOTOR_MIDDLE_INB_GPIO, OUTPUT);
  pinMode(MOTOR_LEFT_INA_GPIO, OUTPUT);
  pinMode(MOTOR_LEFT_INB_GPIO, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);


  ledcWrite(0, 512);
  ledcWrite(1, 255);
  ledcWrite(2, 780);

  digitalWrite(MOTOR_RIGHT_INA_GPIO, HIGH);
  digitalWrite(MOTOR_RIGHT_INB_GPIO, LOW);

  digitalWrite(MOTOR_MIDDLE_INA_GPIO, HIGH);
  digitalWrite(MOTOR_MIDDLE_INB_GPIO, LOW);

  digitalWrite(MOTOR_LEFT_INA_GPIO, HIGH );
  digitalWrite(MOTOR_LEFT_INB_GPIO, LOW );
}

int count = 0;

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  ledcWrite(0, 1024 + count );
  ledcWrite(1, 1024 + count );
  ledcWrite(2, 1024 + count );
  count += 10;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}
