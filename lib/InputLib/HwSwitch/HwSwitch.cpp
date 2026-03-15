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
	prevPinVal = Read();
	UpdateState();
}

//新たなスイッチの状態を取得する
HwSwitch::State	HwSwitch::GetNextState(int8_t currentPinVal)
{
	auto nowTime = millis();

	if (prevPinVal == SwOff && currentPinVal == SwOn)
	{
		currentState = State::Press;
		holdStartTime = nowTime;
	}
	else if (prevPinVal == SwOn && currentPinVal == SwOn)
	{
		/*
		currentState = (nowTime - holdStartTime < longHoldThresholdTime)
			? SwState::ShortHold : SwState::LongHold;
		*/
		State prevState = currentState, nextState = State::Holding;
		if (longHoldThresholdTime <= nowTime - holdStartTime)
		{
			nextState = (prevState == State::Holding) ? State::LongPress : State::LongHolding;
		}
		currentState = nextState;
	}
	else if (prevPinVal == SwOn && currentPinVal == SwOff)
	{
		currentState = State::Release;
		holdStartTime = 0;
	}
	else	//(prevPinVal == SwOff && currentPinVal == SwOff)
	{
		currentState = State::Free;
	}

	prevPinVal = currentPinVal;
	return currentState;
}

//状態を更新する
//・currentStateを更新する。
//・戻り値は現在のピンの値。
int8_t	HwSwitch::UpdateState()
{
	int8_t currentPinVal = Read();
	currentState = GetNextState(currentPinVal);
	return currentPinVal;
}
