#include <ESP32Servo.h>

#define THRUSTER1_PIN 13
#define THRUSTER2_PIN 14
#define THRUSTER3_PIN 15
#define THRUSTER4_PIN 25
#define THRUSTER5_PIN 26
#define THRUSTER6_PIN 18
#define THRUSTER7_PIN 19
#define THRUSTER8_PIN 21

Servo thruster[8];


bool forward = false, backward = false;
bool swayLeft = false, swayRight = false;
bool heaveUp = false, heaveDown = false;
bool yawLeft = false, yawRight = false;
bool rollLeft = false, rollRight = false;
bool pitchForward = false, pitchBackward = false;

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
  // Check serial input for key presses
  while (Serial.available()) {
    char key = Serial.read();
    setMovementFlag(key, true);
  }

  // Apply movements based on flags
  updateThrusters();

  // Print active movement
  printMovement();

  delay(50); 
}


void setMovementFlag(char key, bool state) {
  switch (key) {
    case 'w': forward = state; break;
    case 's': backward = state; break;
    case 'a': swayLeft = state; break;
    case 'd': swayRight = state; break;
    case 'r': heaveUp = state; break;
    case 'f': heaveDown = state; break;
    case 'q': yawLeft = state; break;
    case 'e': yawRight = state; break;
    case 'z': rollLeft = state; break;
    case 'x': rollRight = state; break;
    case 'c': pitchForward = state; break;
    case 'v': pitchBackward = state; break;
    case 'n': // Stop all
      forward = backward = swayLeft = swayRight = heaveUp = heaveDown = false;
      yawLeft = yawRight = rollLeft = rollRight = pitchForward = pitchBackward = false;
      break;
  }
}

// Update thrusters based on current flags
void updateThrusters() {
  // Start with neutral
  for (int i = 0; i < 8; i++) thruster[i].writeMicroseconds(1500);

  // Horizontal thrusters 1-4
  int hThruster[4] = {1500, 1500, 1500, 1500};

  if (forward) for(int i=0;i<4;i++) hThruster[i] += 200;
  if (backward) for(int i=0;i<4;i++) hThruster[i] -= 200;
  if (swayLeft) { hThruster[0]-=200; hThruster[1]+=200; hThruster[2]+=200; hThruster[3]-=200; }
  if (swayRight){ hThruster[0]+=200; hThruster[1]-=200; hThruster[2]-=200; hThruster[3]+=200; }
  if (yawLeft)  { hThruster[0]-=200; hThruster[1]+=200; hThruster[2]-=200; hThruster[3]+=200; }
  if (yawRight) { hThruster[0]+=200; hThruster[1]-=200; hThruster[2]+=200; hThruster[3]-=200; }

  for(int i=0;i<4;i++) thruster[i].writeMicroseconds(hThruster[i]);

  // Vertical thrusters 5-8
  int vThruster[4] = {1500, 1500, 1500, 1500};

  if (heaveUp) for(int i=0;i<4;i++) vThruster[i]+=200;
  if (heaveDown) for(int i=0;i<4;i++) vThruster[i]-=200;
  if (rollLeft)  { vThruster[0]-=200; vThruster[1]+=200; vThruster[2]-=200; vThruster[3]+=200; }
  if (rollRight) { vThruster[0]+=200; vThruster[1]-=200; vThruster[2]+=200; vThruster[3]-=200; }
  if (pitchForward){ vThruster[0]+=200; vThruster[1]+=200; vThruster[2]-=200; vThruster[3]-=200; }
  if (pitchBackward){ vThruster[0]-=200; vThruster[1]-=200; vThruster[2]+=200; vThruster[3]+=200; }

  for(int i=0;i<4;i++) thruster[i+4].writeMicroseconds(vThruster[i]);
}

// Print active movements
void printMovement() {
  String movement = "";

  if (forward) movement += "Forward ";
  if (backward) movement += "Backward ";
  if (swayLeft) movement += "Sway Left ";
  if (swayRight) movement += "Sway Right ";
  if (heaveUp) movement += "Heave Up ";
  if (heaveDown) movement += "Heave Down ";
  if (yawLeft) movement += "Yaw Left ";
  if (yawRight) movement += "Yaw Right ";
  if (rollLeft) movement += "Roll Left ";
  if (rollRight) movement += "Roll Right ";
  if (pitchForward) movement += "Pitch Forward ";
  if (pitchBackward) movement += "Pitch Backward ";

  if (movement == "") movement = "Neutral";

  Serial.println(movement);
}
