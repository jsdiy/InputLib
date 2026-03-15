//	アナログ入力
//	potentio_meter, slid_volune, stick/lever, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/03	初版

#pragma once

#include <Arduino.h>
#include "AnalogInput.hpp"

class	PotMeter	: public AnalogInput
{
public:
	//閾値で区切られたADC値の状態とイベント
	/*
	         →||←hys       →||←hys
	ADC:0 ----++------------++----> max
	       thr↑|            |↑thr

	ADC:0 ----++------------++----> max
	LowRange ←|←  MidRange  →|→ HighRange
	          ↑              ↑OnRiseToHigh (from Mid/Low)
	          ↑OnFallToLow (from Mid/High)

	ADC:0 ----++------------++----> max
	LowRange  →|→ MidRange ←|← HighRange
	           ↑            ↑OnFallFromHigh (to Mid/Low)
	           OnRiseFromLow (to Mid/High)
	*/
	enum	class	State	: uint8_t
	{
		HighRange		= 0x11,
		OnRiseToHigh	= 0x12,
		OnFallFromHigh	= 0x21,
		MidRange		= 0x22,
		OnRiseFromLow	= 0x23,
		OnFallToLow		= 0x41,
		LowRange		= 0x42,
	};
	
	static	constexpr	uint8_t	HighMask	= 0x10;
	static	constexpr	uint8_t	MidMask		= 0x20;
	static	constexpr	uint8_t	LowMask		= 0x40;

public:
	static	constexpr	int16_t	DeadBandAdc	= 20;	//4095の0.5%（不感帯やヒステリシスのヒント）
	static	constexpr	int16_t	DeadBandMV	= 16;	//3300mVの0.5%（不感帯やヒステリシスのヒント）

protected:
	struct	Threshold { int16_t threshold = 0, hysteresis = 0; };

protected:
	Threshold	thrRise, thrFall;
	void	SetThreshold(Threshold& obj, int16_t threshold, int16_t hysteresis);
	State	currentState = State::MidRange;
	State	GetNextState(int16_t curentAdcVal);

public:
	PotMeter() {}
	void	Initialize(gpio_num_t analogPin);
	void	SetRisingThreshold(int16_t threshold, int16_t hysteresis = DeadBandAdc) { SetThreshold(thrRise, threshold, hysteresis); }
	void	SetFallingThreshold(int16_t threshold, int16_t hysteresis = DeadBandAdc) { SetThreshold(thrFall, threshold, hysteresis); }
	int16_t	UpdateState();	//loop()やタイマーで呼び出す
	State	GetState() const { return currentState; }
	bool	IsHighRange() const { return (static_cast<uint8_t>(currentState) & HighMask); }
	bool	IsMidRange() const { return (static_cast<uint8_t>(currentState) & MidMask); }
	bool	IsLowRange() const { return (static_cast<uint8_t>(currentState) & LowMask); }
};
