#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "BluetoothSerial.h"
#include <TroykaIMU.h>

String SIGNAL_START_WRITE = String("SIGNAL_START_TRAINING");
String SIGNAL_STOP_WRITE = String("SIGNAL_STOP_TRAINING");

bool writeMode = false;

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

void handleReceivedMessage(String message) {
  if (message == SIGNAL_START_WRITE) {
    writeMode = true;
  } else if (message == SIGNAL_STOP_WRITE) {
    writeMode = false;
  }
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  // Callback function implementation
  
  switch(event) {
    case ESP_SPP_SRV_OPEN_EVT:
      Serial.println("Client Connected");
      break;
    case ESP_SPP_CLOSE_EVT:
      Serial.println("Client diconnected");
      ESP.restart();
      break;
    case ESP_SPP_DATA_IND_EVT:
      Serial.println("Data received");
      String data = (char*) param->data_ind.data;
      data = data.substring(0, param->data_ind.len);
      handleReceivedMessage(data);
      Serial.println(data);
      Serial.println(param->data_ind.len);
      break;
  }
  Serial.println(ESP_SPP_SRV_OPEN_EVT);
}

void printDeviceAddress() {
  Serial.print("ESP32 BLUETOOTH MAC ADDRESS: ");
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
    if (i < 5){
      Serial.print(":");
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(112500);
  SerialBT.begin(BluetoothName);
  Serial.println(BluetoothName);
  SerialBT.register_callback(callback);
  printDeviceAddress();

  gyro.begin();
  accel.begin();
}

void loop() {
  if (SerialBT.hasClient() && writeMode) {
    char* tmp = measurement();
    Serial.println(tmp);
    SerialBT.println(tmp);
  }
  delayMicroseconds(1);
}