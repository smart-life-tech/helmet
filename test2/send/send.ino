#include <ArduinoBLE.h>

const char *deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid1 = "19b10001-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid2 = "19b10002-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid3 = "19b10003-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid4 = "19b10004-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid5 = "19b10005-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid6 = "19b10006-e8f2-537e-4f6c-d104768a1214";

BLEService service(deviceServiceUuid);

BLEFloatCharacteristic analogValue1(deviceServiceCharacteristicUuid1, BLEWrite | BLEWriteWithoutResponse);
BLEFloatCharacteristic analogValue2(deviceServiceCharacteristicUuid2, BLEWrite | BLEWriteWithoutResponse);
BLEFloatCharacteristic analogValue3(deviceServiceCharacteristicUuid3, BLEWrite | BLEWriteWithoutResponse);
BLEFloatCharacteristic analogValue4(deviceServiceCharacteristicUuid4, BLEWrite | BLEWriteWithoutResponse);
BLEFloatCharacteristic analogValue5(deviceServiceCharacteristicUuid5, BLEWrite | BLEWriteWithoutResponse);
BLEFloatCharacteristic analogValue6(deviceServiceCharacteristicUuid6, BLEWrite | BLEWriteWithoutResponse);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(1); /* code */
    }

    Serial.println("Connected to central device!");
    // Initialize analog inputs
    analogReadResolution(12);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);

    // Initialize BLE
    if (!BLE.begin())
    {
        Serial.println("Failed to start BLE module!");
        while (1)
            ;
    }

    // Set service and characteristics
    service.addCharacteristic(analogValue1);
    service.addCharacteristic(analogValue2);
    service.addCharacteristic(analogValue3);
    service.addCharacteristic(analogValue4);
    service.addCharacteristic(analogValue5);
    service.addCharacteristic(analogValue6);
    BLE.addService(service);

    // Start advertising and wait for a connection
    BLE.advertise();
    while (!BLE.connected())
    {
        delay(100);
    }
}

void loop()
{
    // Update analog values
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    float value4 = analogRead(A3);
    float value5 = analogRead(A4);
    float value6 = analogRead(A5);

    // Write values to characteristics
    analogValue1.writeValue(value1);
    analogValue2.writeValue(value2);
    analogValue3.writeValue(value3);
    analogValue4.writeValue(value4);
    analogValue5.writeValue(value5);
    analogValue6.writeValue(value6);

    // Print sent values on the serial monitor
    Serial.print("Analog values sent: ");
    Serial.print(value1);
    Serial.print(", ");
    Serial.print(value2);
    Serial.print(", ");
    Serial.print(value3);
    Serial.print(", ");
    Serial.print(value4);
    Serial.print(", ");
    Serial.print(value5);
    Serial.print(", ");
    Serial.print(value6);
    Serial.print(", ");
    delay(1000);
}