#include <ESP32Servo.h>

// Thruster pins (safe PWM pins)
#define T1 13
#define T2 14
#define T3 15
#define T4 16
#define T5 17
#define T6 18
#define T7 19
#define T8 21

Servo thruster[8];
int neutral = 1500;
int pulseOffset = 0;

bool state[8];   // true = CW, false = Anti-CW
bool active[8];  // true = thruster running

void setup() {
  Serial.begin(115200);
  Serial.println("ROV Thruster Control Ready!");

  // Allocate 4 timers for 8 thrusters
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  int pins[8] = {T1,T2,T3,T4,T5,T6,T7,T8};
  for(int i=0;i<8;i++){
    thruster[i].setPeriodHertz(50); // standard ESC 50Hz
    thruster[i].attach(pins[i], 1000, 2000); // min 1000, max 2000 us
    thruster[i].writeMicroseconds(neutral);
    active[i] = false;
  }
}

void updateThrusters(){
  for(int i=0;i<8;i++){
    if(active[i]){
      thruster[i].writeMicroseconds(state[i] ? neutral + pulseOffset : neutral - pulseOffset);
    }else{
      thruster[i].writeMicroseconds(neutral);
    }
  }
  printThrusterStatus();
}

void setMovement(bool a[8], bool d[8]){
  for(int i=0;i<8;i++){
    active[i] = a[i];
    state[i] = d[i];
  }
  updateThrusters();
}

void allStop(){
  for(int i=0;i<8;i++) active[i]=false;
  updateThrusters();
  Serial.println("All thrusters stopped.");
}

void printThrusterStatus(){
  Serial.print("Thruster Status -> ");
  for(int i=0;i<8;i++){
    if(active[i]){
      Serial.print("T"); Serial.print(i+1);
      Serial.print(state[i] ? "(CW) " : "(CCW) ");
    }
  }
  Serial.print(" Speed: "); Serial.println(pulseOffset);
}

void loop(){
  if(Serial.available()){
    char key = Serial.read();
    if(key >= 'A' && key <= 'Z') key += 32;

    // Speed control
    if(key>='1' && key<='5'){
      switch(key){
        case '1': pulseOffset=0; break;
        case '2': pulseOffset=100; break;
        case '3': pulseOffset=200; break;
        case '4': pulseOffset=300; break;
        case '5': pulseOffset=400; break;
      }
      Serial.print("Speed set: "); Serial.println(pulseOffset);
      updateThrusters();
    }
    else if(key=='0'){ allStop(); }

    else{
      bool a[8]={false,false,false,false,false,false,false,false};
      bool d[8]={false,false,false,false,false,false,false,false};

      switch(key){
        case '6': // Forward
          a[0]=a[1]=a[2]=a[3]=true;
          d[0]=true; d[1]=false; d[2]=true; d[3]=false;
          Serial.println("Forward"); break;
        case '7': // Backward
          a[0]=a[1]=a[2]=a[3]=true;
          d[0]=false; d[1]=true; d[2]=false; d[3]=true;
          Serial.println("Backward"); break;
        case '8': // Up
          a[4]=a[5]=a[6]=a[7]=true;
          d[4]=true; d[5]=false; d[6]=false; d[7]=true;
          Serial.println("Up"); break;
        case '9': // Down
          a[4]=a[5]=a[6]=a[7]=true;
          d[4]=false; d[5]=true; d[6]=true; d[7]=false;
          Serial.println("Down"); break;
        case 'q': // Pitch Up
          a[4]=a[5]=a[6]=a[7]=true;
          d[4]=true; d[5]=false; d[6]=true; d[7]=false;
          Serial.println("Pitch Up"); break;
        case 'w': // Pitch Down
          a[4]=a[5]=a[6]=a[7]=true;
          d[4]=false; d[5]=true; d[6]=false; d[7]=true;
          Serial.println("Pitch Down"); break;
        case 'e': // Yaw Right
          a[0]=a[1]=a[2]=a[3]=true;
          for(int i=0;i<4;i++) d[i]=true;
          Serial.println("Yaw Right"); break;
        case 'r': // Yaw Left
          a[0]=a[1]=a[2]=a[3]=true;
          for(int i=0;i<4;i++) d[i]=false;
          Serial.println("Yaw Left"); break;
        case 'z': allStop(); break;
      }
      setMovement(a,d);
    }
  }
}
