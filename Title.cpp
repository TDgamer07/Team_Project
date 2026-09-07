#include "DxLib.h"
#include "Title.h"
#include <cmath>

extern void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1);

SceneID Title::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    
    ChangeVolumeSoundMem(255, walkBGM);
    ChangeVolumeSoundMem(255, doorBGM);

    // 画像ハンドルの読み込み
    static int CorridorHandle = -1;
    if (CorridorHandle == -1) {
        CorridorHandle = LoadGraph("タスク\\起動画面.png"); // 廊下の背景画像
    }
    static int FootprintHandle = -1;
    if (FootprintHandle == -1) {
        FootprintHandle = LoadGraph("タスク\\足跡.png"); // 足跡の画像
    }

    // 演出進行用のステート（状態）管理
    enum class Mode { Title, Corridor, FadeOut };
    static Mode mode = Mode::Title;

    static int walkTimer = 0;
    static int currentStep = 0;
    // ドアの手前までの歩数
    const int MAX_STEPS = 8; 
    static float fadeAlpha = 0.0f;

    // 現在の時間（ミリ秒）を取得する
    int t = GetNowCount();
    //タイトル画面処理
    if (mode == Mode::Title) {
        // 脈打つ暗闇の背景
        DrawBox(0, 0, 1280, 720, GetColor(5, 5, 8), TRUE);
        // 画面端を暗くするビネット効果
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, 1280, 120, GetColor(0, 0, 0), TRUE);    // 上
        DrawBox(0, 600, 1280, 720, GetColor(0, 0, 0), TRUE);  // 下
        DrawBox(0, 0, 150, 720, GetColor(0, 0, 0), TRUE);     // 左
        DrawBox(1130, 0, 1280, 720, GetColor(0, 0, 0), TRUE); // 右
        // ブラウン管の砂嵐エフェクト
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
        for (int i = 0; i < 350; i++) {
            int nx = GetRand(1280);
            int ny = GetRand(720);
            int noiseColor = (GetRand(10) > 8) ? GetColor(255, 255, 255) : GetColor(40, 40, 40);
            DrawPixel(nx, ny, noiseColor);
            DrawPixel(nx + 1, ny, noiseColor);
        }
        // 上から降り注ぐ血の雨
        static float dropX[80];
        static float dropY[80];
        static bool initDrops = false;

        if (!initDrops) {
            for (int i = 0; i < 80; i++) {
                dropX[i] = (float)GetRand(1280);
                dropY[i] = (float)GetRand(720);
            }
            initDrops = true;
        }

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
        for (int i = 0; i < 80; i++) {
            dropY[i] += 1.5f + (GetRand(15) / 10.0f);
            dropX[i] += sin(t / 1000.0f + i) * 0.5f;

            if (dropY[i] > 720) {
                dropY[i] = -10.0f;
                dropX[i] = (float)GetRand(1280);
            }
            DrawCircle((int)dropX[i], (int)dropY[i], GetRand(1) + 1, GetColor(130, 10, 10), TRUE);
        }
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // グリッチ風のタイトルロゴ
        int flicker = GetRand(100);
        int titleAlpha = (flicker > 96) ? 30 : 255;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, titleAlpha);

        SetFontSize(64);
        SetFontThickness(5);

        const char* titleText = "T A S K   R O O M";
        int titleX = 350;
        int titleY = 220;

        DrawString(titleX - 5, titleY, titleText, GetColor(200, 0, 0));
        DrawString(titleX + 5, titleY, titleText, GetColor(0, 0, 200));
        DrawString(titleX, titleY, titleText, GetColor(255, 255, 255));

        SetFontSize(16);
        SetFontThickness(1);

        // スタートボタン
        int btnAlpha = (int)(120 + 100 * sin(t / 150.0f));
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, btnAlpha);
        DrawBox(520, 480, 760, 540, GetColor(80, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        char buf[64];
        static int SubFont = -1;
        if (SubFont == -1) {
            SubFont = CreateFontToHandle("メイリオ", 20, 3, DX_FONTTYPE_ANTIALIASING);
        }
        sprintf_s(buf, "%s", "おかえりなさいませ");
        DrawGlitchText(550, 502, buf, SubFont);

        // クリックでpvにいく
        if (leftClicked && IsHit(mouseX, mouseY, 520, 480, 760, 540)) {
            StopSoundMem(player.mainBGM);
            mode = Mode::Corridor;
            walkTimer = 0;
            currentStep = 0;
            fadeAlpha = 0.0f;
        }
    }
    //廊下歩行&暗転フェードアウト処理
    else {
        //廊下背景の描画
        if (CorridorHandle != -1) {
            DrawExtendGraph(0, 0, 1280, 720, CorridorHandle, TRUE);
        }
        else {
            // 画像読み込み失敗時の仮背景
            DrawBox(0, 0, 1280, 720, GetColor(40, 40, 45), TRUE);
        }

        //歩行のタイマー更新
        if (mode == Mode::Corridor) {
            walkTimer++;
            // 25フレームごとに一歩進む
            if (walkTimer >= 25) {
                currentStep++;
                walkTimer = 0;

                //足音SEを鳴らすかもしれない
                PlaySoundMem(walkBGM, DX_PLAYTYPE_BACK);
             
                // 指定歩数に達したら暗転モードへ移行
                if (currentStep >= MAX_STEPS) {
                    mode = Mode::FadeOut;
                }
            }
        }
        //暗転処理
        else if (mode == Mode::FadeOut) {
            // 暗転のスピード
            fadeAlpha += 4.0f; 
            if (fadeAlpha >= 255.0f) {
                fadeAlpha = 255.0f;
            }
        }
     
        //累積した足跡を描画（2Dの遠近感計算）
        for (int i = 0; i < currentStep; i++) {
            // 奥（ドア）に向かう正規化割合 [0.0 ~ 1.0]
            float rate = (float)i / (MAX_STEPS - 1);

            // Y座標：手前(600)から奥(380)へ移動
            float footY = 600.0f - (rate * 220.0f);

            // スケール：手前(1.0)から奥(0.35)へ縮小
            double scale = 1.0 - (rate * 0.65);

            // 左右交互のオフセット（奥に行くほど左右の幅も狭める）
            float sideOffset = ((i % 2 == 0) ? -45.0f : 45.0f) * (float)scale;
            float footX = 640.0f + sideOffset; // 640は画面中央X座標

            // 足跡描画
            if (FootprintHandle != -1) {
                DrawRotaGraphF(footX, footY, scale, 0.0, FootprintHandle, TRUE);
            }
        }

        //暗転用オーバーレイ描画
        if (fadeAlpha > 0.0f) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
            DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        //描画が全て終わった後にシーン遷移の判定を行う
        if (mode == Mode::FadeOut && fadeAlpha >= 255.0f) {
            PlaySoundMem(doorBGM, DX_PLAYTYPE_NORMAL);
            PlaySoundMem(player.mainBGM, DX_PLAYTYPE_LOOP);
            mode = Mode::Title; 
            return SceneID::Home; 
        }
    }

    return SceneID::None;
}