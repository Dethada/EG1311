#include <Servo.h>

#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOTOR_PIN1 6
#define MOTOR_PIN2 5
#define SERVO_PIN 2

const float SPEED_OF_SOUND = 0.0345; // in cm/μs
const float STOPPING_DISTANCE = 5;   // in cm
Servo servo;

void setup() {
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode (ECHO_PIN, INPUT);
  Serial.begin(9600);
  servo.attach(SERVO_PIN, 660, 2400);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // delay 10μs
  digitalWrite(TRIG_PIN, LOW);
  int microsecs = pulseIn(ECHO_PIN, HIGH);
  float detected_distance = microsecs*SPEED_OF_SOUND/2; // in cm
  Serial.println(detected_distance);
  if (detected_distance < STOPPING_DISTANCE) { // stop motor
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, LOW);
    servo.write(180); // trigger slingshot
  } else {
    digitalWrite(MOTOR_PIN1, HIGH);
    digitalWrite(MOTOR_PIN2, HIGH);
  }
  delay(10); // delay 10ms
}
