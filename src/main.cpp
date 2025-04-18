#include <Arduino.h>
#include <MPU9250_WE.h>
#include <Wire.h>
#include <TaskScheduler.h>

#include "BluetoothSerial.h"
#include "bluetooth_client.h"
#include "payload.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Payload performCalculations();
void calculateTaskCallback();

BluetoothSerial SerialBT;
BluetoothClient btClient(SerialBT);
MPU9250_WE mpu = MPU9250_WE();

Task calculateTask(1000, TASK_FOREVER, &calculateTaskCallback);
Scheduler scheduler;

void calculateTaskCallback()
{
	Payload payload = performCalculations();
	btClient.send(payload);
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Bluetooth Serial...");

	Wire.begin();
	mpu.init();
	Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
	delay(1000);
	mpu.autoOffsets();
	mpu.setSampleRateDivider(5);
	mpu.setAccRange(MPU9250_ACC_RANGE_2G);
	mpu.enableAccDLPF(true);
	mpu.setAccDLPF(MPU9250_DLPF_6);

	scheduler.init();
	scheduler.addTask(calculateTask);
	calculateTask.enable();

	while (!btClient.begin("ESP32test")) // Bluetooth device name
	{
		Serial.println("Bluetooth device failed!");
		delay(1000);
	}
	while (!btClient.isConnected())
	{
		Serial.println("Bluetooth device not connected!");
		delay(1000);
	}
}

xyzFloat angles;

void loop()
{
	while (btClient.isConnected())
	{
		btClient.receive(Serial);
		angles = mpu.getAngles();
		scheduler.execute();
	}
	Serial.println("Bluetooth device disconnected!");
	delay(1000);
}

Payload performCalculations()
{
	float angleX = angles.x;
	;
	float angleY = angles.y;
	float angleZ = angles.z;
	uint16_t flex = analogRead(33);
	String description = "Hail Kuru-Kuru";
	String status = "safe";
	Serial.println("Angle X: " + String(angleX) + ", Angle Y: " + String(angleY) + ", Angle Z: " + String(angleZ) + ", Flex: " + String(flex) + ", Description: " + description + ", Status: " + status);
	return Payload(angleX, angleY, angleZ, flex, description, status);
}