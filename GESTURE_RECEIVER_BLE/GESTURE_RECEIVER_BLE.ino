#include <Arduino.h>
#include "BluetoothSerial.h"
#include <vector>

BluetoothSerial SerialBT;

#define FR_IN1    4 //FRONT_RIGHT_MOTOR
#define FR_IN2    5 //FRONT_RIGHT_MOTOR
#define BR_IN1    18 //BACK_RIGHT_MOTOR
#define BR_IN2    19 //BACK_RIGHT_MOTOR
#define FL_IN1    14 //FRONT_LEFT_MOTOR
#define FL_IN2    27 //FRONT_LEFT_MOTOR
#define BL_IN1    26 //BACK_LEFT_MOTOR
#define BL_IN2    25 //BACK_LEFT_MOTOR

int xAxisValue;
int yAxisValue;
int zAxisValue;

// callback function that will be executed when data is received
void logic() 
{
  if (  xAxisValue < 100 &&  yAxisValue < 100)
  {
    //FORWARD_LEFT
    digitalWrite(FR_IN1, HIGH);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, HIGH);
    digitalWrite(BL_IN2, LOW);    
  }
  else if (  xAxisValue > 165 &&  yAxisValue > 90)
  {
    //FORWARD_RIGHT
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, HIGH);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, HIGH); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, LOW);     
  } 
  else if (  xAxisValue < 100 &&  yAxisValue > 150)
  {
    //BACKWARD_LEFT
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, HIGH);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, HIGH);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, LOW);    
  }
  else if (  xAxisValue > 150 &&  yAxisValue < 175)
  {
    //BACKWARD_RIGHT
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, HIGH);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, HIGH);    
  }  
 /* else if ( zAxisValue < 75)
  {
    //TURN_RIGHT
    digitalWrite(FR_IN1, HIGH);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, HIGH);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, HIGH);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, HIGH);
  }
  else if ( zAxisValue > 175)
  {
    //TURN_LEFT
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, HIGH);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, HIGH);
    digitalWrite(FL_IN1, HIGH); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, HIGH);
    digitalWrite(BL_IN2, LOW);;
  }
  */
  else if ( yAxisValue < 75)
  {
    //FORWARD
    digitalWrite(FR_IN1, HIGH);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, HIGH);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, HIGH); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, HIGH);
    digitalWrite(BL_IN2, LOW);  
  }
  else if ( yAxisValue > 175)
  {
    //BACKWARD
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, HIGH);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, HIGH);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, HIGH);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, HIGH);    
  }
  else if ( xAxisValue > 145)
  {
    //RIGHT
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, HIGH);
    digitalWrite(BR_IN1, HIGH);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, HIGH); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, HIGH);   
  }
  else if ( xAxisValue < 90)
  {
    //LEFT
    digitalWrite(FR_IN1, HIGH);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, HIGH);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, HIGH);
    digitalWrite(BL_IN1, HIGH);
    digitalWrite(BL_IN2, LOW);    
  } 
  else
  {
    //STOP
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(BR_IN1, LOW);
    digitalWrite(BR_IN2, LOW);
    digitalWrite(FL_IN1, LOW); 
    digitalWrite(FL_IN2, LOW);
    digitalWrite(BL_IN1, LOW);
    digitalWrite(BL_IN2, LOW);     
  }
   
}

void setUpPinModes()
{
  pinMode(FR_IN1, OUTPUT);
  pinMode(FR_IN2, OUTPUT);
  pinMode(BR_IN1, OUTPUT);
  pinMode(BR_IN2, OUTPUT);
  pinMode(FL_IN1, OUTPUT); 
  pinMode(FL_IN2, OUTPUT);
  pinMode(BL_IN1, OUTPUT);
  pinMode(BL_IN2, OUTPUT);
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
    logic();
  }
}
