//	デジタル入力・アナログ入力ライブラリー	ESP32,ArduinoFramework,C++
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#include <Arduino.h>
#include "HwSwitchA.hpp"	//非同期版はxxxA
#include "PotMeterA.hpp"	//非同期版はxxxA
#include "MyApplication.hpp"

//非同期版はxxxA
HwSwitchA	devD;	//push_switch, toggle_switch, slide_switch, ...
PotMeterA	devA;	//potentio_meter, slid_volune, stick/lever, ...
MyApplication	app;

static	constexpr	gpio_num_t
	GpioAnalog = GPIO_NUM_0,
	GpioDigital = GPIO_NUM_3;

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	devA.Initialize(GpioAnalog);
	devD.Initialize(GpioDigital);
	app.Setup(devA, devD);
}

void	loop(void)
{
	//非同期版はUpdateState()を呼び出す必要はない
	//ここでは省略するが、同期版と同様、GetState()で現在の状態を取得したり、
	//GetState()なしに特別な状態を判定したりすることはできる

	Serial.println("時間がかかる処理を開始... ※処理を抜けるにはイベントを発生させてください");
	app.FlagReset();
	app.HeavyFunction();	//ここで処理がブロックされる
	Serial.println("...完了");

	if (app.flagOnClicked) { Serial.printf("devD: Clickedイベント発生\n"); }
	if (app.flagOnChange) { Serial.printf("devD: Change-%sイベント発生\n", app.GetArg() == RISING ? "RISING" : "FALLING"); }
	if (app.flagOnRising) { Serial.printf("devA: Risingイベント発生 ADC=%d\n", app.GetArg()); }
	if (app.flagOnFalling) { Serial.printf("devA: Fallingイベント発生 ADC=%d\n", app.GetArg()); }

	delay(10);
}
