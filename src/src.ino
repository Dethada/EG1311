#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOTOR_GRP1_FWD_PIN 7
#define MOTOR_GRP1_BCK_PIN 6
#define MOTOR_GRP2_FWD_PIN 5
#define MOTOR_GRP2_BCK_PIN 4

const float SPEED_OF_SOUND = 0.0345; // in cm/μs
const float STOPPING_DISTANCE = 5;   // in cm

bool go_back = false;
bool moving = true;

void setup() {
  // Motor
  pinMode(MOTOR_GRP1_FWD_PIN, OUTPUT);
  pinMode(MOTOR_GRP1_BCK_PIN, OUTPUT);
  pinMode(MOTOR_GRP2_FWD_PIN, OUTPUT);
  pinMode(MOTOR_GRP2_BCK_PIN, OUTPUT);

  // Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode (ECHO_PIN, INPUT);

  Serial.begin(9600);
  moving = get_distance() >= STOPPING_DISTANCE;
}

void forward() {
  digitalWrite(MOTOR_GRP1_FWD_PIN, HIGH);
  digitalWrite(MOTOR_GRP1_BCK_PIN, LOW);

  digitalWrite(MOTOR_GRP2_FWD_PIN, HIGH);
  digitalWrite(MOTOR_GRP2_BCK_PIN, LOW);
}

void backward() {
  digitalWrite(MOTOR_GRP1_FWD_PIN, LOW);
  digitalWrite(MOTOR_GRP1_BCK_PIN, HIGH);

  digitalWrite(MOTOR_GRP2_FWD_PIN, LOW);
  digitalWrite(MOTOR_GRP2_BCK_PIN, HIGH);
}

void stop() {
  digitalWrite(MOTOR_GRP1_FWD_PIN, LOW);
  digitalWrite(MOTOR_GRP1_BCK_PIN, LOW);

  digitalWrite(MOTOR_GRP2_FWD_PIN, LOW);
  digitalWrite(MOTOR_GRP2_BCK_PIN, LOW);
}

float get_distance() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // delay 10μs
  digitalWrite(TRIG_PIN, LOW);
  int microsecs = pulseIn(ECHO_PIN, HIGH);
  float detected_distance = microsecs*SPEED_OF_SOUND/2; // in cm
  return detected_distance;
}

void loop() {
  float detected_distance = get_distance();
  Serial.println(detected_distance);
  if (detected_distance < STOPPING_DISTANCE) { // stop motor
    stop();
    if (moving) {
      go_back = !go_back;
      moving = false;
    }
    delay(1000);
  } else if (go_back) {
    moving = true;
    backward();
  } else {
    moving = true;
    forward();
  }
  delay(10); // delay 10ms
}
