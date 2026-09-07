#pragma once
#include "BaseScene.h"
#include <vector>
#include <string>

//BaseSceneを継承してホーム画面を作成
class HomeScene : public BaseScene {
private:
    //入力欄の管理番号（ハンドル）
    int inputHandle = -1;
    bool isInputMode = false;
    bool isIconSelectMode = false; //アイコン選択画面を開いているか
    QuestType inputType = QuestType::Daily;
    int expIconHandle;
    int levelIconHandle;
    int coinIconHandle;
    int CoinMarkHandle;
    int ClearIconHandle;
    int ShopHandle;
    int ActiveRecallHandle;
    int MinigameHandle;
    int TimerHandle;
    int DailyHandle;
    int DailyFrameHandle;
    int RedStarHandle;
    int WeeklyHandle;
    int WeeklyFrameHandle;
    int BlueStarHandle;
    int DeleteHandle;
    int Magic1Handle;
public:
    HomeScene() {
        expIconHandle = LoadGraph("タスク\\経験値アイコン.png");
        levelIconHandle = LoadGraph("タスク\\Levelアイコン2.png");
        coinIconHandle = LoadGraph("タスク\\coinアイコン.png");
        CoinMarkHandle = LoadGraph("タスク\\coinIcon.png");
        ClearIconHandle = LoadGraph("タスク\\Clearアイコン.png");
        ShopHandle = LoadGraph("タスク\\ショップボタン.png");
        ActiveRecallHandle = LoadGraph("タスク\\アクティブリコール.png");
        MinigameHandle = LoadGraph("タスク\\minigame.png");
        TimerHandle = LoadGraph("タスク\\Timer.png");
        DailyHandle = LoadGraph("タスク\\デイリークエスト追加.png");
        DailyFrameHandle = LoadGraph("タスク\\デイリークエスト枠.png");
        RedStarHandle = LoadGraph("タスク\\赤い星.png");
        WeeklyHandle = LoadGraph("タスク\\ウィークリークエスト追加.png");
        WeeklyFrameHandle = LoadGraph("タスク\\ウィークリークエスト枠.png");
        BlueStarHandle = LoadGraph("タスク\\青い星.png");
        DeleteHandle = LoadGraph("タスク\\削除ボタン.png");
        Magic1Handle = LoadGraph("タスク\\1SUNANDMOONLIFEBACKHERE.png");
    }
    ~HomeScene() {
        DeleteGraph(expIconHandle);
        DeleteGraph(levelIconHandle);
        DeleteGraph(coinIconHandle);
        DeleteGraph(CoinMarkHandle);
        DeleteGraph(ClearIconHandle);
        DeleteGraph(ShopHandle);
        DeleteGraph(ActiveRecallHandle);
        DeleteGraph(MinigameHandle);
        DeleteGraph(TimerHandle);
        DeleteGraph(DailyHandle);
        DeleteGraph(DailyFrameHandle);
        DeleteGraph(RedStarHandle);
        DeleteGraph(WeeklyHandle);
        DeleteGraph(WeeklyFrameHandle);
        DeleteGraph(BlueStarHandle);
        DeleteGraph(DeleteHandle);
        DeleteGraph(Magic1Handle);
    }

    // 親クラスの関数をオーバーライド
    SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};