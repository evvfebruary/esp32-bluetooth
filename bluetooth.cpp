#include "BluetoothSerial.h"
#include <TroykaIMU.h>

BluetoothSerial SerialBT;
String MACadd = "AA:BB:CC:11:22:33";

Gyroscope gyro;
Accelerometer accel;
char buffer[256];
char* measurement(){
    sprintf(buffer,
    "%f;%f;%f;%f;%f;%f;%d;",
    accel.readAX(),
    accel.readAY(),
    accel.readAZ(),
    gyro.readDegPerSecX(),
    gyro.readDegPerSecY(),
    gyro.readDegPerSecZ(),
    millis());

    return buffer;
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  // Callback function implementation
  
  switch(event) {
    case ESP_SPP_SRV_OPEN_EVT:
      Serial.println("Client Connected");
      break;
    case ESP_SPP_CLOSE_EVT:
      Serial.println("Client diconnected");
      break;
  }
}
 
void setup() {
  Serial.begin(112500);
  SerialBT.begin("ESP32");
  Serial.println("ESP32");
  SerialBT.register_callback(callback);

  gyro.begin();
  accel.begin();
}

void loop() {
  char* tmp = measurement();
  Serial.println(tmp);
  
  SerialBT.println(tmp);

  delayMicroseconds(1);
}