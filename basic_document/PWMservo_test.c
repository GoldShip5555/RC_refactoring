#include <stdint.h>

#define NUM_DIGITAL_PINS 32
#define NO_ANGLE 255

uint32_t attachedpins[(NUM_DIGITAL_PINS + 31) / 32]; // 1 bit per digital pin

typedef struct
{
	uint8_t pin;   // サーボモータの接続されているデジタルピンの番号
	uint8_t angle; // サーボモータの現在の角度
	int min16;	   // サーボモータの設定可能な最小角度（16で割られた値）
	int max16;	   // サーボモータの設定可能な最大角度（16で割られた値）
} PWMServo;
//サーボの初期化処理。コンストラクタの替わり
PWMServo PWMServo_init()
{
	PWMServo s;
	s.pin = 255;		// ピン番号を255（無効な値）に初期化
	s.angle = NO_ANGLE; // 角度をNO_ANGLE（255）に初期化
	return s;
}

uint8_t PWMServo_attach(PWMServo *servo, int pinArg, int min, int max)
{
	if (pinArg < 0 || pinArg >= NUM_DIGITAL_PINS)
		return 0; // ピン番号が範囲外の場合、エラーを返す
	if (!digitalPinHasPWM(pinArg))
		return 0;											   // PWMに対応していないピンの場合、エラーを返す
	servo->pin = pinArg;									   // サーボモータの構造体にピン番号を設定
	analogWriteFrequency(servo->pin, 50);					   // ピンの周波数を50に設定
	servo->min16 = min >> 4;								   // 最小値を16で割った値を設定
	servo->max16 = max >> 4;								   // 最大値を16で割った値を設定
	servo->angle = NO_ANGLE;								   // 角度をNO_ANGLEに設定
	digitalWrite(servo->pin, LOW);							   // ピンの出力をLOW（0V）に設定
	pinMode(servo->pin, OUTPUT);							   // ピンを出力モードに設定
	attachedpins[servo->pin >> 5] |= (1 << (servo->pin & 31)); // 使用中のピンを記録する
	return 1;												   // 成功を示す値1を返す
}


