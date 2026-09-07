#include "keyboard.h"

static BYTE gPreKeyStat = 0;
static BYTE gNowKeyStat = 0;
// 毎フレーム最初に呼び出す
HRESULT KeyUpdate() {
	char Buf[256];
	if (GetHitKeyStateAll(Buf) == -1) {
		FUNC_ERR("GetHitKeyStateAll");
		return E_FAIL;
	}
	gPreKeyStat = gNowKeyStat;
	gNowKeyStat = 0;
	for (int i = 0; i < KEY_MAX; i++) {
		if (Buf[KEY_TO_VK[i]] == 1) {
			gNowKeyStat |= (1 << i);
		}
	}

	return S_OK;
}
//
// キーボードが押下された瞬間だけ真、その他は偽を返す。
// 今回使用できるキーボードの種類は8種類までです。
BOOL IsKeyPush(KEY_TYPE keyNo) {
	bool pre = (gPreKeyStat & (1 << keyNo)) > 0;
	bool now = (gNowKeyStat & (1 << keyNo)) > 0;
	return !pre && now;
}
BOOL IsKeyRelease(KEY_TYPE keyNo) {
	bool pre = (gPreKeyStat & (1 << keyNo)) > 0;
	bool now = (gNowKeyStat & (1 << keyNo)) > 0;
	return pre && !now;
}
//
// キーが離された瞬間だけ真、その他は偽を返す。
// 今回使用できるキーボードの種類は８種類までです。
// 各自で制作してください


