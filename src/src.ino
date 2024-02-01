#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOTOR1_FORWARD_PIN 6
#define MOTOR1_BACKWARD_PIN 7
#define MOTOR2_FORWARD_PIN 5
#define MOTOR2_BACKWARD_PIN 4

const float SPEED_OF_SOUND = 0.0345; // in cm/μs
const float STOPPING_DISTANCE = 5;   // in cm

bool ball_launched = false;

void setup() {
  // Motor
  pinMode(MOTOR1_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR1_BACKWARD_PIN, OUTPUT);
  pinMode(MOTOR2_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR2_BACKWARD_PIN, OUTPUT);

  // Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode (ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void forward() {
  digitalWrite(MOTOR1_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR1_BACKWARD_PIN, LOW);

  digitalWrite(MOTOR2_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR2_BACKWARD_PIN, LOW);
}

void backward() {
  digitalWrite(MOTOR1_FORWARD_PIN, LOW);
  digitalWrite(MOTOR1_BACKWARD_PIN, HIGH);

  digitalWrite(MOTOR2_FORWARD_PIN, LOW);
  digitalWrite(MOTOR2_BACKWARD_PIN, HIGH);
}

void stop() {
  digitalWrite(MOTOR1_FORWARD_PIN, LOW);
  digitalWrite(MOTOR1_BACKWARD_PIN, LOW);

  digitalWrite(MOTOR2_FORWARD_PIN, LOW);
  digitalWrite(MOTOR2_BACKWARD_PIN, LOW);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // delay 10μs
  digitalWrite(TRIG_PIN, LOW);
  int microsecs = pulseIn(ECHO_PIN, HIGH);
  float detected_distance = microsecs*SPEED_OF_SOUND/2; // in cm
  Serial.println(detected_distance);
  if (!ball_launched && detected_distance < STOPPING_DISTANCE) { // stop motor
    stop();
    ball_launched = true;
    delay(1000);
  } else if (ball_launched) {
    backward();
  } else {
    forward();
  }
  delay(10); // delay 10ms
}
