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
	void	FlagReset() { flagOnRising = flagOnFalling = flagOnClicked = flagOnChange = false; }
	uint32_t	GetArg() const { return arg; }
	void	HeavyFunction();	//重い処理の例

	//コールバック関数（クラス外部でコールバックを登録するのであればpublicにする）
private:
	void	OnRising(uint32_t arg);
	void	OnFalling(uint32_t arg);
	void	OnClicked();
	void	OnChange(uint32_t arg);

	//コールバック関数でセットされるフラグ（面倒なのでpublicとした）
public:
	volatile	bool	flagOnRising;
	volatile	bool	flagOnFalling;
	volatile	bool	flagOnClicked;
	volatile	bool	flagOnChange;
};
