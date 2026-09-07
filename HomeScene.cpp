#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "HomeScene.h"
#include <ctime>
#include <cmath>
#include <cstring>

extern void DrawGlitchFrame(int x1, int y1, int x2, int y2, int bgColor = GetColor(15, 15, 18), int thickness = 3, int bgAlpha = 128);

// 枠の限界が来たら自動で改行する関数
std::string LineBreak(const std::string& text, int maxCharsPerLine) {
    std::string result = "";
    result.reserve(text.length() + 20); // メモリの再割り当てを防ぐために事前確保

    int charCount = 0;
    for (size_t i = 0; i < text.length(); ) {
        unsigned char c = text[i];
        int byteSize = 1;

        if ((c >= 0xC2 && c <= 0xDF) || (c >= 0xE0 && c <= 0xEF) || (c >= 0xF0 && c <= 0xF4)) {
            if ((c & 0xE0) == 0xC0) byteSize = 2;
            else if ((c & 0xF0) == 0xE0) byteSize = 3;
            else if ((c & 0xF8) == 0xF0) byteSize = 4;
        }
        else if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) {
            byteSize = 2;
        }
        if (i + byteSize > text.length()) byteSize = text.length() - i;

        // substr()を使わず、元文字列から直接指定バイト数だけ追加する
        result.append(text, i, byteSize);
        i += byteSize;
        charCount++;

        if (charCount >= maxCharsPerLine && i < text.length()) {
            result += '\n'; // "\n"という文字列ではなく、'\n'という文字単体を追加
            charCount = 0;
        }
    }
    return result;
}

// 指定した文字数で文字列を強制的にカットする関数
std::string ClipToMaxCharacters(const std::string& text, int maxChars) {
    std::string result = "";
    result.reserve(text.length()); // 事前確保

    int charCount = 0;
    for (size_t i = 0; i < text.length(); ) {
        unsigned char c = text[i];
        int byteSize = 1;

        if ((c >= 0xC2 && c <= 0xDF) || (c >= 0xE0 && c <= 0xEF) || (c >= 0xF0 && c <= 0xF4)) {
            if ((c & 0xE0) == 0xC0) byteSize = 2;
            else if ((c & 0xF0) == 0xE0) byteSize = 3;
            else if ((c & 0xF8) == 0xF0) byteSize = 4;
        }
        else if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) {
            byteSize = 2;
        }

        if (i + byteSize > text.length()) byteSize = text.length() - i;

        if (charCount >= maxChars) break;

        result.append(text, i, byteSize);
        i += byteSize;
        charCount++;
    }
    return result;
}

//全角も半角も1文字としてカウントする関数
int CountCharacters(const std::string& text) {
    int charCount = 0;
    for (size_t i = 0; i < text.length(); ) {
        unsigned char c = text[i];
        int byteSize = 1; // 基本は半角1バイト

        // 文字コード(Shift-JISやUTF-8)を自動判定
        if ((c >= 0xC2 && c <= 0xDF) || (c >= 0xE0 && c <= 0xEF) || (c >= 0xF0 && c <= 0xF4)) {
            if ((c & 0xE0) == 0xC0) byteSize = 2;
            else if ((c & 0xF0) == 0xE0) byteSize = 3;
            else if ((c & 0xF8) == 0xF0) byteSize = 4;
        }
        else if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) {
            byteSize = 2; // Shift-JISの全角
        }

        if (i + byteSize > text.length()) byteSize = text.length() - i;

        i += byteSize;
        charCount++; // どんな文字でも1文字としてカウント！
    }
    return charCount;
}

