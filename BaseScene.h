#pragma once
#include "Common.h"

//すべての画面の親クラス
class BaseScene {
public:
    virtual ~BaseScene() {}

    //毎フレーム呼ばれる処理。
    //次の画面のIDを戻り値として返す
    virtual SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) = 0;
};