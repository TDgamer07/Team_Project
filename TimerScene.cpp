#include "DxLib.h"
#include "TimerScene.h"
#include <cmath>
#include <ctime>

extern void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1);

//デジタル時計風の数字（7セグメント）を描画する関数
void DrawDigitalDigit(int x, int y, int size, int num) {
    if (num < 0 || num > 9) return;

    int w = size;         // 横幅
    int h = size * 2;     // 高さ
    int t = size / 4;     // 線の太さ
    if (t < 2) t = 2;

    // 0?9の数字を構成する7つのパーツのON/OFF（1なら点灯、0なら消灯）
    int segs[10][7] = {
        {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1},
        {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
    };

    int baseColor = GetColor(255, 255, 255); // 基本はデジタルなシアン色

    for (int i = 0; i < 7; i++) {
        if (segs[num][i]) {
            int drawColor = baseColor;
            int alpha = 255;

            // グリッチエフェクト：たまに一部のパーツが赤くバグるか、消えかかる
            if (GetRand(100) > 95) drawColor = GetColor(255, 255, 255); // 呪いの赤
            else if (GetRand(100) > 95) alpha = 50; // 接触不良

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            // パーツの場所に応じて四角形を描画
            switch (i) {
            case 0: DrawBox(x, y, x + w, y + t, drawColor, TRUE); break;                                 // 上
            case 1: DrawBox(x + w - t, y, x + w, y + h / 2, drawColor, TRUE); break;                     // 右上
            case 2: DrawBox(x + w - t, y + h / 2, x + w, y + h, drawColor, TRUE); break;                 // 右下
            case 3: DrawBox(x, y + h - t, x + w, y + h, drawColor, TRUE); break;                         // 下
            case 4: DrawBox(x, y + h / 2, x + t, y + h, drawColor, TRUE); break;                         // 左下
            case 5: DrawBox(x, y, x + t, y + h / 2, drawColor, TRUE); break;                             // 左上
            case 6: DrawBox(x, y + h / 2 - t / 2, x + w, y + h / 2 + t / 2, drawColor, TRUE); break;     // 真ん中
            }
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}

//システム時刻の記録と、画像の読み込み
TimerScene::TimerScene() {
    TimerCount = GetNowCount();
    imageHandle = LoadGraph("タスク\\タイマー背景.png");
}

//画面が切り替わる時（破棄される時）に画像をメモリから消去する
TimerScene::~TimerScene() {
    DeleteGraph(imageHandle);
}

SceneID TimerScene::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    //現在のシステム時刻から開始時の時刻を引き算して、画面に来てからの経過時間を計算する
    int elapsedMs = GetNowCount() - TimerCount;

    int totalSeconds = elapsedMs / 1000; // ミリ秒から秒へ変換
    int hours = totalSeconds / 3600;     // 時間
    int minutes = totalSeconds / 60;     // 分
    int seconds = totalSeconds % 60;     // 秒

    const int MAX_SECONDS = 24 * 60 * 60;
    if (totalSeconds > MAX_SECONDS) {
        totalSeconds = MAX_SECONDS; // 24時間でストップ
    }

    //既存の背景塗りつぶし処理
    DrawBox(0, 0, 1280, 720, GetColor(10, 0, 15), TRUE);

    DrawExtendGraph(0, 0, 1240, 700, imageHandle, TRUE);

    //画面タイトルの描画
    char buf[64];
    static int TitleFont = -1;
    if (TitleFont == -1) {
        TitleFont = CreateFontToHandle("メイリオ", 35, 3, DX_FONTTYPE_ANTIALIASING);
    }
    static int SubFont = -1;
    if (SubFont == -1) {
        SubFont = CreateFontToHandle("メイリオ", 20, 3, DX_FONTTYPE_ANTIALIASING);
    }
    sprintf_s(buf, "%s", "[作業モード]");
    DrawGlitchText(50, 40, buf, TitleFont);
    sprintf_s(buf, "%s", "時間を計測しています \n作業に集中しましょう");
    DrawGlitchText(50, 80, buf, SubFont);
   
    // hours, minutes, seconds をそれぞれ1桁ずつに分解します
    int h1 = hours / 10;   // 時間の十の位
    int h2 = hours % 10;   // 時間の一の位
    int m1 = minutes / 10; // 分の十の位
    int m2 = minutes % 10; // 分の一の位
    int s1 = seconds / 10; // 秒の十の位
    int s2 = seconds % 10; // 秒の一の位

    // 全体的に横長になるので、開始位置(timerX)を少し左の350にずらします
    int timerX = 350;
    int timerY = 550;   // デジタルタイマーのY座標（目の下に配置）
    int digitSize = 30; // 数字の大きさ

    //時間の描画
    DrawDigitalDigit(timerX, timerY, digitSize, h1);
    DrawDigitalDigit(timerX + 80, timerY, digitSize, h2);

    // 1つ目のコロン「：」を点滅させる
    if (GetNowCount() % 1000 < 500) {
        int colonColor = (GetRand(100) > 90) ? GetColor(0, 0, 0) : GetColor(255, 255, 255);
        DrawBox(timerX + 160, timerY + 20, timerX + 175, timerY + 35, colonColor, TRUE);
        DrawBox(timerX + 160, timerY + 60, timerX + 175, timerY + 75, colonColor, TRUE);
    }

    //分の描画
    DrawDigitalDigit(timerX + 210, timerY, digitSize, m1);
    DrawDigitalDigit(timerX + 290, timerY, digitSize, m2);

    // 2つ目のコロン「：」を点滅させる
    if (GetNowCount() % 1000 < 500) {
        int colonColor = (GetRand(100) > 90) ? GetColor(0, 0, 0) : GetColor(255, 255, 255);
        DrawBox(timerX + 370, timerY + 20, timerX + 385, timerY + 35, colonColor, TRUE);
        DrawBox(timerX + 370, timerY + 60, timerX + 385, timerY + 75, colonColor, TRUE);
    }
    //秒の描画
    DrawDigitalDigit(timerX + 420, timerY, digitSize, s1);
    DrawDigitalDigit(timerX + 500, timerY, digitSize, s2);

    //？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
    if (leftClicked && IsHit(mouseX, mouseY, 1190, 640, 1240, 690)) return SceneID::scar;

    //ホームに戻るボタン
    DrawExtendGraph(15, 590, 240, 660, player.HomeHandle, TRUE);
    if (leftClicked && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
        return SceneID::Home;
    }

    return SceneID::None;
};