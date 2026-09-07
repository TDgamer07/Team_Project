#include "DxLib.h"
#include "ShopScene.h"

extern void DrawGlitchText(int x, int y, const char* text, int fontHandle = -1);

SceneID ShopScene::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    const int GRAY = GetColor(80, 80, 80);
    const int BLACK = GetColor(0, 0, 0);

    int t = GetNowCount();

    //漆黒の背景
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

    //じわじわと垂れる血（上から下へ伸びる線）
    static float bloodY[30];
    static int bloodX[30];
    static bool initBlood = false;

    if (!initBlood) {
        for (int i = 0; i < 30; i++) {
            bloodX[i] = GetRand(1280);
            bloodY[i] = GetRand(720) * -1.0f; // 画面外の様々な高さからスタート
        }
        initBlood = true;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    for (int i = 0; i < 30; i++) {
        // 血がゆっくりと下に伸びる
        bloodY[i] += 0.2f + (GetRand(5) / 10.0f);
        if (bloodY[i] > 720) bloodY[i] = -GetRand(300); // 下まで行ったらリセット

        // 血の筋を描画（少し太さを変えて生々しく）
        int w = (i % 3 == 0) ? 3 : 1;
        DrawBox(bloodX[i] - w, 0, bloodX[i] + w, (int)bloodY[i], GetColor(120, 0, 0), TRUE);
        // 先端に血の雫
        DrawCircle((int)bloodX[i], (int)bloodY[i], (int)(w + 1), GetColor(150, 0, 0), TRUE);
    }
   
    char buf[64];
    static int ShopFont = -1;
    if (ShopFont == -1) {
        ShopFont = CreateFontToHandle("メイリオ", 35, 3, DX_FONTTYPE_ANTIALIASING);
    }
    static int GuideFont = -1;
    if (GuideFont == -1) {
        GuideFont = CreateFontToHandle("メイリオ", 20, 3, DX_FONTTYPE_ANTIALIASING);
    }
    sprintf_s(buf, "%s", "[ ショップ ]");
    DrawGlitchText(70, 30, buf, ShopFont);

    sprintf_s(buf, "%s", "通貨を支払うことで、ミニゲームで使用する武器を購入できます");
    DrawGlitchText(280, 40, buf, GuideFont);

    sprintf_s(buf, "所持                      %d 魂", player.coins);
    DrawGlitchText(950, 40, buf, GuideFont);
    
    //DrawString(950, 60, message.c_str(), RED); // お知らせメッセージ

    DrawExtendGraph(20, 50, 1220, 620, Shopframe2Handle, TRUE);

    DrawGlitchText(260, 80, "左上から順番に購入できます");

    sprintf_s(buf, "%s", "購入済み");
    
    //通常のショップ画面（武器を並べて表示）
    if (selectedWeaponIndex == -1) {
        // 剣 : 0 
        DrawExtendGraph(10, 100, 250, 300, ItemHandle, TRUE);
        DrawExtendGraph(20, 140, 210, 290, SwordHandle, TRUE);
        if (player.weapons[0].isOwned) DrawGlitchText(90, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 50, 150, 200, 260)) { selectedWeaponIndex = 0; message = ""; }

        // 杖 : 1 
        DrawExtendGraph(200, 100, 440, 300, ItemHandle, TRUE);
        DrawExtendGraph(230, 140, 400, 270, CaneHandle, TRUE);
        if (player.weapons[1].isOwned) DrawGlitchText(280, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 250, 150, 350, 260) && player.weapons[0].isOwned) { selectedWeaponIndex = 1; message = ""; }

        // 鎌 : 2 
        DrawExtendGraph(390, 100, 630, 300, ItemHandle, TRUE);
        DrawExtendGraph(440, 140, 570, 270, SickleHandle, TRUE);
        if (player.weapons[2].isOwned) DrawGlitchText(470, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 445, 150, 550, 260) && player.weapons[1].isOwned) { selectedWeaponIndex = 2; message = ""; }

        // 斧 : 3 
        DrawExtendGraph(580, 100, 820, 300, ItemHandle, TRUE);
        DrawExtendGraph(620, 140, 780, 270, AxeHandle, TRUE);  
        if (player.weapons[3].isOwned) DrawGlitchText(660, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 650, 150, 750, 260) && player.weapons[2].isOwned) { selectedWeaponIndex = 3; message = ""; }
     
        // 槍 : 4 
        DrawExtendGraph(770, 100, 1010, 300, ItemHandle, TRUE);
        DrawExtendGraph(830, 140, 950, 260, SpearHandle, TRUE);
        if (player.weapons[4].isOwned) DrawGlitchText(850, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 830, 150, 950, 260) && player.weapons[3].isOwned) { selectedWeaponIndex = 4; message = ""; }

        // 弓 : 5 
        DrawExtendGraph(960, 100, 1200, 300, ItemHandle, TRUE);
        DrawExtendGraph(960, 140, 1150, 260, BowHandle, TRUE);
        if (player.weapons[5].isOwned) DrawGlitchText(1040, 210, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 970, 150, 1130, 260) && player.weapons[4].isOwned) { selectedWeaponIndex = 5; message = ""; }

        // 拳銃 : 6 
        DrawExtendGraph(10, 350, 250, 550, ItemHandle, TRUE);
        DrawExtendGraph(60, 380, 210, 530, HandgunHandle, TRUE);
        if (player.weapons[6].isOwned) DrawGlitchText(90, 460, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 50, 370, 200, 520) && player.weapons[5].isOwned) { selectedWeaponIndex = 6; message = ""; }

        // スナイパー : 7 
        DrawExtendGraph(200, 350, 440, 550, ItemHandle, TRUE);
        DrawExtendGraph(260, 410, 380, 470, SniperHandle, TRUE);
        if (player.weapons[7].isOwned) DrawGlitchText(280, 460, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 250, 370, 400, 480) && player.weapons[7].isOwned) { selectedWeaponIndex = 7; message = ""; }

        // ダブルブレードハサミ 
        DrawExtendGraph(390, 350, 630, 550, ItemHandle, TRUE);
        DrawExtendGraph(450, 380, 560, 530, DoubleBladeHandle, TRUE);
        if (player.weapons[8].isOwned) DrawGlitchText(470, 460, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 420, 370, 580, 480) && player.weapons[8].isOwned) { selectedWeaponIndex = 8; message = ""; }

        //鎌2 
        DrawExtendGraph(580, 350, 820, 550, ItemHandle, TRUE);
        DrawExtendGraph(620, 380, 770, 510, Sickle2Handle, TRUE);
        if (player.weapons[9].isOwned) DrawGlitchText(660, 460, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 620, 370, 770, 480) && player.weapons[9].isOwned) { selectedWeaponIndex = 9; message = ""; }

        //槍2 
        DrawExtendGraph(770, 350, 1010, 550, ItemHandle, TRUE);
        DrawExtendGraph(820, 390, 950, 510, Spear2Handle, TRUE);
        if (player.weapons[10].isOwned) DrawGlitchText(850, 460, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 810, 370, 960, 480) && player.weapons[10].isOwned) { selectedWeaponIndex = 10; message = ""; }

        //戻るボタン
        DrawExtendGraph(2, 620, 220, 680, player.HomeHandle, TRUE);
        if (leftClicked && IsHit(mouseX, mouseY, 30, 620, 200, 680)) {
            return SceneID::Home;
        }
    }
    //購入確認画面（武器がクリックされた時に上に被せて表示）
    else {
        // 画面中央にポップアップウィンドウを描画
        DrawBox(300, 200, 980, 500, BLACK, TRUE); // 黒いフチ
        DrawBox(305, 205, 975, 495, GRAY, TRUE);  // グレーの背景

        // 選択された武器のデータを参照する
        Weapon& w = player.weapons[selectedWeaponIndex];

        sprintf_s(buf, "%sを購入しますか？", w.name.c_str());
        DrawGlitchText(350, 250, buf, GuideFont);

        sprintf_s(buf, "価格は　 %d 魂となります", w.price);
        DrawGlitchText(350, 300, buf, GuideFont);

        // 購入不可の理由を表示
        if (w.isOwned) {
            sprintf_s(buf, "%s", "※ すでに購入してるみたいですよ～");
            DrawGlitchText(350, 350, buf, GuideFont);
        }
        else if (player.coins < w.price) {
            sprintf_s(buf, "%s", "※ 魂が足りないようだ、もっと自分のタスクをこなそう");
            DrawGlitchText(350, 350, buf, GuideFont);
        }

        //キャンセルボタン
        DrawBox(400, 400, 600, 460, BLACK, TRUE);
        sprintf_s(buf, "%s", "やめる");
        DrawGlitchText(460, 420, buf, GuideFont);
        if (leftClicked && IsHit(mouseX, mouseY, 400, 400, 600, 460)) {
            selectedWeaponIndex = -1; // 選択を解除して元の画面に戻る
        }

        //購入するボタン
        // まだ持っていない ＆ コインが足りている時だけ押せる
        if (!w.isOwned && player.coins >= w.price) {
            DrawBox(700, 400, 900, 460, BLACK, TRUE);
            sprintf_s(buf, "%s", "購入する");
            DrawGlitchText(760, 420, buf, GuideFont);
            if (leftClicked && IsHit(mouseX, mouseY, 700, 400, 900, 460)) {
                player.coins -= w.price;  // コインを減らす
                w.isOwned = true;         // 所持フラグをON
                player.Save();            // セーブ！

                message = w.name + " を購入しました！";
                selectedWeaponIndex = -1; // 買ったら元の画面に戻る
            }
        }
        else {
            // 買えない時はボタンを暗くして押せなくする
            DrawBox(700, 400, 900, 460, BLACK, FALSE);
            sprintf_s(buf, "%s", "購入不可");
            DrawGlitchText(760, 420, buf, GuideFont);
        }
    }

    return SceneID::None;
}