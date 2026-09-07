#include "DxLib.h"
#include "Common.h"
#include "BaseScene.h"
#include "HomeScene.h"
#include "MiniGame.h"
#include "TimerScene.h"
#include "ActiveRecall.h"
#include "ShopScene.h"
#include "Title.h"
#include "scar.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const int WIDTH = 1240, HEIGHT = 700;
    SetWindowText("TASK ROOM");
    SetGraphMode(WIDTH, HEIGHT, 32);
    ChangeWindowMode(TRUE);
    if (DxLib_Init() == -1) return -1;
    SetBackgroundColor(10, 10, 15);
    SetDrawScreen(DX_SCREEN_BACK);

    int timer = 0;
    //全員で共有するプレイヤーデータ
    Player player;
    //プレイヤーの情報をロードする処理
    player.Load();

    SceneID currentSceneID = SceneID::Title;

    //現在の画面をポインタで管理する
    BaseScene* currentScene = new Title();

    // 最初にメインBGMをループ再生しておく
    PlaySoundMem(player.mainBGM, DX_PLAYTYPE_LOOP);

    int oldMouse = 0;

    while (1) {
        ClearDrawScreen();
        timer++;

        // マウスのクリック状態を取得
        int mouse = GetMouseInput();
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        bool leftClicked = (mouse & MOUSE_INPUT_LEFT) && !(oldMouse & MOUSE_INPUT_LEFT);
        oldMouse = mouse;

        //今の画面の処理を呼び出し、次に移動したい画面のIDを受け取る
        SceneID nextSceneID = currentScene->UpdateAndDraw(player, leftClicked, mouseX, mouseY, timer);

        // 別の画面に切り替える
        if (nextSceneID != SceneID::None) {

            // BGMの切り替え制御
            // ミニゲームへ移動する場合：通常BGMを止めてミニゲームBGMを開始
            if (nextSceneID == SceneID::MiniGame) {
                StopSoundMem(player.mainBGM);
            }
            //ミニゲームから「それ以外の画面」に戻る場合：ミニゲームBGMを止めて通常BGMを再開
            else if (currentSceneID == SceneID::MiniGame && nextSceneID != SceneID::MiniGame) {
                PlaySoundMem(player.mainBGM, DX_PLAYTYPE_LOOP);
            }
            delete currentScene;

            // 指示された新しい画面を作り直して差し替える
            if (nextSceneID == SceneID::Home) {
                currentScene = new HomeScene();
            }
            else if (nextSceneID == SceneID::MiniGame) {
                currentScene = new MiniGameScene();
            }
            else if (nextSceneID == SceneID::Timer) {
                currentScene = new TimerScene();
            }
            else if (nextSceneID == SceneID::ActiveRecall) {
                currentScene = new ActiveRecall();
            }
            else if (nextSceneID == SceneID::scar) {
                currentScene = new scar();
            }
            else if (nextSceneID == SceneID::Shop) {
                currentScene = new ShopScene();
            }
            
            //現在のシーンIDを更新
            currentSceneID = nextSceneID;
        }

        ScreenFlip();
        WaitTimer(33);
        if (ProcessMessage() == -1) break;
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
    }
    //プレイヤーの情報を保存する処理
    player.Save();
    //画面のメモリを解放
    delete currentScene;

    DxLib_End();
    return 0;
}