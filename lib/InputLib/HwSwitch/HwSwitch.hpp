//	ハードウェアスイッチ
//	push_switch, toggle_switch, slide_switch, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
/*
2025/01	AVR版(2019/01)を移植、ReleaseとOffの意味を入れ替え
2025/09	ESP32版としてmills()利用へ改造
2025/10	Clicked()などの状態を追加。利便性向上
2026/03	HwSwitchをDigitalInputとHwSwitchに分割、HwSwitchを非同期版作成に合わせて改造
*/

#pragma	once

#include <Arduino.h>
#include "DigitalInput.hpp"

//スイッチの押下状態
enum	class	SwState	: uint8_t
{
	On			= 0x11,	//オンになった瞬間
	ShortHold	= 0x12,	//オンであり続けている状態（指定時間未満）
	LongHold	= 0x13,	//オンであり続けている状態（指定時間以上経過）
	Release		= 0x01,	//オフになった瞬間
	Off			= 0x02	//オフであり続けている状態
};

//ハードウェアスイッチ
class	HwSwitch	: public DigitalInput
{
protected:
	static	constexpr	uint8_t	SwOnMask = 0x10;
	static	constexpr	ulong	TimeInfinity = ULONG_MAX;

private:
	int8_t	SwOn, SwOff;	//HIGH/LOWが割り当てられる
	ulong	longHoldThresholdTime = TimeInfinity, holdStartTime = 0;
	int8_t	prevPinVal;
	SwState	swState = SwState::Off;
	SwState	GetNextState(int8_t currentPinVal);

public:
	HwSwitch() {}
	void	Initialize(gpio_num_t swPin);
	void	ActiveLowSwitch() { SwOn = LOW; SwOff = HIGH; }	//デフォルトではActiveLowのスイッチを想定
	void	ActiveHighSwitch() { SwOn = HIGH; SwOff = LOW; }
	void	SetLongHoldThresholdTime(ulong millis) { longHoldThresholdTime = millis; }
	int8_t	UpdateState();	//loop()やタイマーで呼び出す
	SwState	GetState() const { return swState; }
	bool	Clicked() const { return (swState == SwState::Release); }
	bool	Holding() const { return (swState == SwState::LongHold); }	//LongHoldし続けている状態
	bool	IsOn() const { return (static_cast<uint8_t>(swState) & SwOnMask); }
	bool	IsOff() const { return !IsOn(); }
};

/*
ActiveLowSwitch()
・スイッチ作動でピンにLOW が入力される回路の場合。例: pin(プルアップ)--プッシュオンスイッチ--GND
ActiveHighSwitch()
・スイッチ作動でピンにHIGH が入力される回路の場合。例: pin(プルアップ)--プッシュオフスイッチ--GND
*/
