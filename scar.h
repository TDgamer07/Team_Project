#pragma once
#include "BaseScene.h"
#include <string>

//アクティブリコールクラス
class scar : public BaseScene {
public:
	scar() {}
	~scar() {}
	SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};

