#pragma once
#include "BaseScene.h"
#include <string>

//アクティブリコールクラス
class Title : public BaseScene {
private:
	bool isInputMode = false;  //メモ入力画面
	int  inputHandle = -1;     //DxLibの文字入力
	int walkBGM;
	int doorBGM;
public:
	Title() {
		walkBGM = LoadSoundMem("サウンド\\ヒール音.m4a");
		doorBGM = LoadSoundMem("サウンド\\ドア.mp3");
	}
	~Title() {
		StopSoundMem(walkBGM);
		StopSoundMem(doorBGM);
	}
	SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};
