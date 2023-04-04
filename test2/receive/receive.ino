#include <ArduinoBLE.h>

const char *deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char *deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

BLEService service(deviceServiceUuid);

BLEFloatCharacteristic analogValue1(deviceServiceCharacteristicUuid, BLERead | BLENotify);
BLEFloatCharacteristic analogValue2(deviceServiceCharacteristicUuid, BLERead | BLENotify);
BLEFloatCharacteristic analogValue3(deviceServiceCharacteristicUuid, BLERead | BLENotify);
BLEFloatCharacteristic analogValue4(deviceServiceCharacteristicUuid, BLERead | BLENotify);
BLEFloatCharacteristic analogValue5(deviceServiceCharacteristicUuid, BLERead | BLENotify);
BLEFloatCharacteristic analogValue6(deviceServiceCharacteristicUuid, BLERead | BLENotify);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(1);
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
    // Update analog values and notify subscribers
   /*analogValue1.writeValue(analogRead(A0));
    analogValue2.writeValue(analogRead(A1));
    analogValue3.writeValue(analogRead(A2));
    analogValue4.writeValue(analogRead(A3));
    analogValue5.writeValue(analogRead(A4));
    analogValue6.writeValue(analogRead(A5));*/
    delay(1000);

    // Print received values on the serial monitor
    if (analogValue1.written()||analogValue2.written()||analogValue3.written()||analogValue4.written()||analogValue5.written()||analogValue6.written())
    {
        float value1 = analogValue1.value();
        float value2 = analogValue2.value();
        float value3 = analogValue3.value();
        float value4 = analogValue4.value();
        float value5 = analogValue5.value();
        float value6 = analogValue6.value();

        Serial.print("Analog values: ");
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
        Serial.println(value6);
    }
}
