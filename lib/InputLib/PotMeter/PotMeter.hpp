//	アナログ入力
//	potentio_meter, slid_volune, stick/lever, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#pragma once

#include <Arduino.h>
#include "AnalogInput.hpp"

//閾値に対する値の状態
enum	class	AdcThrState	: uint8_t
{
	//ADC:0 --+--------+--> max
	//Bellow ←|← None →|→ Above
	//        ↑Falling ↑Rising
	None,
	Rising,	Above, 
	Falling, Bellow,
};

class	PotMeter	: public AnalogInput
{
public:
	static	constexpr	int16_t	DeadBandAdc	= 20;	//4096の0.5%（不感帯やヒステリシスのヒント）
	static	constexpr	int16_t	DeadBandMV	= 16;	//3300mVの0.5%（不感帯やヒステリシスのヒント）

protected:
	struct	Threshold { int16_t threshold = 0, hysteresis = 0; };

protected:
	Threshold	thrRise, thrFall;
	void	SetThreshold(Threshold& obj, int16_t threshold, int16_t hysteresis);
	AdcThrState	thrState = AdcThrState::None;
	AdcThrState	GetNextState(int16_t curentVal);

public:
	PotMeter() {}
	void	Initialize(gpio_num_t analogPin);
	void	SetRisingThreshold(int16_t threshold, int16_t hysteresis = DeadBandAdc) { SetThreshold(thrRise, threshold, hysteresis); }
	void	SetFallingThreshold(int16_t threshold, int16_t hysteresis = DeadBandAdc) { SetThreshold(thrFall, threshold, hysteresis); }
	int16_t	UpdateState();	//loop()やタイマーで呼び出す
	AdcThrState	GetState() const { return thrState; }
	bool	IsRising() const { return (thrState == AdcThrState::Rising); }
	bool	IsFalling() const { return (thrState == AdcThrState::Falling); }
};
