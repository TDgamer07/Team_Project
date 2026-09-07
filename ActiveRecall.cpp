#include "DxLib.h"
#include "ActiveRecall.h"

//HomeScene.cpp で定義した関数を再利用
extern int CountCharacters(const std::string& text);
extern std::string ClipToMaxCharacters(const std::string& text, int maxChars);
extern void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1);

void DrawGlitchFrame(int x1, int y1, int x2, int y2, int bgColor = GetColor(15, 15, 18), int thickness = 3, int bgAlpha = 128) {
    //中身（背景）を灰色の半透明で塗りつぶし
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, bgAlpha);
    DrawBox(x1, y1, x2, y2, GetColor(60, 60, 60), TRUE); // 灰色の半透明（RGB: 60, 60, 60）
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //ランダムなチラつき
    int flicker = GetRand(100);
    int alpha = (flicker > 90) ? 180 : 255;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    //RGB色ズレの幅
    int shiftX = (GetRand(100) > 90) ? GetRand(3) + 1 : 1;
    int shiftY = (GetRand(100) > 90) ? GetRand(2) - 1 : 0;

    //枠線の描画（指定された太さの分だけ内側にずらしながら重ね書きする）
    for (int i = 0; i < thickness; i++) {
        int tx1 = x1 + i, ty1 = y1 + i;
        int tx2 = x2 - i, ty2 = y2 - i;

        DrawBox(tx1 - shiftX, ty1 - shiftY, tx2 - shiftX, ty2 - shiftY, GetColor(255, 0, 100), FALSE); // 赤ズレ
        DrawBox(tx1 + shiftX, ty1 + shiftY, tx2 + shiftX, ty2 + shiftY, GetColor(0, 200, 255), FALSE); // 青ズレ
        DrawBox(tx1, ty1, tx2, ty2, GetColor(255, 255, 255), FALSE); // メインの白枠
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //枠が物理的に欠けるノイズエフェクト（太さに合わせて削る範囲を広げる）
    if (GetRand(100) > 90) {
        // 上下の枠を削る
        int noiseX = x1 + GetRand(x2 - x1);
        int noiseWidth = GetRand(20) + 5;
        DrawBox(noiseX, y1 - 5, noiseX + noiseWidth, y1 + thickness + 5, bgColor, TRUE);
        DrawBox(noiseX, y2 - thickness - 5, noiseX + noiseWidth, y2 + 5, bgColor, TRUE);
    }
    if (GetRand(100) > 90) {
        // 左右の枠を削る
        int noiseY = y1 + GetRand(y2 - y1);
        int noiseHeight = GetRand(15) + 5;
        DrawBox(x1 - 5, noiseY, x1 + thickness + 5, noiseY + noiseHeight, bgColor, TRUE);
        DrawBox(x2 - thickness - 5, noiseY, x2 + 5, noiseY + noiseHeight, bgColor, TRUE);
    }
    //たまに画面外まで突き抜ける走査線（横線）を飛ばす
    if (GetRand(100) > 95) {
        int lineY = y1 + GetRand(y2 - y1);
        int lineColor = (GetRand(10) > 5) ? GetColor(200, 200, 200) : GetColor(255, 0, 100);

        // 横線も枠の太さに合わせて少し太くする
        for (int i = 0; i < (thickness / 2 + 1); i++) {
            DrawLine(x1 - 30, lineY + i, x2 + 30, lineY + i, lineColor, 1);
        }
    }
}

