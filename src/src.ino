#include <Servo.h>
// On boards other than the Mega, use of the library disables analogWrite() (PWM)
// functionality on pins 9 and 10, whether or not there is a Servo on those pins.

#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOTOR_GRP1_FWD_PIN 7
#define MOTOR_GRP1_BCK_PIN 6
#define MOTOR_GRP2_FWD_PIN 5
#define MOTOR_GRP2_BCK_PIN 4
#define SERVO_PIN 2

#define INTER_LOOP_DELAY 10 // ms
#define STOPPING_DELAY 250 // ms
#define SLINGSHOT_FIRING_DELAY 1000 // ms
#define SLINGSHOT_RESET_DELAY 1000 // ms
#define BUFFER_TIME_TO_LEAVE_THE_WALL 2000 // ms

const float SPEED_OF_SOUND = 0.0345; // in cm/μs
const float STOPPING_DISTANCE = 10;   // in cm
Servo servo;

// State variables
bool prev_forward = true;
bool prev_stopped = false;
bool curr_forward = true;
bool curr_stopped = false;

void store_state() {
    prev_forward = curr_forward;
    prev_stopped = curr_stopped;
}

bool was_moving_forward() {
    return prev_forward && !prev_stopped;
}

bool is_moving_forward() {
    return curr_forward && !curr_stopped;
}

bool is_moving_backward() {
    return !curr_forward && !curr_stopped;
}

bool is_moving() {
    return is_moving_forward() || is_moving_backward();
}

void forward() {
    digitalWrite(MOTOR_GRP1_FWD_PIN, HIGH);
    digitalWrite(MOTOR_GRP1_BCK_PIN, LOW);

    digitalWrite(MOTOR_GRP2_FWD_PIN, HIGH);
    digitalWrite(MOTOR_GRP2_BCK_PIN, LOW);
    store_state();
    curr_forward = true;
    curr_stopped = false;
}

void backward() {
    digitalWrite(MOTOR_GRP1_FWD_PIN, LOW);
    digitalWrite(MOTOR_GRP1_BCK_PIN, HIGH);

    digitalWrite(MOTOR_GRP2_FWD_PIN, LOW);
    digitalWrite(MOTOR_GRP2_BCK_PIN, HIGH);
    store_state();
    curr_forward = false;
    curr_stopped = false;
}

void stop() {
    digitalWrite(MOTOR_GRP1_FWD_PIN, LOW);
    digitalWrite(MOTOR_GRP1_BCK_PIN, LOW);

    digitalWrite(MOTOR_GRP2_FWD_PIN, LOW);
    digitalWrite(MOTOR_GRP2_BCK_PIN, LOW);
    store_state();
    curr_stopped = true;
}

float get_distance() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int microsecs = pulseIn(ECHO_PIN, HIGH);
    float detected_distance = microsecs * SPEED_OF_SOUND / 2; // in cm
    return detected_distance;
}

bool must_stop() {
    float detected_distance = get_distance();
    Serial.println(detected_distance);
    return detected_distance < STOPPING_DISTANCE;
}

void fire_slingshot() {
    Serial.println("Slingshot firing!");
    servo.write(180);
    delay(SLINGSHOT_FIRING_DELAY);
    Serial.println("Slingshot fired!");
}

void reset_slingshot() {
    Serial.println("Slingshot resetting...");
    servo.write(0);
    delay(SLINGSHOT_RESET_DELAY);
    Serial.println("Slingshot reset!");
}

void setup() {
    Serial.begin(9600);

    // Motor
    pinMode(MOTOR_GRP1_FWD_PIN, OUTPUT);
    pinMode(MOTOR_GRP1_BCK_PIN, OUTPUT);
    pinMode(MOTOR_GRP2_FWD_PIN, OUTPUT);
    pinMode(MOTOR_GRP2_BCK_PIN, OUTPUT);

    // Ultrasonic Sensor
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    pinMode (ECHO_PIN, INPUT);

    // Servo
    servo.attach(SERVO_PIN, 660, 2400);
    // Set the servo into the initial position to prepare for firing.
    reset_slingshot();

    // It is expected that there is no obstacle in front of the robot
    // at the start of the run, so we should start by moving forward.
    forward();
}

void loop() {
    // Under expected conditions this is true iff we reach the wall,
    // so stop and fire the slingshot, then reverse.
    if (must_stop() && was_moving_forward()) {
        stop();
        delay(STOPPING_DELAY);
        fire_slingshot();
        reset_slingshot();
        backward();
        delay(BUFFER_TIME_TO_LEAVE_THE_WALL);
    }

    if (!is_moving()) {
        // Sanity check. Execution should not reach here,
        // as the only time the robot should stop is when it first reaches
        // the wall and while firing the slingshot.
        Serial.println("Error: unexpected stopped state");
    }
    delay(INTER_LOOP_DELAY);
}
