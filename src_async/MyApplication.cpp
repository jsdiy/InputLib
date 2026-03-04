//	デジタル入力・アナログ入力ライブラリー	ESP32,ArduinoFramework,C++
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include "MyApplication.hpp"

void	MyApplication::Setup(PotMeterA& devA, HwSwitchA& devD)
{
	FlagReset();

	//アナログ入力
	//ここではADCの変換値を100-3500程度と想定
	//2800を上回ったとき、または、800を下回ったときを検出する
	devA.SetRisingThreshold(2800);
	devA.SetFallingThreshold(800);
	//コールバックを登録する
	devA.RisingCb().Add([this](uint32_t n){ this->OnRising(n); });
	devA.FallingCb().Add([this](uint32_t n){ this->OnFalling(n); });
	devA.StartMonitoring();	//監視開始

	//デジタル入力
	//押しボタンスイッチが接続されていると想定
	//コールバックを登録する
	devD.ClickedCb().Add([this](){ this->OnClicked(); });
	devD.ChangeCb().Add([this](uint32_t n){ this->OnChange(n); });
	devD.StartMonitoring();	//監視開始
}

//コールバック（フラグ設定程度にとどめ、必要な処理はloop()で行うこと）
void	MyApplication::OnRising(uint32_t arg)
{
	this->arg = arg;	//ADC値
	flagOnRising = true;
}

//コールバック（フラグ設定程度にとどめ、必要な処理はloop()で行うこと）
void	MyApplication::OnFalling(uint32_t arg)
{
	this->arg = arg;	//ADC値
	flagOnFalling = true;
}

//コールバック（フラグ設定程度にとどめ、必要な処理はloop()で行うこと）
void	MyApplication::OnClicked()
{
	flagOnClicked = true;
}

//コールバック（フラグ設定程度にとどめ、必要な処理はloop()で行うこと）
void	MyApplication::OnChange(uint32_t arg)
{
	this->arg = arg;	// RISING/FALLING
	flagOnChange = true;
}

//時間がかかる関数
void	MyApplication::HeavyFunction()
{
	//例）LCD画面を更新し続ける処理で、1画面描画するごとにフラグを確認する
	while (1)
	{
		//Lcd.DrawImage();
		delay(10);	//Lcd.DrawImage()に時間がかかることのシミュレーション
		
		//イベントが発生していたら処理を抜ける
		if (flagOnRising || flagOnFalling || flagOnClicked || flagOnChange) { break; }
	}
}
