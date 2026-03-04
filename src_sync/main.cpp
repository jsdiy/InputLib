//	デジタル入力・アナログ入力ライブラリー	ESP32,ArduinoFramework,C++
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#include <Arduino.h>
#include "HwSwitch.hpp"
#include "PotMeter.hpp"

HwSwitch	devD;	//push_switch, toggle_switch, slide_switch, ...
PotMeter	devA;	//potentio_meter, slid_volune, stick/lever, ...

static	constexpr	gpio_num_t
	GpioAnalog = GPIO_NUM_0,
	GpioDigital = GPIO_NUM_3;

//状態
AdcThrState	devAState;
SwState	devDState;
static	std::string	ToString(AdcThrState state);
static	std::string	ToString(SwState state);
static	void	Print(AdcThrState devAState, SwState devDState);

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	//アナログ入力
	//ここではADCの変換値を100-3500程度と想定
	//2800を上回ったとき、または、800を下回ったときを検出する
	devA.Initialize(GpioAnalog);
	devA.SetRisingThreshold(2800);
	devA.SetFallingThreshold(800);
	devAState = devA.GetState();

	//デジタル入力
	//押しボタンスイッチが接続されていると想定
	//ボタン押下状態や長押しを検出する
	devD.Initialize(GpioDigital);
	devD.SetLongHoldThresholdTime(3000);	//3秒押したら長押しとする
	devDState = devD.GetState();

	Print(devAState, devDState);
}

void	loop(void)
{
	//UpdateState()はループごとに呼び出す必要がある
	//GetState()で現在の状態を取得する
	//特別な状態はGetState()なしに判定することができる

	devA.UpdateState();
	if (devA.IsRising()) { Serial.println("高域突入"); }
	if (devA.IsFalling()) { Serial.println("低域突入"); }
	auto aState = devA.GetState();

	devD.UpdateState();
	if (devD.Clicked()) { Serial.println("クリック"); }
	if (devD.Holding()) { Serial.print("長押し中 "); }
	auto dState = devD.GetState();

	if ((devAState != aState) || (devDState != dState))
	{
		devAState = aState;
		devDState = dState;
		Print(devAState, devDState);
	}

	delay(10);
}

//状態表示
static	void	Print(AdcThrState devAState, SwState devDState)
{
	Serial.printf("devA:%s devD:%s\n", ToString(devAState).c_str(), ToString(devDState).c_str());
}

static	std::string	ToString(AdcThrState state)
{
	switch (state)
	{
	case	AdcThrState::Above:		return "Above";
	case	AdcThrState::Bellow:	return "Bellow";
	case	AdcThrState::Falling:	return "Falling";
	case	AdcThrState::None:		return "None";
	case	AdcThrState::Rising:	return "Rising";
	default:	return "";
	}
}

static	std::string	ToString(SwState state)
{
	switch (state)
	{
	case	SwState::LongHold:	return "LongHold";
	case	SwState::Off:		return "Off";
	case	SwState::On:		return "On";
	case	SwState::Release:	return "Release";
	case	SwState::ShortHold:	return "ShortHold";
	default:	return "";
	}
}
