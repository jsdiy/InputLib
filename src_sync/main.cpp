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
PotMeter::State	devAState;
HwSwitch::State	devDState;
static	std::string	ToString(PotMeter::State state);
static	std::string	ToString(HwSwitch::State state);
static	void	Print(PotMeter::State devAState, HwSwitch::State devDState);

//表示動作のフラグ
static	uint8_t	range = 0;
static	bool	swOn = false;

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	//アナログ入力
	//ここではADCの変換値を100-3500程度と想定
	//2800を上回ったとき、または、800を下回ったときを検出する
	devA.Initialize(GpioAnalog);
	devA.SetRisingThreshold(2800, devA.DeadBandAdc * 2);
	devA.SetFallingThreshold(800, devA.DeadBandAdc * 2);
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

	auto adcVal = devA.UpdateState();
	if (devA.IsHighRange()	&& (range != 1)) { Serial.println("高域"); range = 1; }
	if (devA.IsLowRange()	&& (range != 2)) { Serial.println("低域"); range = 2; }
	if (devA.IsMidRange()	&& (range != 3)) { Serial.println("中域"); range = 3; }
	auto aState = devA.GetState();

	devD.UpdateState();
	if (devD.IsSwOn() && !swOn) { Serial.println("押してる"); swOn = true; }
	if (devD.IsSwOff() && swOn) { Serial.print("放してる"); swOn = false; }
	auto dState = devD.GetState();

	if ((devAState != aState) || (devDState != dState))
	{
		devAState = aState;
		devDState = dState;
		Print(devAState, devDState);

		Serial.printf("devA: ADC=%d\n", adcVal);
	}

	delay(10);
}

//状態表示
static	void	Print(PotMeter::State devAState, HwSwitch::State devDState)
{
	Serial.printf("devA:%s devD:%s\n", ToString(devAState).c_str(), ToString(devDState).c_str());
}

static	std::string	ToString(PotMeter::State state)
{
	switch (state)
	{
	case	PotMeter::State::HighRange:			return "HighRange";
	case	PotMeter::State::LowRange:			return "LowRange";
	case	PotMeter::State::MidRange:			return "MidRange";
	case	PotMeter::State::OnFallFromHigh:	return "OnFallFromHigh";
	case	PotMeter::State::OnFallToLow:		return "OnFallToLow";
	case	PotMeter::State::OnRiseFromLow:		return "OnRiseFromLow";
	case	PotMeter::State::OnRiseToHigh:		return "OnRiseToHigh";
	default:	return "";
	}
}

static	std::string	ToString(HwSwitch::State state)
{
	switch (state)
	{
	case	HwSwitch::State::Free:			return "Free";
	case	HwSwitch::State::Holding:		return "Holding";
	case	HwSwitch::State::LongHolding:	return "LongHolding";
	case	HwSwitch::State::LongPress:		return "LongPress";
	case	HwSwitch::State::Press:			return "Press";
	case	HwSwitch::State::Release:		return "Release";
	default:	return "";
	}
}
