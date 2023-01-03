// Arduino_LSM9DS1 - Version: Latest
#include <Arduino_LSM9DS1.h>

// #include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

// Set up the FSRs
const int frontFSR = A0;
const int backFSR = A1;
const int leftFSR = A2;
const int rightFSR = A3;
const int centerFSR = A4;
float x, y, z;
// Set up the accelerometer
// Arduino_LSM9DS1 lsm;
String gesture = "";
String oldGestureValue = "";
// Set up the Bluetooth module
// BLEPeripheral blePeripheral;
//BLEDevice blePeripheral;
// Set up the service and characteristics for the Bluetooth module
//BLEService service("FFF0");
//BLECharCharacteristic txCharacteristic("FFF1", BLERead | BLEWrite);
// BLECharacteristic gestureCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
const char *deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

void setup()
{
  // Initialize the FSRs
  pinMode(frontFSR, INPUT);
  pinMode(backFSR, INPUT);
  pinMode(leftFSR, INPUT);
  pinMode(rightFSR, INPUT);
  pinMode(centerFSR, INPUT);

  // Initialize the accelerometer
  IMU.begin();
  if (!BLE.begin())
  {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }


  BLE.setLocalName("Helmet Impact Sensor");
  BLE.advertise();
  // Add the service and characteristics
  // blePeripheral.addAttribute(service);
  //blePeripheral.addAttribute(txCharacteristic);


}

void loop()
{
  connectToPeripheral();
  // Send the message over Bluetooth
  // txCharacteristic.writeValue(message);
  // blePeripheral.poll();
}
void connectToPeripheral()
{
  BLEDevice peripheral;

  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);

  if (peripheral)
  {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral)
{
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect())
  {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  }
  else
  {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes())
  {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  }
  else
  {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic gestureCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);

  if (!gestureCharacteristic)
  {
    Serial.println("* Peripheral device does not have gesture_type characteristic!");
    peripheral.disconnect();
    return;
  }
  else if (!gestureCharacteristic.canWrite())
  {
    Serial.println("* Peripheral does not have a writable gesture_type characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected())
  {
    gesture = gestureDetectection();

    if (oldGestureValue != gesture)
    {
      oldGestureValue = gesture;
      Serial.print("* Writing value to gesture_type characteristic: ");
      Serial.println(gesture);
      // Convert the String to a byte array
      int len = gesture.length() + 1;
      char charBuf[len];
      gesture.toCharArray(charBuf, len);
      byte byteBuf[len];
      for (int i = 0; i < len; i++) {
        byteBuf[i] = charBuf[i];
      }
      gestureCharacteristic.writeValue(byteBuf, len);
      Serial.println("* Writing value to gesture_type characteristic done!");
      Serial.println(" ");
    }
  }
  Serial.println("- Peripheral device disconnected!");
}

String gestureDetectection()
{
  // Read the values from the FSRs
  int frontForce = analogRead(frontFSR);
  int backForce = analogRead(backFSR);
  int leftForce = analogRead(leftFSR);
  int rightForce = analogRead(rightFSR);
  int centerForce = analogRead(centerFSR);

  // Read the acceleration data from the accelerometer
  //sensors_event_t accelEvent;
  //IMU.getEvent(&accelEvent, LSM9DS1_ACCELEROMETER);


  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }
  // Calculate the force impact speed
  float impactSpeed = sqrt(x * x + y * y + z * z);

  // Build the message to send over Bluetooth
  String message = "Front force: " + String(frontForce) + "\n" +
                   "Back force: " + String(backForce) + "\n" +
                   "Left force: " + String(leftForce) + "\n" +
                   "Right force: " + String(rightForce) + "\n" +
                   "Center force: " + String(centerForce) + "\n" +
                   "Impact speed: " + String(impactSpeed) + "\n";
  return message;
}