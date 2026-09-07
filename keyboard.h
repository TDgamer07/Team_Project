#pragma once
#include "DxLib.h"
#define FUNC_ERR(x) MessageBox(NULL, "次の関数でエラーになりました", x, MB_OK)
//使用するキーの種類指定してください
//上限8種類
const DWORD KEY_TO_VK[] = {
	KEY_INPUT_UP,
	KEY_INPUT_DOWN,
	KEY_INPUT_LEFT,
	KEY_INPUT_RIGHT,
	KEY_INPUT_SPACE,
	KEY_INPUT_RETURN,
	KEY_INPUT_X,
};
//IsKeyPush関数の引数に使用します。
//上記　KEY_TO_VK配列と対応させて、指定してください。
//最大8種類まで＋最後にKEY_MAXを入れておいてください

enum KEY_TYPE {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_SPACE,
	KEY_RETURN,
	KEY_X,
	KEY_MAX,
};
//
// 毎フレーム最初に呼び出す
HRESULT KeyUpdate();
//
// キーボードが押下された瞬間だけ真、その他は偽を返す。
// 今回使用できるキーボードの種類は8種類までです。
BOOL IsKeyPush(KEY_TYPE keyNo);
BOOL IsKeyRelease(KEY_TYPE keyNo);
//
// キーが離された瞬間だけ真、その他は偽を返す。
// 今回使用できるキーボードの種類は８種類までです。
// 各自で制作してください
