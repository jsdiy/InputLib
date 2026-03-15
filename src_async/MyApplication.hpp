//	デジタル入力・アナログ入力ライブラリー	ESP32,ArduinoFramework,C++
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#pragma once

#include <Arduino.h>
#include "HwSwitchA.hpp"	//非同期版はxxxA
#include "PotMeterA.hpp"	//非同期版はxxxA

class	MyApplication
{
private:
	HwSwitchA	devD;	//push_switch, toggle_switch, slide_switch, ...
	PotMeterA	devA;	//potentio_meter, slid_volune, stick/lever, ...
	volatile	uint32_t	arg = 0;	//コールバック関数内でセットされる値
	
public:
	MyApplication() {}
	void	Setup(PotMeterA& devA, HwSwitchA& devD);
	void	FlagReset() { flagOnFallFromHigh = flagOnFallToLow = flagOnRiseFromLow = flagOnRiseToHigh =
						flagOnPress = flagOnLongPress = flagOnRelease = flagOnChange = false; }
	uint32_t	GetArg() const { return arg; }
	void	HeavyFunction();	//重い処理の例

	//コールバック関数（クラス外部でコールバックを登録するのであればpublicにする）
private:
	void	OnFallFromHigh(uint32_t arg);
	void	OnFallToLow(uint32_t arg);
	void	OnRiseFromLow(uint32_t arg);
	void	OnRiseToHigh(uint32_t arg);

	void	OnPress();
	void	OnLongPress();
	void	OnRelease();
	void	OnChange(uint32_t arg);

	//コールバック関数でセットされるフラグ（面倒なのでpublicとした）
public:
	volatile	bool	flagOnFallFromHigh;
	volatile	bool	flagOnFallToLow;
	volatile	bool	flagOnRiseFromLow;
	volatile	bool	flagOnRiseToHigh;

	volatile	bool	flagOnPress;
	volatile	bool	flagOnLongPress;
	volatile	bool	flagOnRelease;
	volatile	bool	flagOnChange;
};
