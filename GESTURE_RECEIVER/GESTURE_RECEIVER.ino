#include <Arduino.h>
#include "BluetoothSerial.h"
#include <vector>

BluetoothSerial SerialBT;

#define UP 1
#define DOWN 2
#define TURN_LEFT 3
#define TURN_RIGHT 4
#define STOP 0

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;    
};

std::vector<MOTOR_PINS> motorPins = 
{
  {12, 13},  //LEFT_MOTOR
  {15, 14},  //RIGHT_MOTOR 
  // {27, 26},  //LEFT_MOTOR
  // {25, 33},  //RIGHT_MOTOR 
};

int xAxisValue;
int yAxisValue;
int zAxisValue;

// callback function that will be executed when data is received
void logic() 
{
  if ( yAxisValue < 75)
  {
    processCarMovement(FORWARD);  
  }
  else if ( zAxisValue > 175)
  {
    processCarMovement(TURN_RIGHT);
  }
  else if ( zAxisValue < 75)
  {
    processCarMovement(TURN_LEFT);
  }
  else if ( yAxisValue > 175)
  {
    processCarMovement(BACKWARD);     
  }
  else
  {
    processCarMovement(STOP);     
  }
}

void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);    
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);     
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
}

void processCarMovement(int inputValue)
{
  switch(inputValue)
  {
    case UP:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, FORWARD);
      Serial.println("UP");
      break;
    case DOWN:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);
      Serial.println("DOWN"); 
      break;

    case TURN_LEFT:
      rotateMotor(LEFT_MOTOR, FORWARD);
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      Serial.println("LEFT");  
      break;
    break;
    case TURN_RIGHT:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD); 
      Serial.println("RIGHT");    
      break;
    case STOP:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);    
      break;
  
    default:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);     
      break;
  }
}

void setUpPinModes()
{
  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);  
    rotateMotor(i, STOP);  
  }
}

void setup(void) 
{
  setUpPinModes();
  
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  Serial.println("Bluetooth initialized.");

}
 
void loop() 
{
  uint8_t recv_data[3];
  if (SerialBT.available()) {
    SerialBT.readBytes(recv_data, 3);

    xAxisValue = recv_data[0];
    yAxisValue = recv_data[1]; // Joystick X-axis value
    zAxisValue = recv_data[2]; // Joystick Y-axis value
    Serial.printf("xAxisValue: %d, yAxisValue: %d, zAxisValue: %d\n", recv_data[0], recv_data[1], recv_data[2]);
    void logic();
  }
  delay(20);
}
