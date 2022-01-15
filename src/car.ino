#include <AFMotor.h>
#include <Dabble.h>

static inline int8_t sgn(int val) {
  if (val < 0) return -1;
  if (val == 0) return 0;
  return 1;
}

#define MOTORS 4
#define MOTOR_UPPER_LEFT 1
#define MOTOR_UPPER_RIGHT 2
#define MOTOR_LOWER_LEFT 3
#define MOTOR_LOWER_RIGHT 4
#define PIN_BLUETOOTH_TX A0
#define PIN_BLUETOOTH_RX A1
#define AXIS_LIMIT 7
#define SPEED_MIN 85
#define SPEED_MAX 255
#define SPEED_LIMIT (SPEED_MAX - SPEED_MIN)
#define DIRECTION_LIMIT 100

AF_DCMotor motors[] = {
  AF_DCMotor(MOTOR_UPPER_LEFT, MOTOR12_64KHZ),
  AF_DCMotor(MOTOR_UPPER_RIGHT, MOTOR12_64KHZ),
  AF_DCMotor(MOTOR_LOWER_LEFT, MOTOR12_64KHZ),
  AF_DCMotor(MOTOR_LOWER_RIGHT, MOTOR12_64KHZ)
};
int directions[] = { -1, 1, -1, 1 };

void setup() {
  Dabble.begin(9600, PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX);
}

void loop() {
  Dabble.processInput();

  float yAxis = GamePad.getYaxisData();
  float xAxis = GamePad.getXaxisData();
  int speed = map(yAxis, -AXIS_LIMIT, AXIS_LIMIT, -SPEED_LIMIT, SPEED_LIMIT);
  int direction = map(xAxis, -AXIS_LIMIT, AXIS_LIMIT, -DIRECTION_LIMIT, DIRECTION_LIMIT);
  int rawSpeed = speed == 0 ? 0 : SPEED_MIN + abs(speed);

  for (int i = 0; i < MOTORS; i++) {
    int friction = sgn(direction) == directions[i] ? rawSpeed * abs(direction) / 100 : 0;
    motors[i].setSpeed(rawSpeed - friction);
    motors[i].run(speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE);
  }
}
