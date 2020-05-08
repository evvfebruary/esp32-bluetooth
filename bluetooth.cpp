#include "BluetoothSerial.h"
#include <TroykaIMU.h>

String BluetoothName = "ESP32";
BluetoothSerial SerialBT;

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
      // блютуз крашится, после реконнекта
      // это костыль, чтобы рпешить проблему
      ESP.restart();
      break;
    case ESP_SPP_DATA_IND_EVT:
      Serial.println("Data received");
      Serial.printf("ESP_SPP_DATA_IND_EVT len=%d handle=%d", param->data_ind.len, param->data_ind.handle);
      break;
  }
  Serial.println(ESP_SPP_SRV_OPEN_EVT);
}
 
void setup() {
  Serial.begin(112500);
  SerialBT.begin(BluetoothName);
  Serial.println(BluetoothName);
  SerialBT.register_callback(callback);

  gyro.begin();
  accel.begin();
}

void loop() {
  if (SerialBT.hasClient()) {
    char* tmp = measurement();
    Serial.println(tmp);
    SerialBT.println(tmp);
  }
  delayMicroseconds(1);
}