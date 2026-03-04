# InputLib
デジタル入力/アナログ入力ライブラリー。Arduino Framework環境、ESP32用。

## 概要
- ESP32でボタンスイッチやアナログレバーなどを使用する際に、ピン入力を読み取るライブラリです。
- 自分がパッと使えるように置いています。
- 予告なしに改造します。改造前との互換性は考慮しません。

## 動作確認環境
- Windows11
- PlatformIO + Arduino Framework
- ESP32, ESP32-C3
- タクトスイッチ(Tactile Switch)、サムスティック(Thumbstick)

## サンプルコードの使い方
1. \<src>フォルダを削除（またはリネーム）する。
1. \<src_input>, \<src_sync>, \<src_async>フォルダのいずれかを\<src>にリネームする。
1. ビルド、転送、実行。
1. ボタンやレバーを操作すると、[ターミナル]タブ（シリアルモニター）に結果が出力される。

## フォルダ構成
```
<InputLib>
  +--<DigitalInput>
  |    +--DigitalInput
  +--<AnalogInput>
  |    +--AnalogInput
  +--<HwSwitch>
  |    +--HwSwitch
  |    +--HwSwitchA
  +--<PotMeter>
  |    +--PotMeter
  |    +--PotMeterA
  +--<CallbackHandler>
       +--CallbackHandler
```
## クラス図（概要）
```
main -----◆ DigitalInput

main -----◆ HwSwitch -----△ DigitalInput

main -----◆ HwSwitchA +-----△ HwSwitch -----△ DigitalInput
                       +-----◆ CallbackHandler


main -----◆ AnalogInput

main -----◆ PotMeter -----△ AnalogInput

main -----◆ PotMeterA +-----△ PotMeter -----△ AnalogInput
                       +-----◆ CallbackHandler
```
#### 凡例  
```
A-----◆B	クラスAはメンバーとしてクラスBの実体を持つ  
A-----△B	クラスAはクラスBを継承する  
		・Aが派生クラス、Bが基底クラス  
```
## クラスの説明：デジタル入力
### DigitalInput
- チャタリング対策付きのdigitalRead()

### HwSwitch
- ボタンスイッチ、トグルスイッチ、スライドスイッチなどを想定
- オン、オフ、長押し、クリックなどの状態を判定することができる（同期処理）
- アクティブ・ロー／ハイどちらの接続方式にも対応  
	例: pin(プルアップ)--プッシュオンスイッチ--GND  
	例: pin(プルアップ)--プッシュオフスイッチ--GND
```
enum class SwState : uint8_t { On, ShortHold, LongHold, Release, Off };
押しボタンスイッチの操作に対する判定の状態
  何もしていない|<押した>|      押し続けている     |<放した> |何もしていない
  Off--------->|   On  |ShortHold-->|LongHold-->|Release |Off---------->
                                    ↑長押し開始  |クリック |
※OnとReleaseは一瞬の出来事
※ShortHoldからLongHoldへ変化する瞬間が長押し開始の判定箇所
※<放した>瞬間がクリックの判定箇所
```

### HwSwitchA
- HwSwitchの非同期版（Ticker.attach_ms()によるポーリング）
- コールバックを登録し、イベント検出時に実行することができる
- イベントは、クリック、長押し開始、RISING, FALLING, CHANGEに対応  
	CHANGEはRISING/FALLINGの区別が可能

## クラスの説明：アナログ入力
### AnalogInput
- 平滑化対応のanalogRead()/analogReadMilliVolts()

### PotMeter
- ボリューム（VR/可変抵抗器）、スライドボリューム、ジョイスティック/レバーなどを想定
- 上限/下限の閾値・ヒステリシスを設定し、それらを越えたか判定することができる（同期処理）
```
enum class AdcThrState : uint8_t { None, Rising, Above, Falling, Bellow };
閾値に対する判定の状態
ADC:0 ----+------------+----> max
Bellow <--|<-- None -->|--> Above
          ↑Falling     ↑Rising
```

### PotMeterA
- PotMeterの非同期版（Ticker.attach_ms()によるポーリング）
- コールバックを登録し、イベント検出時に実行することができる
- イベントは、上限を上回った、下限を下回った、に対応

## クラスの説明：デジタル入力/アナログ入力に共通
### CallbackHandler
- コールバックの登録、削除、実行を管理する
- 登録できる関数の型は、void func(void), void func(uint32_t)

### イベントとコールバック関数の引数の関係
- どのイベントもコールバック関数は void func(void)型を登録できる
- void func(uint32_t)型を登録した場合は受け取れる引数がイベントによって異なる

|HwSwitchAで発生するイベント|イベントの説明|void func(uint32_t n) の n|
|---|---|---|
|OnClicked|クリックされた|0|
|OnHeld|長押しが開始された|0|
|OnRising|attachInterrupt(,,RISING)に相当|0|
|OnFalling|attachInterrupt(,,FALLING)に相当|0|
|OnChange|attachInterrupt(,,CHANGE)に相当|RISING または FALLING|

|PotMeterAで発生するイベント|イベントの説明|void func(uint32_t n) の n|
|---|---|---|
|OnRising|設定した上限値を上回った|そのときのADCの値（実質int16_t型）|
|OnFalling|設定した下限値を下回った|そのときのADCの値（実質int16_t型）|

-----
以上
