//	ハードウェアスイッチ
//	push_switch, toggle_switch, slide_switch, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include <Arduino.h>
#include "HwSwitch.hpp"

//初期化
void	HwSwitch::Initialize(gpio_num_t swPin)
{
	DigitalInput::Initialize(swPin);
	ActiveLowSwitch();
	prevPinVal = SwOff;
	UpdateState();
}

//新たなスイッチの状態を取得する
SwState	HwSwitch::GetNextState(int8_t currentPinVal)
{
	auto nowTime = millis();

	if (prevPinVal == SwOff && currentPinVal == SwOn)
	{
		swState = SwState::On;
		holdStartTime = nowTime;
	}
	else if (prevPinVal == SwOn && currentPinVal == SwOn)
	{
		swState = (nowTime - holdStartTime < longHoldThresholdTime)
			? SwState::ShortHold : SwState::LongHold;
	}
	else if (prevPinVal == SwOn && currentPinVal == SwOff)
	{
		swState = SwState::Release;
		holdStartTime = 0;
	}
	else	//(prevPinVal == SwOff && currentPinVal == SwOff)
	{
		swState = SwState::Off;
	}

	prevPinVal = currentPinVal;
	return swState;
}

//状態を更新する
//・swStateを更新する。
//・戻り値は現在のピンの値。
int8_t	HwSwitch::UpdateState()
{
	int8_t currentPinVal = Read();
	swState = GetNextState(currentPinVal);
	return currentPinVal;
}
