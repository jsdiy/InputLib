//	アナログ入力
//	potentio_meter, slid_volune, stick/lever, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include <Arduino.h>
#include "PotMeter.hpp"

void	PotMeter::Initialize(gpio_num_t analogPin)
{
	AnalogInput::Initialize(analogPin);
	SetRisingThreshold(INT16_MAX, 0);
	SetFallingThreshold(INT16_MIN, 0);
	UpdateState();
}

void	PotMeter::SetThreshold(Threshold& obj, int16_t threshold, int16_t hysteresis)
{
	obj.threshold = threshold;
	obj.hysteresis = hysteresis;
}

AdcThrState	PotMeter::GetNextState(int16_t curentVal)
{
	if (thrState == AdcThrState::Rising) { thrState = AdcThrState::Above; }
	if (thrState == AdcThrState::Falling) { thrState = AdcThrState::Bellow; }
	AdcThrState nextThrState = thrState;

	if (thrRise.threshold <= curentVal)	//※A
	{
		if (thrState != AdcThrState::Above) { nextThrState = AdcThrState::Above; }
	}

	if (curentVal < thrRise.threshold - thrRise.hysteresis)
	{
		if (thrState == AdcThrState::Above) { nextThrState = AdcThrState::None; }
		//NoneではなくBellowの可能性もあるが、それはこの後※Bで引っかかる
	}

	if (curentVal <= thrFall.threshold)	//※B
	{
		if (thrState != AdcThrState::Bellow) { nextThrState = AdcThrState::Bellow; }
	}

	if (thrFall.threshold + thrFall.hysteresis < curentVal)
	{
		if (thrState == AdcThrState::Bellow) { nextThrState = AdcThrState::None; }
		//NoneではなくAboveの可能性もあるが、それは既に※Aで引っかかっており、ここへは来ない
	}

	if ((thrState != AdcThrState::Above) && (nextThrState == AdcThrState::Above)) { nextThrState = AdcThrState::Rising; }
	if ((thrState != AdcThrState::Bellow) && (nextThrState == AdcThrState::Bellow)) { nextThrState = AdcThrState::Falling; }

	return nextThrState;
}

//状態を更新する
//・thrStateを更新する。
//・戻り値は現在のADC値。
int16_t	PotMeter::UpdateState()
{
	int16_t curentVal = Read();
	thrState = GetNextState(curentVal);
	return curentVal;
}
