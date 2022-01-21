#include <QTRSensors.h>
#include <AFMotor.h>

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
// TODO: add ports to motors
AF_DCMotor jobb_elso();
AF_DCMotor bal_elso();
AF_DCMotor jobb_hatso();
AF_DCMotor bal_hatso();


void setup() {
  // put your setup code here, to run once:
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A15, A14, A13, A12, A11, A10, A9, A8}, SensorCount);
  qtr.setEmitterPin(52);
  
  Serial.begin(9600);

  set_speeds(200);
  release_motors();
}



int get_minmax_idx(int minmax){
  // minmax = 0 -> find idx of min value
  // minmax = 1 -> find idx of max value
  idx = 0;
  for (int i = 1; i < SensorCount; i++) {
    if (minmax == 0) {
      if (sensorValues[i] < sensorValues[idx]){
        idx = i;
      }
    }
    else{
      if (sensorValues[i] > sensorValues[idx]) {
        idx = i;
      }
    }
  }
}

void go_forward(){
  jobb_elso.run(FORWARD);
  jobb_hatso.run(FORWARD);
  bal_elso.run(FORWARD);
  bal_hatso.run(FORWARD);
}

void turn_left() {
  jobb_elso.run(FORWARD);
  jobb_hatso.run(FORWARD);
  bal_elso.run(BACKWARD);
  bal_hatso.run(BACKWARD);
}

void turn_right() {
  jobb_elso.run(BACKWARD);
  jobb_hatso.run(BACKWARD);
  bal_elso.run(FORWARD);
  bal_hatso.run(FORWARD);
}

void set_speeds(int speed_val){
  jobb_elso.setSpeed(speed_val);
  jobb_hatso.setSpeed(speed_val);
  bal_elso.setSpeed(speed_val);
  bal_hatso.setSpeed(speed_val);
}

void release_motors(){
  jobb_elso.run(RELEASE);
  jobb_hatso.run(RELEASE);
  bal_elso.run(RELEASE);
  bal_hatso.run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:
  qtr.read(sensorValues);

  int max_idx = get_minmax_idx(1);

  if(max_idx < 3) {
    // if the line is towards the left of the sensor
    set_speeds(10);
    turn_left();
    delay(10);
  }
  else{
    if(max_idx > 4) {
      // if the line is towards the right of the sensor
      set_speeds(10);
      turn_right();
      delay(10);
    }
    else{
      set_speeds(100);
      go_forward();
      delay(100);
    }
  }
  
}
