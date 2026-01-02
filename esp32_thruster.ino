#include <ESP32Servo.h>

#define THRUSTER1_PIN 13
#define THRUSTER2_PIN 14
#define THRUSTER3_PIN 15
#define THRUSTER4_PIN 16
#define THRUSTER5_PIN 17
#define THRUSTER6_PIN 18
#define THRUSTER7_PIN 19
#define THRUSTER8_PIN 21

Servo thruster[8];

void setup() {
  Serial.begin(115200);

  int pins[8] = {THRUSTER1_PIN, THRUSTER2_PIN, THRUSTER3_PIN, THRUSTER4_PIN,
                  THRUSTER5_PIN, THRUSTER6_PIN, THRUSTER7_PIN, THRUSTER8_PIN};

  for (int i = 0; i < 8; i++) {
    thruster[i].setPeriodHertz(50);
    thruster[i].attach(pins[i], 1000, 2000);
    thruster[i].writeMicroseconds(1500); // Neutral
  }
  delay(3000); // ESC arming
}

void loop() {
  if (Serial.available()) {
    char key = Serial.read();
    moveROV(key);
  }
}

void moveROV(char key) {
  // Start with neutral
  for (int i = 0; i < 8; i++) thruster[i].writeMicroseconds(1500);

  switch (key) {
    case 'w': // Surge Forward
      for (int i = 0; i < 4; i++) thruster[i].writeMicroseconds(1700);
      break;
    case 's': // Surge Backward
      for (int i = 0; i < 4; i++) thruster[i].writeMicroseconds(1300);
      break;
    case 'a': // Sway Left
      thruster[0].writeMicroseconds(1300);
      thruster[1].writeMicroseconds(1700);
      thruster[2].writeMicroseconds(1700);
      thruster[3].writeMicroseconds(1300);
      break;
    case 'd': // Sway Right
      thruster[0].writeMicroseconds(1700);
      thruster[1].writeMicroseconds(1300);
      thruster[2].writeMicroseconds(1300);
      thruster[3].writeMicroseconds(1700);
      break;
    case 'r': // Heave Up
      for (int i = 4; i < 8; i++) thruster[i].writeMicroseconds(1700);
      break;
    case 'f': // Heave Down
      for (int i = 4; i < 8; i++) thruster[i].writeMicroseconds(1300);
      break;
    case 'q': // Yaw Left
      thruster[0].writeMicroseconds(1300);
      thruster[1].writeMicroseconds(1700);
      thruster[2].writeMicroseconds(1300);
      thruster[3].writeMicroseconds(1700);
      break;
    case 'e': // Yaw Right
      thruster[0].writeMicroseconds(1700);
      thruster[1].writeMicroseconds(1300);
      thruster[2].writeMicroseconds(1700);
      thruster[3].writeMicroseconds(1300);
      break;
    case 'z': // Roll Left
      thruster[4].writeMicroseconds(1300);
      thruster[5].writeMicroseconds(1700);
      thruster[6].writeMicroseconds(1300);
      thruster[7].writeMicroseconds(1700);
      break;
    case 'x': // Roll Right
      thruster[4].writeMicroseconds(1700);
      thruster[5].writeMicroseconds(1300);
      thruster[6].writeMicroseconds(1700);
      thruster[7].writeMicroseconds(1300);
      break;
    case 'c': // Pitch Forward
      thruster[4].writeMicroseconds(1700);
      thruster[5].writeMicroseconds(1700);
      thruster[6].writeMicroseconds(1300);
      thruster[7].writeMicroseconds(1300);
      break;
    case 'v': // Pitch Backward
      thruster[4].writeMicroseconds(1300);
      thruster[5].writeMicroseconds(1300);
      thruster[6].writeMicroseconds(1700);
      thruster[7].writeMicroseconds(1700);
      break;
  }
}
