#include "DxLib.h"
#include "scar.h"

extern void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1);

SceneID scar::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {

    const int WHITE = GetColor(255, 255, 255);
    //画像の読み込み
    static int imgNormal = -1;
    static int imgAnomalies[9];

    static int human = LoadGraph("隠し\\人間.png");

    if (imgNormal == -1) {
        imgNormal = LoadGraph("隠し\\Corridor_Normal.png");
        for (int i = 0; i < 9; ++i) {
            char path[256];
            sprintf_s(path, "隠し\\Anomaly%d.png", i + 1);
            imgAnomalies[i] = LoadGraph(path);
        }
    }

    //状態管理
    static int currentFloor = 1;
    static bool isGenerated = false;
    static bool hasAnomaly = false;
    static int currentAnomalyIndex = -1;

    //プレイヤーの座標とサイズ
    static float playerX = 400.0f; // 画面中央
    static float playerY = 400.0f;
    const float SPEED = 6.0f;
    
    static int largeFont = -1;
    if (largeFont == -1) {
        largeFont = CreateFontToHandle("メイリオ", 40, 3, DX_FONTTYPE_ANTIALIASING);
    }

    //部屋の生成
    if (!isGenerated) {
        if (currentFloor == 9) {
            currentFloor = 1;
            isGenerated = false;
            return SceneID::Home;
        }

        if (currentFloor != 1) {
            hasAnomaly = (GetRand(100) < 50);

            if (hasAnomaly) {
                currentAnomalyIndex = GetRand(8);
            }
        }
        playerX = 400.0f;

        isGenerated = true;
    }

    //WASDキー操作
    if (CheckHitKey(KEY_INPUT_A) == 1) {
        playerX -= SPEED;
    }
    if (CheckHitKey(KEY_INPUT_D) == 1) {
        playerX += SPEED;
    }

    //描画処理
    if (hasAnomaly) {
        DrawExtendGraph(0, 0, 1230, 720, imgAnomalies[currentAnomalyIndex], TRUE);
    }
    else {
        DrawExtendGraph(0, 0, 1230, 720, imgNormal, TRUE);
    }

    // プレイヤーキャラクターの仮描画
    DrawExtendGraph((int)playerX, (int)playerY, (int)playerX + 150, (int)playerY + 150, human, TRUE);

    char buf[64];
    sprintf_s(buf, "%d", currentFloor);
    DrawGlitchText(10, 50, buf, largeFont);
    DrawGlitchText(10, 90, "第1の部屋は正常です。8回連続でクリアすれば脱出できます");
    static int TitleFont = -1;
    if (TitleFont == -1) {
        TitleFont = CreateFontToHandle("メイリオ", 35, 3, DX_FONTTYPE_ANTIALIASING);
    }
    DrawGlitchText(50, 20, "[Energy Change Dream製　異変悪夢]", TitleFont);

    // ドアとの当たり判定
    //進むドア（右）に入ったか
    if (playerX > 1000)
    {
        if (!hasAnomaly) {
            currentFloor++; // 正解
        }
        else {
            currentFloor = 1; // 失敗
        }
        isGenerated = false;
    }
    //戻るドア（左）に入ったか
    else if (playerX < 100)
    {
        if (hasAnomaly) {
           currentFloor++; // 正解
        }
        else {
           currentFloor = 1; // 失敗
        }
        isGenerated = false;
    }

    //ホームボタン
    DrawExtendGraph(15, 590, 240, 660, player.HomeHandle, TRUE);
    if (leftClicked && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
        currentFloor = 1;
        isGenerated = false;
        return SceneID::Home;
    }
    return SceneID::None;
}