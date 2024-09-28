#include <BluetoothSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

BluetoothSerial SerialBT;


String MACadd = "0C:B8:15:F6:53:52"; // Write Robot's MAC address
uint8_t address[6] = {0x0C, 0xB8, 0x15, 0xF6, 0x53, 0x52}; // 0x08, 0xD1, 0xF9, 0x35, 0x47, 0x0A

// MPU control/status vars
MPU6050 mpu;
bool dmpReady = false;
uint8_t devStatus; 
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

void setupMPU() {

  Wire.begin();// For ESP32
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    dmpReady = true;
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_MPU6050", true);
  Serial.println("The device started in master mode, make sure the remote BT device is on!");
  
  if (SerialBT.connect(address)) {
    Serial.println("Connected Successfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure the remote device is available and in range, then restart the app.");
    }
  }
  
  setupMPU();
}

void loop() {
  uint8_t send_data[3];

  if (!dmpReady) {
    Serial.println("MPU initialization failed. Check your connections and try again.");
    delay(1000); // Wait for a moment before retrying
    return;
  }

  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    int xAxisValue = constrain(ypr[2] * 180 / M_PI, -90, 90);
    int yAxisValue = constrain(ypr[1] * 180 / M_PI, -90, 90);
    int zAxisValue = constrain(ypr[0] * 180 / M_PI, -90, 90);

    int xAxis = map(xAxisValue, -90, 90, 0, 254);
    int yAxis = map(yAxisValue, -90, 90, 0, 254);
    int zAxis = map(zAxisValue, -90, 90, 0, 254);

    send_data[0] = xAxis;
    send_data[1] = yAxis;
    send_data[2] = zAxis;

    Serial.print("Data: ");
    for (int i = 0; i < 3; i++) {
      Serial.print(send_data[i]);
      Serial.print(" ");
    }
    Serial.println();

    SerialBT.write(send_data, 3);
    delay(50);
  }
}
