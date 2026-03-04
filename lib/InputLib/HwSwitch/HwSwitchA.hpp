//	ハードウェアスイッチ 非同期版
//	push_switch, toggle_switch, slide_switch, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#pragma	once

#include <Arduino.h>
#include <Ticker.h>
#include "HwSwitch.hpp"
#include "CallbackHandler.hpp"

//ハードウェアスイッチ 非同期版
class	HwSwitchA	: public HwSwitch
{
protected:
	static	constexpr	uint32_t	TickMSec = 50;
	//50ms --> 監視間隔は秒間20回	※DigitalInput::DebounceTimeMillisより長い方がよい

private:
	Ticker	ticker;
	CallbackHandler	onClicked, onHeld;
	CallbackHandler	onRising, onFalling, onChange;	//attachInterrupt()のトリガーに対応するもの
	void	EventTrigger(int8_t pinVal, SwState prevSwState);

public:
	HwSwitchA() {}
	void	StartMonitoring(uint32_t tickMillis = TickMSec);	//監視開始	※UpdateState()をポーリングする
	void	StopMonitoring() { ticker.detach(); }	//監視停止
	int8_t	UpdateState();	//戻り値は現在のピン状態(HIGH/LOW)
	CallbackHandler&	ClickedCb() { return onClicked; }
	CallbackHandler&	HeldCb() { return onHeld; }	//ShortHoldからLongHoldへ変化したタイミング
	CallbackHandler&	RisingCb() { return onRising; }
	CallbackHandler&	FallingCb() { return onFalling; }
	CallbackHandler&	ChangeCb() { return onChange; }
};
