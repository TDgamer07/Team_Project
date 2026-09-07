#pragma once
#include "BaseScene.h"
#include <string>

//アクティブリコールクラス
class ActiveRecall : public BaseScene {
private:
	bool isInputMode = false;  //メモ入力画面
	int  inputHandle = -1;     //DxLibの文字入力
	int  Magic2Handle;
public:
	ActiveRecall() {
		Magic2Handle = LoadGraph("タスク\\2EYEWATCHUNDERWORLD.png");
	}
	~ActiveRecall() {
		DeleteGraph(Magic2Handle);
	}
	SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};