//文字列をグリッチ風に描画する関数
void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1) {
    //ランダムなチラつき（透明度）
    int flicker = GetRand(100);
    int alpha = (flicker > 95) ? 80 : 255; // 5%の確率で薄くなる
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    //RGB色ズレの幅（普段は1px、たまに大きくズレる）
    int shiftX = (GetRand(100) > 90) ? GetRand(4) + 2 : 1;
    int shiftY = (GetRand(100) > 95) ? GetRand(3) - 1 : 0;

    // 赤ピンクと水色でズレを描画（フォント指定の有無で分岐）
    if (fontHandle == -1) {
        DrawString(x - shiftX, y - shiftY, text, GetColor(255, 0, 100));
        DrawString(x + shiftX, y + shiftY, text, GetColor(0, 200, 255));
        DrawString(x, y, text, GetColor(255, 255, 255));
    }
    else {
        DrawStringToHandle(x - shiftX, y - shiftY, text, GetColor(255, 0, 100), fontHandle);
        DrawStringToHandle(x + shiftX, y + shiftY, text, GetColor(0, 200, 255), fontHandle);
        DrawStringToHandle(x, y, text, GetColor(255, 255, 255), fontHandle);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //文字を横に切り裂くブロックノイズ（画面バグの表現）
    if (GetRand(100) > 85) { // 15%の確率でノイズ発生
        // フォントサイズに合わせてノイズの横幅を正確に取得
        int textWidth = (fontHandle == -1) ?
            GetDrawStringWidth(text, (int)strlen(text)) :
            GetDrawStringWidthToHandle(text, (int)strlen(text), fontHandle);

        int lineY = y + GetRand(16);      // 文字の高さに合わせてランダムな位置
        int noiseHeight = GetRand(2) + 1; // 欠ける線の太さ

        // 背景色に近い黒で上書きして、文字が「欠けている」ように見せる
        DrawBox(x - 5, lineY, x + textWidth + 5, lineY + noiseHeight, GetColor(5, 5, 8), TRUE);

        // たまに白いノイズ線も走らせる
        if (GetRand(100) > 50) {
            DrawLine(x - 10, lineY, x + textWidth + 10, lineY, GetColor(200, 200, 200), 1);
        }
    }
}

// ホラー風の鏡フレームを描画する関数
void DrawHorrorMirrorFrame(int cx, int cy, int radX, int radY) {
    // 1. 鏡の外枠（古びた不気味な木枠・金属枠）
    DrawOval(cx, cy, radX + 15, radY + 15, GetColor(20, 15, 20), TRUE);  // 土台（黒ずんだ枠）
    DrawOval(cx, cy, radX + 10, radY + 10, GetColor(80, 20, 20), FALSE); // 錆びた赤金の縁線(厚み1)
    DrawOval(cx, cy, radX + 5, radY + 5, GetColor(40, 40, 50), FALSE); // 内枠

    // 2. 鏡面の濁り・くすみ（半透明でうっすら暗くする）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
    DrawOval(cx, cy, radX, radY, GetColor(10, 15, 25), TRUE);

    // 3. 鏡の斜め反射ライン（古めかしいガラスのツヤ表現）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
    DrawLine(cx - radX + 20, cy - radY + 30, cx + radX - 40, cy + radY - 10, GetColor(255, 255, 255), 3);

    // 4. 鏡のひび割れ（幾何学的に亀裂を入れる）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    int crackColor = GetColor(180, 190, 200); // ガラスの割れ目色

    // 中心から放射状に伸びる亀裂
    DrawLine(cx - 10, cy - 20, cx - radX + 20, cy - radY + 40, crackColor, 1);
    DrawLine(cx - radX + 20, cy - radY + 40, cx - radX + 10, cy - radY + 80, crackColor, 1);

    DrawLine(cx + 10, cy + 10, cx + radX - 30, cy + radY - 20, crackColor, 1);
    DrawLine(cx + radX - 30, cy + radY - 20, cx + radX - 10, cy + radY - 50, crackColor, 1);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//ランダムな会話テキストを取得する関数
const char* GetRandomMessage() {
    static const char* messages[] = {
        "「この部屋で達成したタスクなどは組織のエネルギーとなるそうです」",
        "「今日も私に会いに来てくれたんですか？、取り敢えずコーヒーを淹れてきますね」",
        "「……。なんでもないです」",
        "「ホーム画面にある魔方陣は第1の魔法[太陽と月が重なる時、星々の力で再び生命が蘇る\n   だろう]と言う意味らしいですね...この世界に魔法があると思いますか？」",
        "「アクティブリコールにある魔方陣は第3の魔法[6つの概念の集合体は貴方を見つめてい\n   る、そして記憶の貯蔵庫へと誘うだろう]と言う意味があります。貴方が知識を定着さ\n   せると得する何かでもいるんですかね..?」",
        "「そのミニゲームは協力組織の一員から貰ったのです。その組織の地域ではこういった怪\n   異討伐を専門とした訓練としてこのゲームが開発されたそうです。気分転換にプレイす\n   るのはいかがでしょう？」",
        "「この端末、中々便利でしょう？殆どの機能が備わっているんですから....」",
        "「実はこの端末には隠し要素があるみたい。私には気づけませんでしたが、貴方なら...?」",
    };
    int num = sizeof(messages) / sizeof(messages[0]);
    return messages[rand() % num];
}

//シンプルなウィンドウの描画
bool UpdateAndDrawSimpleWindow(int x, int y, int w, int h, const char* title, bool leftClicked, int mouseX, int mouseY) {
    // カラー設定
    int bgColor = GetColor(0, 0, 0);       // ウィンドウ背景
    int frameColor = GetColor(50, 50, 50);       // 外枠とタイトルバー
    int textColor = GetColor(255, 255, 255);     // タイトルの文字色

    //ウィンドウ本体の背景と外枠
    DrawBox(x, y, x + w, y + h, bgColor, TRUE);
    DrawBox(x, y, x + w, y + h, frameColor, FALSE); // FALSEで枠線のみ

    //タイトルバー
    int titleH = 24; // タイトルバーの高さ
    DrawBox(x, y, x + w, y + titleH, frameColor, TRUE);
    DrawString(x + 10, y + 4, title, textColor);

    //閉じるボタン [X]
    int btnW = 20, btnH = 20;
    int btnX = x + w - btnW - 2;
    int btnY = y + 2;

    //マウスがボタンの上にあるか判定
    bool isCloseHovered = (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= btnY && mouseY <= btnY + btnH);

    //マウスが乗っている時は赤くする
    int btnColor = isCloseHovered ? GetColor(220, 50, 50) : GetColor(100, 100, 100);
    DrawBox(btnX, btnY, btnX + btnW, btnY + btnH, btnColor, TRUE);
    DrawString(btnX + 6, btnY + 2, "X", textColor);

    //閉じるボタンがクリックされたら true を返す
    if (isCloseHovered && leftClicked) {
        return true;
    }
    return false;
}

//アイコンのクリック処理と会話の中身の描画
// 毎フレーム UpdateAndDraw の中で呼び出します
void HandleIconAndConversation(int iconX, int iconY, int iconW, int iconH, bool leftClicked, int mouseX, int mouseY, int charaGraphHandle) {
    // 状態を保持する変数
    static bool isWindowOpen = false;
    static const char* currentText = "";

    static int LogFont = -1;
    if (LogFont == -1) {
        // サイズ25、太さ3、アンチエイリアスありで綺麗なフォントを作成
        LogFont = CreateFontToHandle("メイリオ", 25, 3, DX_FONTTYPE_ANTIALIASING);
    }

    //アイコンのクリック判定
    bool isIconHovered = (mouseX >= iconX && mouseX <= iconX + iconW && mouseY >= iconY && mouseY <= iconY + iconH);

    // アイコンをクリックしたら、新しいランダムテキストを取得してウィンドウを開く
    if (isIconHovered && leftClicked) {
        isWindowOpen = true;
        currentText = GetRandomMessage();
    }

    //ウィンドウが開いている場合の処理 
    if (isWindowOpen) {
        int winX = 80, winY = 180;
        int winW = 1090, winH = 300;
       
        if (UpdateAndDrawSimpleWindow(winX, winY, winW, winH, "黒目さん", leftClicked, mouseX, mouseY)) {
            isWindowOpen = false;
        }

        // ウィンドウが閉じられていなければ「中身（キャラ・テキスト）」を描画する
        if (isWindowOpen) {
            // ウィンドウの中の描画開始座標（タイトルバーの下から開始）
            int contentX = winX + 10;
            int contentY = winY + 34;

            //キャラクター画像の描画
            if (charaGraphHandle != -1) {
                DrawExtendGraph(contentX, contentY, contentX + 80, contentY + 80, charaGraphHandle, TRUE);
            }

            //会話テキストの描画（キャラ画像の右側に表示）
            DrawGlitchText(contentX + 100, contentY + 15, currentText, LogFont);
        }
    }
}

SceneID HomeScene::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    const int WHITE = GetColor(255, 255, 255);
    const int BLUE = GetColor(0, 0, 255);
    const int GRAY = GetColor(80, 80, 80);
    const int PURPLE = GetColor(128, 0, 128);

    int darkBgColor = GetColor(15, 15, 18);
    int thickness = 5;
    int bgAlpha = 200;

    //大きな数字用のフォントを生成
    static int largeFont = -1;
    if (largeFont == -1) {
        // サイズ25、太さ3、アンチエイリアスありで綺麗なフォントを作成
        largeFont = CreateFontToHandle("メイリオ", 25, 3, DX_FONTTYPE_ANTIALIASING);
    }
    static int timeFont = -1;
    if (timeFont == -1) {
        timeFont = CreateFontToHandle("メイリオ", 35, 3, DX_FONTTYPE_ANTIALIASING);
    }
   
    //アイコン画像の読み込みを1つだけに簡略化
    static int userIcon = -1;
    static bool isIconLoaded = false;
    if (!isIconLoaded) {
        userIcon = LoadGraph("タスク\\アイコン異形頭.png");
        isIconLoaded = true;
    }

    //背景
    DrawExtendGraph(100, 30, 1150, 680, Magic1Handle, TRUE);

    //通常モード時のUI描画＆クリック判定
    if (!isInputMode) {
        // 枠の基本座標
        int fX1 = 30, fY1 = 30, fX2 = 200, fY2 = 200;

        //単一のアイコン画像を描画
        if (userIcon != -1) {
            DrawExtendGraph(fX1 + 5, fY1 + 5, fX2 - 5, fY2 - 5, userIcon, TRUE);
        }
        
        //ホラー風の鏡フレーム（アンティーク調）を上から重ねる
        // 黒ずんだ外枠（5ピクセルの厚みを持たせて角を隠す）
        for (int i = 0; i < 5; i++) {
            DrawBox(fX1 + i, fY1 + i, fX2 - i, fY2 - i, GetColor(25, 20, 20), FALSE);
        }
        // 錆びた赤金色の内枠線
        DrawBox(fX1 + 5, fY1 + 5, fX2 - 5, fY2 - 5, GetColor(100, 30, 30), FALSE);

        //鏡のガラス表現（汚れ、反射、ヒビ割れを画像の上から重ねる）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
        // 鏡の斜め反射ライン（ガラスっぽさを出す）
        DrawLine(fX1 + 20, fY2 - 30, fX2 - 40, fY1 + 10, GetColor(255, 255, 255), 4);
        DrawLine(fX1 + 40, fY2 - 10, fX2 - 20, fY1 + 30, GetColor(255, 255, 255), 2);

        //鏡のヒビ割れ（不吉な亀裂）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        int crackColor = GetColor(180, 190, 200);

        // 左上のヒビ
        DrawLine(fX1 + 5, fY1 + 20, fX1 + 30, fY1 + 50, crackColor, 1);
        DrawLine(fX1 + 30, fY1 + 50, fX1 + 25, fY1 + 90, crackColor, 1);
        DrawLine(fX1 + 30, fY1 + 50, fX1 + 60, fY1 + 65, crackColor, 1);

        // 右下のヒビ
        DrawLine(fX2 - 5, fY2 - 15, fX2 - 40, fY2 - 45, crackColor, 1);
        DrawLine(fX2 - 40, fY2 - 45, fX2 - 70, fY2 - 40, crackColor, 1);
        DrawLine(fX2 - 40, fY2 - 45, fX2 - 30, fY2 - 80, crackColor, 1);

        // 描画モードを元に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        //現在の時刻を取得
        time_t t = time(nullptr);
        struct tm* localTime = localtime(&t);

        //時計表示をホラー＆グリッチ風に変更 
        int darkBlood = GetColor(30, 0, 5);

        // ベースの背景枠（暗黒の赤紫）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(730, 30, 1220, 130, darkBlood, TRUE);

        // 下部の発光ライン（血のような赤）
        SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
        DrawGlitchFrame(725, 25, 1225, 130, darkBgColor, thickness, bgAlpha);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 時刻の文字列を作成
        char timeStr[64];
        sprintf_s(timeStr, "%04d / %02d / %02d   %02d:%02d:%02d",
            localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

        //グリッチ描画（timeFontを指定して大きく描画）
        DrawGlitchText(750, 60, timeStr, timeFont);

        //経験値・レベル・コイン・クエスト達成数の描画 
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        // グリッチ文字（白）が目立つように、背景枠を元のGRAYから暗い色へ変更
        DrawGlitchFrame(225, 25, 700, 150, darkBgColor, thickness, bgAlpha);

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        char buf[64]; 
        //レベル
        DrawExtendGraph(245, 30, 320, 94, levelIconHandle, TRUE);
        sprintf_s(buf, "%d", player.level);
        DrawGlitchText(320, 45, buf, largeFont);
        //コイン
        DrawExtendGraph(430, 25, 530, 94, coinIconHandle, TRUE);
        DrawExtendGraph(640, 25, 710, 100, CoinMarkHandle, TRUE);
        sprintf_s(buf, " %d", player.coins);
        DrawGlitchText(515, 45, buf, largeFont);
        //クリア数
        DrawExtendGraph(435, 85, 523, 140, ClearIconHandle, TRUE);
        sprintf_s(buf, "%d", player.completedCount);
        DrawGlitchText(525, 110, buf, largeFont);
        //経験値
        DrawExtendGraph(200, 65, 340, 195, expIconHandle, TRUE);
        sprintf_s(buf, "%d", player.MAXxp);
        DrawGlitchText(320, 110, buf, largeFont);

        //ショップボタン
        DrawExtendGraph(25, 220, 200, 305, ShopHandle, TRUE);
        if (leftClicked && IsHit(mouseX, mouseY, 25, 220, 195, 305)) return SceneID::Shop;
        //アクティブリコールボタン
        DrawExtendGraph(30, 329, 200, 405, ActiveRecallHandle, TRUE);
        if (leftClicked && IsHit(mouseX, mouseY, 30, 329, 190, 405)) return SceneID::ActiveRecall;
        //ミニゲームボタン
        if (player.weapons[0].isOwned) {
            DrawExtendGraph(20, 410, 205, 500, MinigameHandle, TRUE);
        }
        if (leftClicked && IsHit(mouseX, mouseY, 20, 410, 195, 500)) {
            if (player.weapons[0].isOwned) {
                return SceneID::MiniGame;
            }
        }
        //タイマーボタン
        DrawExtendGraph(5, 520, 230, 670, TimerHandle, TRUE);
        if (leftClicked && IsHit(mouseX, mouseY, 35, 520, 180, 670)) return SceneID::Timer;

        //デイリークエストエリア（改行＆枠の自動拡大対応）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawBox(230, 200, 700, 660, GRAY, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        DrawExtendGraph(20, 150, 890, 700, DailyFrameHandle, TRUE);
        DrawExtendGraph(210, 180, 260, 230, RedStarHandle, TRUE);
        DrawExtendGraph(670, 180, 720, 230, RedStarHandle, TRUE);
        DrawExtendGraph(210, 630, 260, 680, RedStarHandle, TRUE);
        DrawExtendGraph(670, 630, 720, 680, RedStarHandle, TRUE);

        sprintf_s(buf, "%s", "デイリークエスト");
        DrawGlitchText(360, 165, buf, largeFont);

        //デイリー追加ボタン
        DrawExtendGraph(660, 155, 720, 190, DailyHandle, TRUE);

        //タスクが5個以上の場合は警告メッセージを表示し、ボタンを暗くする
        if (player.dailyQuests.size() >= 5) {
            DrawGlitchText(330, 205, "※上限に達しているため追加できません");
        }

        //アイコン選択中でなく、かつタスクが5個未満の時だけ反応する
        if (!isIconSelectMode && leftClicked && IsHit(mouseX, mouseY, 670, 155, 700, 190)) {
            if (player.dailyQuests.size() < 5) {
                isInputMode = true; inputType = QuestType::Daily;

                inputHandle = MakeKeyInput(60, FALSE, FALSE, FALSE, FALSE, FALSE);
                SetActiveKeyInput(inputHandle);
                leftClicked = false; // クリック消費
            }
        }

        int currentDailyY = 230; // タスクを描画するY座標（可変）
        for (size_t i = 0; i < player.dailyQuests.size(); i++) {
            //タスク名と報酬文字を合体させてから改行させる
            std::string fullText = player.dailyQuests[i].title + " (XP:+10)";
            std::string wrappedText = LineBreak(fullText, 21); // 約21文字で改行

            //改行(\n)の数を数えて、枠の高さを決める
            int lines = 1;
            for (char c : wrappedText) {
                if (c == '\n') lines++;
            }
            int boxHeight = 15 + lines * 20; // 1行なら35px、2行なら55px…と広がる

            int ty1 = currentDailyY;
            int ty2 = ty1 + boxHeight;

            // タスク名（左側）
            DrawBox(250, ty1, 630, ty2, PURPLE, FALSE);
            DrawGlitchText(260, ty1 + 10, wrappedText.c_str());

            // 削除（×）ボタン（右側）
            int btnY1 = ty1 + (boxHeight - 35) / 2;
            int btnY2 = btnY1 + 35;

            //削除ボタン
            DrawExtendGraph(635, btnY1, 685, btnY2, DeleteHandle, TRUE);

            // クリック判定
            if (leftClicked) {
                if (IsHit(mouseX, mouseY, 640, btnY1, 680, btnY2)) {
                    player.dailyQuests.erase(player.dailyQuests.begin() + i);
                    player.Save(); break;
                }
                else if (IsHit(mouseX, mouseY, 250, ty1, 630, ty2)) {
                    player.AddReward(10, 10);
                    player.completedCount++; //[cite: 1]
                    player.dailyQuests.erase(player.dailyQuests.begin() + i);
                    player.Save(); break;
                }
            }
            currentDailyY += boxHeight + 10; // 次のタスク用にY座標を下へずらす
        }

        //ウィークリークエストエリア（改行＆枠の自動拡大対応）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawBox(730, 200, 1200, 660, GRAY, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        sprintf_s(buf, "%s", "ウィークリークエスト");
        DrawGlitchText(855, 165, buf, largeFont);

        DrawExtendGraph(480, 190, 1220, 665, WeeklyFrameHandle, TRUE);
        DrawExtendGraph(710, 180, 760, 230, BlueStarHandle, TRUE);
        DrawExtendGraph(1168, 180, 1218, 230, BlueStarHandle, TRUE);
        DrawExtendGraph(710, 630, 760, 680, BlueStarHandle, TRUE);
        DrawExtendGraph(1168, 630, 1218, 680, BlueStarHandle, TRUE);

        //ウィークリークエスト追加ボタン 
        DrawExtendGraph(1160, 153, 1200, 190, WeeklyHandle, TRUE);

        //タスクが5個以上の場合は警告メッセージを表示し、ボタンを暗くする
        if (player.weeklyQuests.size() >= 5) {
            DrawGlitchText(810, 205, "※上限に達しているため追加できません");
        }

        // アイコン選択中でなく、かつタスクが5個未満の時だけ反応する
        if (!isIconSelectMode && leftClicked && IsHit(mouseX, mouseY, 1160, 153, 1190, 190)) {
            if (player.weeklyQuests.size() < 5) {
                isInputMode = true; inputType = QuestType::Weekly;

                inputHandle = MakeKeyInput(60, FALSE, FALSE, FALSE, FALSE, FALSE);
                SetActiveKeyInput(inputHandle);
                leftClicked = false; // クリック消費
            }
        }

        int currentWeeklyY = 230; // タスクを描画するY座標（可変）
        for (size_t i = 0; i < player.weeklyQuests.size(); i++) {
            //タスク名と報酬文字を合体させてから改行させる[cite: 1]
            std::string fullText = player.weeklyQuests[i].title + " (XP:+20)";
            std::string wrappedText = LineBreak(fullText, 21);

            //枠の高さ計算
            int lines = 1;
            for (char c : wrappedText) {
                if (c == '\n') lines++;
            }
            int boxHeight = 15 + lines * 20;

            int ty1 = currentWeeklyY;
            int ty2 = ty1 + boxHeight;

            // タスク描画
            DrawBox(750, ty1, 1130, ty2, BLUE, FALSE);
            DrawGlitchText(760, ty1 + 10, wrappedText.c_str());

            // 削除ボタン
            int btnY1 = ty1 + (boxHeight - 35) / 2;
            int btnY2 = btnY1 + 35;

            DrawExtendGraph(1130, btnY1, 1180, btnY2, DeleteHandle, TRUE);

            // クリック判定
            if (leftClicked) {
                if (IsHit(mouseX, mouseY, 1140, btnY1, 1180, btnY2)) {
                    player.weeklyQuests.erase(player.weeklyQuests.begin() + i);
                    player.Save(); break;
                }
                else if (IsHit(mouseX, mouseY, 750, ty1, 1130, ty2)) {
                    player.AddReward(30, 50);
                    player.completedCount++; //[cite: 1]
                    player.weeklyQuests.erase(player.weeklyQuests.begin() + i);
                    player.Save(); break;
                }
            }
            currentWeeklyY += boxHeight + 10; // 次のタスク用にY座標を下へずらす
        }
        //会話の判定と描画をクエストのforループから出し、最後に配置
        HandleIconAndConversation(30, 30, 200, 200, leftClicked, mouseX, mouseY, userIcon);
    }
    //文字入力モード
    else {
        //入力ウィンドウ全体の横幅を広げる (全角30文字が余裕で収まるサイズに)
        DrawBox(200, 200, 1080, 450, GRAY, TRUE);
        DrawBox(200, 200, 1080, 450, WHITE, FALSE);

        //入力文字のグリッチ（色化けと明滅）計算
        int flicker = GetRand(100);
        int mainColor = GetColor(255, 255, 255); // 基本は白

        // 10%の確率で文字の色が赤や青にバグる
        if (flicker > 95) {
            mainColor = GetColor(255, 0, 100); // 赤ズレ
        }
        else if (flicker > 90) {
            mainColor = GetColor(0, 200, 255); // 青ズレ
        }

        // 5%の確率で画面全体が薄くチラつく（接触不良の表現）
        if (GetRand(100) > 95) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
        }

        //入力文字・変換文字の色を設定する処理
        SetKeyInputStringColor(
            mainColor,               // ① 確定した文字（ここでバグらせる）
            GetColor(255, 255, 255), // ② 通常カーソル
            GetColor(150, 255, 150), // ③ ひらがな入力中の文字
            GetColor(255, 255, 0),   // ④ 入力中のカーソル
            GetColor(255, 100, 100), // ⑤ 入力中の下線
            GetColor(255, 200, 0),   // ⑥ 変換候補で選ばれている文字
            GetColor(200, 200, 200)  // ⑦ 「あ」などの入力モードテキスト
        );

        if (inputType == QuestType::Daily) DrawGlitchText(230, 230, "デイリークエストの追加");
        else DrawGlitchText(230, 230, "ウィークリークエストの追加");

        DrawGlitchText(230, 270, "タスクを入力し、Enterキーで決定してください（30文字まで）");
        DrawGlitchText(230, 400, "※ Deleteキーでキャンセル");

        //入力枠も横に広げる (左を230、右を1050に拡張)
        DrawBox(230, 320, 1050, 360, WHITE, FALSE);

        // リアルタイムで30文字制限のチェックを行い、超えた場合は強制カット
        char currentStr[100];
        GetKeyInputString(currentStr, inputHandle);
        if (CountCharacters(currentStr) > 30) {
            std::string clippedStr = ClipToMaxCharacters(currentStr, 30);
            SetKeyInputString(clippedStr.c_str(), inputHandle);
        }

        //描画エリアを制限して、変換中の文字が右に突き抜けるのを物理的に隠す
        RECT oldRect;
        GetDrawArea(&oldRect); // 元の描画範囲をRECT構造体に保存

        // 描画範囲を「入力枠の横幅内」に制限
        SetDrawArea(230, 0, 1050, 1000);

        //文字の描画開始位置も左に寄せる
        DrawKeyInputString(240, 332, inputHandle);

        // 描画可能エリアを元に戻す
        SetDrawArea(oldRect.left, oldRect.top, oldRect.right, oldRect.bottom);

        //入力枠の横スジ（ブロックノイズ）エフェクト
        // 描画した入力文字の上から、強制的に背景色（GRAY）の直線を引いて文字を削る
        if (GetRand(100) > 80) { // 20%の確率でノイズ発生
            int noiseY = 325 + GetRand(30);   // 入力枠（Y:320?360）の中でランダムな高さ
            int noiseHeight = GetRand(3) + 1; // 削る線の太さ

            // 背景と同じ色で上書きして、文字が「横に欠けている」ように見せる
            DrawBox(235, noiseY, 1045, noiseY + noiseHeight, GRAY, TRUE);

            // たまに走査線（白いノイズ線や赤いエラー線）も走らせる
            if (GetRand(100) > 50) {
                int noiseColor = (GetRand(10) > 5) ? GetColor(200, 200, 200) : GetColor(255, 0, 100);
                DrawLine(235, noiseY, 1045, noiseY, noiseColor, 1);
            }
        }

        // チラつき用のブレンド設定を通常に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        if (CheckKeyInput(inputHandle) == 1) {
            char str[100];
            GetKeyInputString(str, inputHandle);
            DeleteKeyInput(inputHandle);
            inputHandle = -1;
            isInputMode = false;

            // 確定時も念のため30文字でカット
            std::string titleStr = ClipToMaxCharacters(str, 30);
            if (!titleStr.empty()) {
                if (inputType == QuestType::Daily) {
                    player.dailyQuests.push_back(Quest(titleStr, QuestType::Daily, 10, 5));
                }
                else {
                    player.weeklyQuests.push_back(Quest(titleStr, QuestType::Weekly, 50, 30));
                }
                player.Save();
            }
        }
        else if (CheckHitKey(KEY_INPUT_DELETE) == 1) {
            DeleteKeyInput(inputHandle);
            inputHandle = -1;
            isInputMode = false;
            while (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { ProcessMessage(); }
        }
    }
    return SceneID::None;
}