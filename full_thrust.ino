#include <ESP32Servo.h>

// Thruster pin definitions
#define THRUSTER1_PIN 17
#define THRUSTER2_PIN 18
#define THRUSTER3_PIN 15
#define THRUSTER4_PIN 16
#define THRUSTER5_PIN 26
#define THRUSTER6_PIN 18
#define THRUSTER7_PIN 19
#define THRUSTER8_PIN 21

Servo thruster[8];

void setup() {
  Serial.begin(115200);

  int pins[8] = {THRUSTER1_PIN, THRUSTER2_PIN, THRUSTER3_PIN, THRUSTER4_PIN,
                 THRUSTER5_PIN, THRUSTER6_PIN, THRUSTER7_PIN, THRUSTER8_PIN};

  // Attach ESCs to pins
  for (int i = 0; i < 8; i++) {
    thruster[i].setPeriodHertz(50);
    thruster[i].attach(pins[i], 1000, 2000);
    thruster[i].writeMicroseconds(1500); // Neutral on startup
  }

  delay(3000); // Allow ESCs to arm
  Serial.println("Thrusters ready. Type 'on', 'rev', or 'off'.");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "w") {
      setThrusters(1700);
      Serial.println("Thrusters ON (Forward)");
    } 
    else if (cmd == "a") {
      setThrusters(1300);
      Serial.println("Thrusters ON (Reverse)");
    } 
    else if (cmd == "n") {
      setThrusters(1500);
      Serial.println("Thrusters OFF (Neutral)");
    }
  }
}

// Function to set all thrusters
void setThrusters(int pulse) {
  for (int i = 0; i < 8; i++) {
    thruster[i].writeMicroseconds(pulse);
  }
}
