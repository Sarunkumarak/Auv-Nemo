#include <ESP32Servo.h>

// Thruster pins
#define T1 13
#define T2 18
#define T3 15
#define T4 16
#define T5 17
#define T6 14
#define T7 19
#define T8 21

Servo thruster[8];
int neutral = 1500;        // PWM for stopped thruster
int pulseOffset = 100;     // Initial speed offset

// Movement flags (persistent during normal operation)
bool move_forward = false;
bool move_backward = false;
bool move_up = false;
bool move_down = false;
bool pitch_up = false;
bool pitch_down = false;
bool yaw_right = false;
bool yaw_left = false;

bool armed = true; // Track if ESCs are armed

void setup() {
  Serial.begin(115200);

  int pins[8] = {T1, T2, T3, T4, T5, T6, T7, T8};
  for (int i = 0; i < 8; i++) {
    thruster[i].attach(pins[i]);
    thruster[i].writeMicroseconds(neutral);
  }

  delay(3000); // ESC arming
  Serial.println("ESCs armed. Ready to control the ROV!");
  Serial.println("Movement keys: 6-Forward, 7-Backward, 8-Up, 9-Down");
  Serial.println("Pitch: q-Up, w-Down | Yaw: e-Right, r-Left");
  Serial.println("Stop all: z | Neutral: n | Disarm: d | Re-arm: a | Emergency Stop: 0");
  Serial.println("Speed: 1-Very Low, 2-Low, 3-Medium, 4-High, 5-Very High");
  Serial.println("Type multiple keys together, e.g., 8q (Up + Pitch Up)");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    for (int i = 0; i < input.length(); i++) {
      char key = input[i];

      // Speed control (does not reset movements)
      if (key >= '1' && key <= '5') {
        pulseOffset = (key - '0') * 50; // 1->50, 5->250
        Serial.print("Speed set to: ");
        Serial.println(pulseOffset);
      }

      // Only process movements if ESCs are armed
      if (armed) {
        if (key == '6') move_forward = true;
        if (key == '7') move_backward = true;
        if (key == '8') move_up = true;
        if (key == '9') move_down = true;
        if (key == 'q') pitch_up = true;
        if (key == 'w') pitch_down = true;
        if (key == 'e') yaw_right = true;
        if (key == 'r') yaw_left = true;
      }

      // Stop all movements
      if (key == 'z' && armed) {
        resetMovements();
        Serial.println("All movements stopped (z pressed)");
      }

      // Neutral PWM
      if (key == 'n' && armed) {
        for (int j = 0; j < 8; j++) thruster[j].writeMicroseconds(neutral);
        Serial.println("Thrusters set to neutral (n pressed)");
      }

      // Disarm ESCs
      if (key == 'd' && armed) {
        resetMovements();
        for (int j = 0; j < 8; j++) thruster[j].detach();
        armed = false;
        Serial.println("ESCs disarmed and movements reset (d pressed)");
      }

      // = Re-arm ESCs (fixed)
      if (key == 'a' && !armed) {
        resetMovements();  // full movement flag reset
        int pins[8] = {T1, T2, T3, T4, T5, T6, T7, T8};
        for (int j = 0; j < 8; j++) {
          thruster[j].attach(pins[j]);
          thruster[j].writeMicroseconds(neutral); // Neutral after re-arm
        }
        delay(3000);
        armed = true;
        Serial.println("ESCs re-armed! All thrusters neutral and ready (a pressed)");
      }

      // Emergency stop
      if (key == '0' && armed) {
        resetMovements();
        for (int j = 0; j < 8; j++) thruster[j].writeMicroseconds(neutral);
        Serial.println("Emergency stop! Thrusters neutral (0 pressed)");
      }
    }

    // Update thrusters after processing all keys
    if (armed) updateThrusters();
  }
}

// --- Reset all movement flags ---
void resetMovements() {
  move_forward = move_backward = move_up = move_down = false;
  pitch_up = pitch_down = yaw_right = yaw_left = false;
}

// --- Update thrusters based on active movements ---
void updateThrusters() {
  int h_speed[4] = {neutral, neutral, neutral, neutral}; // Horizontal: T1-T4
  int v_speed[4] = {neutral, neutral, neutral, neutral}; // Vertical: T5-T8

  // Horizontal thrusters
  if (move_forward) {
    h_speed[0] += pulseOffset; h_speed[1] -= pulseOffset;
    h_speed[2] += pulseOffset; h_speed[3] -= pulseOffset;
  }
  if (move_backward) {
    h_speed[0] -= pulseOffset; h_speed[1] += pulseOffset;
    h_speed[2] -= pulseOffset; h_speed[3] += pulseOffset;
  }
  if (yaw_right) {
    h_speed[0] += pulseOffset; h_speed[1] -= pulseOffset;
    h_speed[2] += pulseOffset; h_speed[3] -= pulseOffset;
  }
  if (yaw_left) {
    h_speed[0] -= pulseOffset; h_speed[1] += pulseOffset;
    h_speed[2] -= pulseOffset; h_speed[3] += pulseOffset;
  }
  for (int i = 0; i < 4; i++) thruster[i].writeMicroseconds(h_speed[i]);

  // Vertical thrusters
  if (move_up) {
    for (int i = 0; i < 4; i++) v_speed[i] += pulseOffset;
  }
  if (move_down) {
    for (int i = 0; i < 4; i++) v_speed[i] -= pulseOffset;
  }
  if (pitch_up) {
    v_speed[0] += pulseOffset; v_speed[1] -= pulseOffset;
    v_speed[2] -= pulseOffset; v_speed[3] += pulseOffset;
  }
  if (pitch_down) {
    v_speed[0] -= pulseOffset; v_speed[1] += pulseOffset;
    v_speed[2] += pulseOffset; v_speed[3] -= pulseOffset;
  }
  for (int i = 0; i < 4; i++) thruster[i + 4].writeMicroseconds(v_speed[i]);
}
