#include <Arduino.h>
#include <MPU9250_WE.h>
#include <Wire.h>
#include <TaskScheduler.h>
#include <RotaryEncoder.h>

#include "BluetoothSerial.h"
#include "bluetooth_client.h"
#include "payload.h"
#include "motor.h"
#include "dss.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Payload performCalculations();
void calculateTaskCallback();

BluetoothSerial SerialBT;
BluetoothClient btClient(SerialBT);
MPU9250_WE mpu = MPU9250_WE();
RotaryEncoder *encoder = nullptr;
Motor motor;
Task calculateTask(3000, TASK_FOREVER, &calculateTaskCallback);
Scheduler scheduler;

IRAM_ATTR void checkPosition()
{
	encoder->tick();
}

IRAM_ATTR void resetPosition()
{
	encoder->setPosition(0);
}

float getBatteryLevel(uint16_t analogVal)
{
	int voltage;
	voltage = map(analogVal, 1790, 2559, 0, 100); // map to mV
	return voltage;
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Bluetooth Serial...");
	pinMode(13, OUTPUT);

	Wire.begin();
	mpu.init();
	Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
	delay(1000);
	mpu.autoOffsets();
	mpu.setSampleRateDivider(5);
	mpu.setAccRange(MPU9250_ACC_RANGE_2G);
	mpu.enableAccDLPF(true);
	mpu.setAccDLPF(MPU9250_DLPF_6);

	encoder = new RotaryEncoder(25, 27, RotaryEncoder::LatchMode::TWO03);
	attachInterrupt(digitalPinToInterrupt(25), checkPosition, CHANGE);
	attachInterrupt(digitalPinToInterrupt(27), checkPosition, CHANGE);
	attachInterrupt(digitalPinToInterrupt(26), resetPosition, RISING);

	motor.begin();
	motor.setConfig(true, 0); // cold, heater 0

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
		btClient.receive(Serial, motor);
		angles = mpu.getAngles();
		if (angles.x > 5 && angles.x < 15)
		{
			// DUDUK SAKIT, VIBRATOR ON
		}
		scheduler.execute();
	}
	Serial.println("Bluetooth device disconnected!");
	delay(1000);
}

void calculateTaskCallback()
{
	Payload payload = performCalculations();
	DSS dss(payload);
	String desc = dss.generateDescription();
	String status = dss.generateStatus();
	Payload finalPayload = payload.copyWith(-1, -1, -1, 0, desc, status, 0, 0);

	btClient.send(finalPayload);
}

Payload performCalculations()
{
	encoder->tick(); // just call tick() to check the state.
	float angleX = angles.x;
	float angleY = angles.y;
	float angleZ = angles.z;
	uint16_t flex = analogRead(33);
	int encoderPos = encoder->getPosition();
	float batteryLevel = getBatteryLevel(analogRead(34));
	Serial.print("analog: ");
	Serial.println(analogRead(34));

	return Payload(angleX, angleY, angleZ, flex, "", "", encoderPos, batteryLevel);
}

// #include <Arduino.h>

// void setup()
// {
// 	Serial.begin(115200);
// 	Serial.println("Hello, World!");
// 	pinMode(5, OUTPUT);
// 	pinMode(17, OUTPUT);
// 	pinMode(18, OUTPUT);
// 	pinMode(19, OUTPUT);
// 	digitalWrite(5, HIGH);
// 	digitalWrite(17, HIGH);
// }

// void loop()
// {
// 	for (int i = 0; i < 128; i++)
// 	{
// 		analogWrite(18, i);
// 		delayMicroseconds(100);
// 		analogWrite(19, 0);
// 		delay(50);
// 	}
// 	for (int i = 128; i > 0; i--)
// 	{
// 		analogWrite(18, i);
// 		delayMicroseconds(100);
// 		analogWrite(19, 0);
// 		delay(50);
// 	}
// 	// for (int i = 0; i < 128; i++)
// 	// {
// 	// 	analogWrite(18, 0);
// 	// 	analogWrite(19, i);
// 	// 	delay(50);
// 	// }
// 	// for (int i = 128; i > 0; i--)
// 	// {
// 	// 	analogWrite(18, 0);
// 	// 	analogWrite(19, i);
// 	// 	delay(50);
// 	// }
// }