#include "Arduino.h"
#include "PWMServo.h"
// NO_ANGLEという名前のマクロを定義。これはサーボが接続されていないことを示す値として使用
#define NO_ANGLE (0xff)
//デジタルピンの接続状態を保存する静的な配列を定義。各ビットはピンの接続状態を表す
uint32_t PWMServo::attachedpins[(NUM_DIGITAL_PINS+31)/32]; // 1 bit per digital pin
// コンストラクタを定義。新しいPWMServoオブジェクトが作成されるときにこのメソッドが実行され、ピンと角度を初期化。
PWMServo::PWMServo() : pin(255), angle(NO_ANGLE) {}
//サーボを特定のピンに接続するメソッドを定義。引数は接続するピンと、サーボの最小/最大パルス幅。
uint8_t PWMServo::attach(int pinArg, int min, int max)
{
    //ピン番号が適正かどうかチェック、書き直したらいらないかも
  if (pinArg < 0 || pinArg >= NUM_DIGITAL_PINS) return 0;
  if (!digitalPinHasPWM(pinArg)) return 0;
  pin = pinArg;
  //analogWriteの周期を設定する。
  analogWriteFrequency(pin, 50);
  //MAX設定値を16で割る
  min16 = min >> 4;
  max16 = max >> 4;
  //アングル無し設定
  angle = NO_ANGLE;
  //ピンモード設定
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  attachedpins[pin >> 5] |= (1 << (pin & 31));
  return 1;
}
//サーボを指定した角度に回転させるメソッドを定義。引数はサーボを回転させる角度。
void PWMServo::write(int angleArg)
{
  if (pin >= NUM_DIGITAL_PINS) return;
  if (angleArg < 0) angleArg = 0;
  if (angleArg > 180) angleArg = 180;
  angle = angleArg;
  uint32_t us = (((max16 - min16) * 46603 * angle) >> 11) + (min16 << 12); // us*256
  uint32_t duty = (us * 3355) >> 22;
  analogWriteResolution(12);
  analogWrite(pin, duty);
}
//サーボが現在接続されているかどうかを返すメソッドを定義。接続されている場合は1を、そうでない場合は0を返す。
//なんで見に行ってるかよくわからない多重接続や再接続拒否するため？
uint8_t PWMServo::attached()
{
  if (pin >= NUM_DIGITAL_PINS) return 0;
  return (attachedpins[pin >> 5] & (1 << (pin & 31))) ? 1 : 0;
}