SceneID ActiveRecall::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    const int WHITE = GetColor(255, 255, 255);
    const int RED = GetColor(255, 100, 100);
    const int GRAY = GetColor(80, 80, 80);

    //背景
    DrawExtendGraph(250, 20, 990, 680, Magic2Handle, TRUE);

    int darkBgColor = GetColor(15, 15, 18);
    int thickness = 5;
    int bgAlpha = 128;

    char buf[64];
    static int TitleFont = -1;
    if (TitleFont == -1) {
        TitleFont = CreateFontToHandle("メイリオ", 35, 3, DX_FONTTYPE_ANTIALIASING);
    }
    static int SubFont = -1;
    if (SubFont == -1) {
        SubFont = CreateFontToHandle("メイリオ", 20, 3, DX_FONTTYPE_ANTIALIASING);
    }
    sprintf_s(buf, "%s", "[アクティブリコール]");
    DrawGlitchText(70, 30, buf, TitleFont);
    sprintf_s(buf, "%s", "毎日勉強した事を思い出して、記憶を定着させましょう");
    DrawGlitchText(50, 80, buf, SubFont);

    //通常モード時
    if (!isInputMode) {
        DrawGlitchFrame(50, 110, 1190, 590, darkBgColor, thickness, bgAlpha);

        //追加ボタン
        DrawGlitchFrame(800, 40, 1150, 90, darkBgColor, thickness, bgAlpha);
        sprintf_s(buf, "%s", "思い出した事をメモ");
        DrawGlitchText(890, 58, buf, SubFont);

        if (leftClicked && IsHit(mouseX, mouseY, 800, 40, 1150, 90)) {
            isInputMode = true;
            inputHandle = MakeKeyInput(60, FALSE, FALSE, FALSE, FALSE, FALSE);
            SetActiveKeyInput(inputHandle);
        }

        //過去の学習メモ履歴エリア
        sprintf_s(buf, "%s", "今までの履歴");
        DrawGlitchText(70, 135, buf, SubFont);

        //マウスホイールによるスクロール処理
        static int scrollOffset = 0;       // スクロール量を記憶する変数
        int wheel = GetMouseWheelRotVol(); // ホイールの回転量を取得
        if (wheel != 0) {
            scrollOffset -= wheel * 40;    // 1回のホイールで40px移動
        }

        //スクロールの制限（一番上や、一番下を超えないようにする）
        int itemHeight = 50; // メモ1つの縦幅
        int totalMemos = (int)player.studyMemos.size();
        int visibleHeight = 360; // 表示できるエリアの高さ（Y座標160?520あたり）
        int maxScroll = (totalMemos * itemHeight) - visibleHeight;

        if (maxScroll < 0) maxScroll = 0; // メモが少ない時はスクロールしない
        if (scrollOffset > maxScroll) scrollOffset = maxScroll; // 下限
        if (scrollOffset < 0) scrollOffset = 0; // 上限

        // リストを新しい順（後ろから順に）描画する
        for (int i = totalMemos - 1; i >= 0; i--) {
            int displayIndex = (totalMemos - 1) - i; // 最新が0番目

            //Y座標からスクロール量を引くことで上下に動かす
            int y = 170 + displayIndex * itemHeight - scrollOffset;

            //表示エリア（Y: 160?520）からはみ出るメモは描画をスキップする
            if (y < 150) continue; // 上にはみ出た
            if (y > 520) continue; // 下にはみ出た

            // メモ枠（少し短くして、右にゴミ箱のスペースを作る）
            DrawBox(70, y, 1100, y + 40, GRAY, FALSE);
            DrawGlitchText(90, y + 12, player.studyMemos[i].date.c_str());
            DrawGlitchText(240, y + 12, player.studyMemos[i].content.c_str());

            //削除（×）ボタン
            DrawBox(1110, y, 1160, y + 40, RED, TRUE);
            DrawString(1130, y + 12, "×", WHITE);

            // ゴミ箱がクリックされたら削除して即セーブ！
            if (leftClicked && IsHit(mouseX, mouseY, 1110, y, 1160, y + 40)) {
                player.studyMemos.erase(player.studyMemos.begin() + i);
                player.Save();
                break;
            }
        }
        // メモが1つもない時の案内
        if (player.studyMemos.empty()) {
            sprintf_s(buf, "%s", "ここに打ち込んだメモが残ります");
            DrawGlitchText(100, 200, buf, SubFont);
        }

        //ホームボタン
        DrawExtendGraph(15, 590, 240, 660, player.HomeHandle, TRUE);
        if (leftClicked && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            return SceneID::Home;
        }
    }
    //文字入力モード
    else {
        //ウィンドウを広げる 
        int darkBgColor = GetColor(15, 15, 18);
        DrawGlitchFrame(200, 200, 1080, 450, darkBgColor, thickness, bgAlpha);

        //タイトルや案内テキストをグリッチ化（DrawGlitchTextに置き換え）
        DrawGlitchText(230, 230, "【 今日の勉強内容を入力 】");
        DrawGlitchText(230, 270, "今日学習したことや、覚えた内容を簡単に入力してEnterキーを押してください（30文字まで）。");
        DrawGlitchText(230, 400, "※ Deleteキーでキャンセル");

        //入力文字のグリッチ（色化けと明滅）計算 
        int flicker = GetRand(100);
        int mainColor = GetColor(255, 255, 255); // 基本は白

        if (flicker > 95) {
            mainColor = GetColor(255, 0, 100); // 呪いの赤
        }
        else if (flicker > 90) {
            mainColor = GetColor(0, 200, 255); // デジタルな青
        }

        // 5%の確率で入力文字全体が薄くチラつく
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

        //入力枠を横に広げる
        DrawBox(230, 320, 1050, 360, WHITE, FALSE);

        //リアルタイム30文字制限（HomeSceneの関数をそのまま）
        char currentStr[100];
        GetKeyInputString(currentStr, inputHandle);
        if (CountCharacters(currentStr) > 30) {
            std::string clippedStr = ClipToMaxCharacters(currentStr, 30);
            SetKeyInputString(clippedStr.c_str(), inputHandle);
        }

        //描画エリアの制限（クリッピングではみ出し防止）
        RECT oldRect;
        GetDrawArea(&oldRect);
        SetDrawArea(230, 0, 1050, 1000);

        // 文字の描画
        DrawKeyInputString(240, 332, inputHandle);

        // 描画エリアを元に戻す
        SetDrawArea(oldRect.left, oldRect.top, oldRect.right, oldRect.bottom);

        //入力枠の横スジ（ブロックノイズ）エフェクト
        if (GetRand(100) > 80) { // 20%の確率でノイズ発生
            int noiseY = 325 + GetRand(30);   // 入力枠の中でランダムな高さ
            int noiseHeight = GetRand(3) + 1; // 削る線の太さ

            // 背景色（darkBgColor）で上書きして、文字が「横に欠けている」ように見せる
            DrawBox(235, noiseY, 1045, noiseY + noiseHeight, darkBgColor, TRUE);

            // たまに走査線（白いノイズ線や赤いエラー線）も走らせる
            if (GetRand(100) > 50) {
                int noiseColor = (GetRand(10) > 5) ? GetColor(200, 200, 200) : GetColor(255, 0, 100);
                DrawLine(235, noiseY, 1045, noiseY, noiseColor, 1);
            }
        }

        // チラつき用のブレンド設定を通常に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // Enterキーで決定
        if (CheckKeyInput(inputHandle) == 1) {
            char str[100];
            GetKeyInputString(str, inputHandle);
            DeleteKeyInput(inputHandle);
            inputHandle = -1;
            isInputMode = false;

            // 確定時も念のため30文字でカット
            std::string contentStr = ClipToMaxCharacters(str, 30);
            if (!contentStr.empty()) {
                //今日の日付を取得する
                DATEDATA date;
                GetDateTime(&date);
                char dateBuf[64];
                //日付文字列に変更
                sprintf_s(dateBuf, "%04d/%02d/%02d", date.Year, date.Mon, date.Day);
                //日付とメモの内容を保存する
                player.studyMemos.push_back(AR(dateBuf, contentStr));
                //メモの褒美:経験値+5とコイン+5を付与
                player.AddReward(5, 5);
                //メモを書いた瞬間にテキストファイルへセーブ
                player.Save();
            }
        }
        // Deleteキーでキャンセル
        else if (CheckHitKey(KEY_INPUT_DELETE) == 1) {
            DeleteKeyInput(inputHandle);
            inputHandle = -1;
            isInputMode = false;
            while (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { ProcessMessage(); }
        }
    }
    return SceneID::None;
}