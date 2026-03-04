//	デジタル入力・アナログ入力ライブラリー	ESP32,ArduinoFramework,C++
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#include <Arduino.h>
#include "DigitalInput.hpp"
#include "AnalogInput.hpp"

DigitalInput	devD;	//push_switch, toggle_switch, slide_switch, ...
AnalogInput		devA;	//potentio_meter, slid_volune, stick/lever, ...

static	constexpr	gpio_num_t
	GpioAnalog = GPIO_NUM_0,
	GpioDigital = GPIO_NUM_3;

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	devA.Initialize(GpioAnalog);
	devD.Initialize(GpioDigital);
}

void	loop(void)
{
	int16_t adcVal = devA.Read();	//ADC:0-4095
	int8_t pinVal = devD.Read();	//1:HIGH, 0:LOW
	Serial.printf("devA:%d devD:%d\n", adcVal, pinVal);
	delay(10);
}
