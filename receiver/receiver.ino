/*
  BLE_Peripheral.ino
version 5.0
  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE
  as a peripheral device and specifies a service and a characteristic. Depending
  of the value of the specified characteristic, an on-board LED gets on.
test
  The circuit:
  - Arduino Nano 33 BLE.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

enum
{
  GESTURE_NONE = -1,
  GESTURE_UP = 0,
  GESTURE_DOWN = 1,
  GESTURE_LEFT = 2,
  GESTURE_RIGHT = 3
};

const char *deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid2 = "19b10001-e8f2-537e-4f6c-d104768a1215";

String gesture = "";

BLEService gestureService(deviceServiceUuid);
BLECharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite,"start- Characteristic <gesture_type> has changed!- Characteristic <gesture_type> has changed!Characteristic <gesture_type> has changed!Characteristic <gesture_type> has changed!");
//BLEByteCharacteristic gestureCharacteristic2(deviceServiceCharacteristicUuid2, BLERead | BLEWrite);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  if (!BLE.begin())
  {
    Serial.println("- Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");
  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  //gestureService.addCharacteristic(gestureCharacteristic2);
  BLE.addService(gestureService);
  //gestureCharacteristic.writeValue(-1);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop()
{
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central)
  {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected())
    {
      if (gestureCharacteristic.written())
      {
        gesture = reinterpret_cast<const char *>(gestureCharacteristic.value());
        Serial.print("data recived :");
        Serial.println(gesture);
        //Serial.println(gestureCharacteristic.value()); 
        writeGesture(gesture);
      }
    }

    Serial.println("* Disconnected to central device!");
  }
}

void writeGesture(String gesture)
{
  Serial.println("- Characteristic <gesture_type> has changed!");
  Serial.println(gesture);
}