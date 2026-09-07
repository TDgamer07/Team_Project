#pragma once
#include "BaseScene.h"

//BaseScene.hを継承してタイマー画面を作成
class TimerScene : public BaseScene {
private:
    int TimerCount = 0; //タイマーの時間開始時刻
    int imageHandle;    // PNG画像を管理するための変数を追加
public:
    TimerScene();
    ~TimerScene();      // 「{}」を消して、cpp側で処理を書くように変更
    SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};