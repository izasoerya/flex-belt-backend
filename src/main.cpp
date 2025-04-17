#include <Arduino.h>
#include <mpu6050_FastAngles.h>
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
mpu6050_FastAngles mpu;

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

void loop()
{
	while (btClient.isConnected())
	{
		btClient.receive(Serial);
		scheduler.execute();
	}
	Serial.println("Bluetooth device disconnected!");
	delay(1000);
}

Payload performCalculations()
{
	float angleX = mpu.getAngle('X');
	float angleY = mpu.getAngle('Y');
	float angleZ = mpu.getAngle('Z');
	uint16_t flex = analogRead(33);
	String description = "Hail Kuru-Kuru";
	String status = "safe";

	return Payload(angleX, angleY, angleZ, flex, description, status);
}