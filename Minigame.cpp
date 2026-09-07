#include "DxLib.h"
#include "Minigame.h"
#include "keyboard.h"
#include "stdlib.h"
#include "time.h"
#include "Common.h"
#include <fstream>

int gamemode = 0;
const int WHITE = GetColor(255, 255, 255);
float mag = 1;
int playtime = 0;
int wepnum = 0;
int playnum = 0;
int weptex[12];
int ct[12] = { 30 ,  30 , 30 , 50 , 15 , 25 , 30 , 60 ,  40 , 30,  0 ,20 };
int gtime[12] = { 20 ,  20 , 10,1000 ,  5 , 20 ,  9 , 20 ,1000,  30,  6 , 9 };
int sizex[12] = { 142 , 100, 700,  100, 257, 100, 100, 100, 219, 100,290 ,100 };
int sizey[12] = { 100 , 100, 100,  100, 100, 100, 100, 100, 100, 100,170 , 100 };
int speedx[12] = { 0 ,   5 ,  0 , 20 ,  0 , 40 , 80 ,120 , 20 ,  20,  0 ,  80 };
float speedy[12] = { 0 ,   0 ,  0 , 10 ,  0 ,  3 ,  0 ,  0 , 10 ,  0,  0 , 0 };//落下
float gra[12] = { 0 ,   0 ,  0 ,  1 ,  0 ,0.5 ,  0 ,  0 ,  1 ,  0,  0 , 0 };//重力
int penet[12] = { 0,    0,   0,   0,   1,   0,   0,   1,   1 ,  1,  0 , 1 };//貫通
float spmag[12] = { 1,    1,   1,   1,   1  , 1,   1,   0,   1,   1,  1 , 1 };//減速
float scoremag[12] = { 1.1,    1.2,   1,   1.4,   1.1  , 1.3,   1.4,   1.5,   1.6,   1.7,  2 , 1.8 };//スコア倍率
int motion[12] = { 1,1,1,0,1,0,0,0,0,1,0,0 };
int score = 0;
int heightscore = 0;
int numtex[10];
int cttex[91];
int numMax = 12;
int playmax = 3;
int playertex[3];
int back[6];
int wepx[12] = { 100 , 100, 100, 100, 100, 100, 100, 250, 219 ,  100,290 , 100 };
int getflg[12] = { 1,    0,   0,   0,   0,   0,   0,   0,   0 ,  0,  0  , 0 };//所持
int pflg[3] = { 1,0,0 };//player所持
int pjump[3] = { 2,1,3 };
float atacksp[3] = { 1,0.8,1 };
float runsp[3] = { 1,0.9,1 };

int item[6];
float iatack[5] = { 0.92,0.9,1,1,1 };
int ijump[5] = { 1,0,1,0,0 };
float irun[5] = { 0.92,1,1,0.9,1 };
float iscore[5] = { 1.18,1,1,1,1.2 };
int ilv[6] = { 0,0,0,0,0,0 };
int itemnum = 0;
int jem;
int jemnum = 0;

int start;
int atacks;
int atacks1;
int die;
int die1;
int jumps;
int bgm0;
int bgm1;
int bgm2;
int load;
int tup;
int gs;
int fum;

int button[11];

int ecount1 = 0;
int ecount2 = 0;
int char1;
int tempx = 100;
int tempy = 100;
int tempmg = 1;

int oldMouse;
int mouse = 0;
int gatyaflg = 0;

int items[10];
//まかせる
SceneID MiniGameScene::UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) {
    static PLAYER gPlayer;
    static ENEMY gEnemy[20];
    static ENEMY2 gEnemy2[20];
    static ENEMY3 gEnemy3[20];
    static SKY sky[10];
    static WEP wep;
    static BULLET bullet[10];
    static int loadflg = 0;
    GetMousePoint(&mouseX, &mouseY);
    oldMouse = mouse;
    mouse = GetMouseInput();
    bool Lclick = (mouse & MOUSE_INPUT_LEFT);

    if (oldMouse) {
        if (!Lclick) {
            PlaySoundMem(tup, DX_PLAYTYPE_BACK);
        }
    }


    KeyUpdate();
    switch (gamemode) {
    case 0:
        if (loadflg == 0) {
            loadflg = 1;
        }
        else {
            gamemode = 1;
            break;
        }
        oldMouse = FALSE;
        load = LoadSoundMem("music\\load.mp3");
        loadd();

        ChangeVolumeSoundMem(100, load);
        PlaySoundMem(load, DX_PLAYTYPE_BACK);
        for (int i = 0; i < 6; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\back%d.png", i);
            if ((back[i] = LoadGraph(buff)) == -1) {
            }
        }
        for (int i = 0; i < 3; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\player%d.png", i);
            if ((playertex[i] = LoadGraph(buff)) == -1) {
            }
        }
        for (int i = 0; i < 12; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\wep%d.png", i);
            if ((weptex[i] = LoadGraph(buff)) == -1) {
            }
        }
        for (int i = 0; i < 11; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\button%d.png", i);
            if ((button[i] = LoadGraph(buff)) == -1) {
            }
        }
        for (int i = 0; i < 5; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\item%d.png", i);
            if ((item[i] = LoadGraph(buff)) == -1) {
            }
        }
        if ((item[5] = LoadGraph("images\\item6.png")) == -1) {
        }
        DrawGraph(0, 0, back[3], TRUE);
        ScreenFlip();

        SetAlwaysRunFlag(TRUE);


        if ((char1 = LoadGraph("images\\char.png")) == -1) {
        }
        bgm0 = LoadSoundMem("music\\gamebgm0.mp3");
        ChangeVolumeSoundMem(100, bgm0);

        bgm1 = LoadSoundMem("music\\gamebgm1.mp3");
        ChangeVolumeSoundMem(90, bgm1);

        bgm2 = LoadSoundMem("music\\gamebgm2.mp3");
        ChangeVolumeSoundMem(100, bgm2);

        start = LoadSoundMem("music\\start.mp3");
        ChangeVolumeSoundMem(100, start);

        atacks = LoadSoundMem("music\\atack.mp3");
        ChangeVolumeSoundMem(100, atacks);

        atacks1 = LoadSoundMem("music\\atack1.mp3");
        ChangeVolumeSoundMem(100, atacks1);

        die = LoadSoundMem("music\\enemydie.mp3");
        ChangeVolumeSoundMem(210, die);

        die1 = LoadSoundMem("music\\die1.mp3");
        ChangeVolumeSoundMem(100, die1);

        jumps = LoadSoundMem("music\\jump.mp3");
        ChangeVolumeSoundMem(100, jumps);

        gs = LoadSoundMem("music\\gas.mp3");
        ChangeVolumeSoundMem(200, gs);

        tup = LoadSoundMem("music\\tup.mp3");
        ChangeVolumeSoundMem(100, tup);

        fum = LoadSoundMem("music\\fum.mp3");
        ChangeVolumeSoundMem(100, fum);
        gPlayer.set(0);

        for (int i = 0; i < 91; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\ct%d.png", i);
            if ((cttex[i] = LoadGraph(buff)) == -1) {
            }
        }
        sky[0].set(0);
        for (int i = 1; i < 10; i++) {
            sky[i].set(sky[i - 1].mPosX + (float)sky[0].size);
        }
        for (int i = 0; i < 10; i++) {
            char buff[100];
            sprintf_s(buff, 100, "images\\num%d.png", i);
            if ((numtex[i] = LoadGraph(buff)) == -1) {
            }
        }
        while (CheckSoundMem(load) == 1);

        gamemode++;

        break;
    case 1:
    {
        int temp = player.GetWeaponCnt();
        for (int i = 0; i < temp; i++) {
            getflg[i] = 1;
        }
    }
    tempmg = 1;
    mag = 1;
    playtime = 0;
    if (ecount1 + ecount2 > 80) {
        pflg[1] = 1;
    }
    if (ecount2 > 20) {
        pflg[2] = 1;
    }
    getflg[11] = ilv[5];

    if (CheckSoundMem(bgm1) == 0) {
        PlaySoundMem(bgm1, DX_PLAYTYPE_BACK);
    }
    DrawGraph(0, 0, back[gamemode - 1], TRUE);
    if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
        DrawExtendGraph(67, 605, 203, 645, button[0], TRUE);
    }
    else {
        DrawGraph(50, 600, button[0], TRUE);
    }
    ///////////////ガチャ
    {
        int x = 980;
        int y = 590;
        if (Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
            DrawExtendGraph(x + 17, y + 5, x + 153, y + 45, button[1], TRUE);
        }
        else {
            DrawGraph(x, y, button[1], TRUE);
        }
        if (oldMouse) {
            if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
                gatyaflg = 0;
                gamemode = 4;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        if (heightscore == 0) {
            i = 9;
            dnum(829 + 40 * i, 28, 0);
        }
        else if (heightscore >= pow(10, 9 - i)) {
            int temp = heightscore / ((int)pow(10, 9 - i)) % 10;
            dnum(829 + 40 * i, 28, temp);
        }
    }
    for (int i = 0; i < 6; i++) {
        if (jemnum == 0) {
            i = 5;
            dnum(979 + 40 * i, 510, 0);
        }
        else if (jemnum >= pow(10, 5 - i)) {
            int temp = jemnum / ((int)pow(10, 5 - i)) % 10;
            dnum(979 + 40 * i, 510, temp);
        }
    }
    if (oldMouse && !Lclick && IsHit(mouseX, mouseY, 529, 414, 711, 425)) {
        wepnum++;

        while (getflg[wepnum % numMax] == 0) {
            wepnum++;
        }
    }
    if (IsKeyRelease(KEY_UP)) {
        PlaySoundMem(tup, DX_PLAYTYPE_BACK);
        wepnum++;

        while (getflg[wepnum % numMax] == 0) {
            wepnum++;
        }
    }
    if (oldMouse && !Lclick && IsHit(mouseX, mouseY, 622, 414, 618, 425)) {
        wepnum += numMax - 1;
        while (getflg[wepnum % numMax] == 0) {
            wepnum += numMax - 1;
        }
    }
    if (IsKeyRelease(KEY_DOWN)) {
        PlaySoundMem(tup, DX_PLAYTYPE_BACK);

        wepnum += numMax - 1;
        while (getflg[wepnum % numMax] == 0) {
            wepnum += numMax - 1;
        }
    }
    if (oldMouse && !Lclick && IsHit(mouseX, mouseY, 680, 210, 695, 270)) {
        playnum++;
        while (pflg[playnum % playmax] == 0) {
            playnum++;
        }
    }
    if (IsKeyRelease(KEY_RIGHT)) {

        PlaySoundMem(tup, DX_PLAYTYPE_BACK);
        playnum++;
        while (pflg[playnum % playmax] == 0) {
            playnum++;
        }
    }
    if (oldMouse && !Lclick && IsHit(mouseX, mouseY, 545, 210, 559, 270)) {
        playnum += playmax - 1;
        while (pflg[playnum % playmax] == 0) {
            playnum += playmax - 1;
        }
    }
    if (IsKeyRelease(KEY_LEFT)) {
        PlaySoundMem(tup, DX_PLAYTYPE_BACK);

        playnum += playmax - 1;
        while (pflg[playnum % playmax] == 0) {
            playnum += playmax - 1;
        }
    }
    wepnum %= numMax;
    playnum %= playmax;
    score = 0;
    gPlayer.set(playertex[playnum]);
    gPlayer.mPosX = 570;
    gPlayer.mPosY = 190;
    gPlayer.draw();
    wep.set(weptex[wepnum], wepnum);

    bulletset(bullet);
    wep.draw(620 - wepx[wepnum] / 2, 450);
    DrawExtendGraph(tempx - (30 * tempmg), tempy - (77 * tempmg), tempx + (30 * tempmg), tempy + (26 * tempmg), char1, TRUE);
    if (leftClicked && IsHit(mouseX, mouseY, tempx - (30 * tempmg), tempy - (77 * tempmg), tempx + (30 * tempmg), tempy + (26 * tempmg))) {
        tempmg++;
    }
    if (IsKeyRelease(KEY_RETURN)) {

        mag = 1;
        playtime = 0;
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            gEnemy[enemyIdx].set();
        }
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            gEnemy2[enemyIdx].set();
        }
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            gEnemy3[enemyIdx].set();
        }
        for (int i = 0; i < 10; i++) {
            bullet[i].set(wepnum);
        }
        gPlayer.set(playertex[playnum]);
        if (tempmg >= 15) {
            gPlayer.mImgNo = gEnemy2[0].mImgNo;
        }
        gPlayer.jflg = pjump[playnum];
        gamemode++;
        StopSoundMem(bgm1);
        PlaySoundMem(start, DX_PLAYTYPE_BACK);

    }

    break;
    case 2:

        if (CheckSoundMem(bgm0) == 0) {
            PlaySoundMem(bgm0, DX_PLAYTYPE_BACK);
        }
        // 「ホームに戻る」ボタン
        DrawGraph(0, 0, back[gamemode - 1], TRUE);
        if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            DrawExtendGraph(67, 605, 203, 645, button[4], TRUE);
        }
        else {
            DrawGraph(50, 600, button[4], TRUE);
        }
        for (int i = 0; i < 10; i++) {
            int temp = score / ((int)pow(10, 9 - i)) % 10;
            if (score > pow(10, 9 - i)) {
                dnum(10 + 40 * i, 500, temp);
            }
        }
        playtime++;
        if (playtime > 200) {
            playtime = 0;
            mag += 0.1 * spmag[wepnum];
        }
        score += mag * pow(iscore[itemnum], ilv[itemnum]);
        skymove(sky, mag);

        playermove(&gPlayer);
        enemymove(gEnemy, mag);
        enemymove2(gEnemy2, mag);
        enemymove3(gEnemy3, mag);
        hitcheck(&gPlayer, gEnemy);
        hitcheck2(&gPlayer, gEnemy2);
        hitcheck3(&gPlayer, gEnemy3);

        atack(&gPlayer, gEnemy, gEnemy2, gEnemy3, &wep, bullet);


        if (wep.ct > 0) {
            int cttemp = 0;
            float temp = (float)ct[wepnum] * atacksp[playnum] * pow(iatack[itemnum], ilv[itemnum]) / 90;
            cttemp = wep.ct / temp;
            DrawGraph(1000, 500, cttex[cttemp], TRUE);
        }
        else {
            DrawGraph(1000, 500, cttex[0], TRUE);
        }

        break;
    case 3:
        if (CheckSoundMem(bgm2) == 0) {
            PlaySoundMem(bgm2, DX_PLAYTYPE_BACK);
        }
        DrawGraph(0, 0, back[gamemode - 1], TRUE);
        if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            DrawExtendGraph(67, 605, 203, 645, button[4], TRUE);
        }
        else {
            DrawGraph(50, 600, button[4], TRUE);
        }

        for (int i = 0; i < 10; i++) {
            int temp = score / ((int)pow(10, 9 - i)) % 10;
            if (score > pow(10, 9 - i)) {
                dnum(400 + 40 * i, 300, temp);
            }
        }

        if (IsKeyRelease(KEY_RETURN)) {
            tempmg = 1;

            gamemode = 1;


            StopSoundMem(bgm2);
            saved();

            if (heightscore < score) {
                heightscore = score;
            }
        }
        break;
    case 4:
        if (CheckSoundMem(bgm1) == 0) {
            PlaySoundMem(bgm1, DX_PLAYTYPE_BACK);
        }
        DrawGraph(0, 0, back[4], TRUE);

        //戻る
        if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            DrawExtendGraph(67, 605, 203, 645, button[4], TRUE);
        }
        else {
            DrawGraph(50, 600, button[4], TRUE);
        }
        {
            int x = 50;
            int y = 150;
            DrawGraph(x, y, button[10], TRUE);

            if ((oldMouse && !Lclick && IsHit(mouseX, mouseY, x + 301, y + 49, x + 349, y + 185)) || (IsKeyRelease(KEY_RIGHT))) {

                PlaySoundMem(tup, DX_PLAYTYPE_BACK);
                itemnum++;
            }
            if ((oldMouse && !Lclick && IsHit(mouseX, mouseY, x + 22, y + 49, x + 71, y + 185)) || (IsKeyRelease(KEY_LEFT))) {
                PlaySoundMem(tup, DX_PLAYTYPE_BACK);

                itemnum += 4;
            }
            itemnum %= 5;
            DrawGraph(x + 88, y + 17, item[itemnum], TRUE);
            DrawGraph(x + 190, y + 231, numtex[ilv[itemnum]], TRUE);
        }
        //単発
        {
            int x = 900;
            int y = 300;
            if (Lclick && IsHit(mouseX, mouseY, x, y, x + 200, y + 70) && gatyaflg == 0) {
                DrawExtendGraph(x + 20, y + 7, x + 180, y + 63, button[2], TRUE);
            }
            else {
                DrawGraph(x, y, button[2], TRUE);
            }
            if (oldMouse) {
                if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 200, y + 70)) {
                    if (jemnum >= 10) {
                        gatyaflg = 1;
                    }
                }
            }
        }
        //10連
        {
            int x = 900;
            int y = 450;
            if (Lclick && IsHit(mouseX, mouseY, x, y, x + 200, y + 70) && gatyaflg == 0) {
                DrawExtendGraph(x + 20, y + 7, x + 180, y + 63, button[3], TRUE);
            }
            else {
                DrawGraph(x, y, button[3], TRUE);
            }
            if (oldMouse) {
                if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 200, y + 70)) {
                    if (jemnum >= 100) {
                        gatyaflg = 2;
                    }
                }
            }
        }
        if (gatyaflg > 0) {
            DrawGraph(370, 200, button[5], TRUE);
            DrawGraph(400, 235, button[1 + gatyaflg], TRUE);

            {
                int x = 370 + 100;
                int y = 200 + 200;
                if (Lclick && IsHit(mouseX, mouseY, x, y, x + 100, y + 50)) {
                    DrawExtendGraph(x + 10, y + 5, x + 90, y + 45, button[6], TRUE);
                }
                else {
                    DrawGraph(x, y, button[6], TRUE);
                }
                if (oldMouse) {
                    if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 100, y + 50)) {
                        PlaySoundMem(gs, DX_PLAYTYPE_BACK);
                        items[0] = -1;
                        gamemode += gatyaflg;

                    }
                }
            }
            {
                int x = 370 + 300;
                int y = 200 + 200;
                if (Lclick && IsHit(mouseX, mouseY, x, y, x + 100, y + 50)) {
                    DrawExtendGraph(x + 10, y + 5, x + 90, y + 45, button[7], TRUE);
                }
                else {
                    DrawGraph(x, y, button[7], TRUE);
                }
                if (oldMouse) {
                    if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 100, y + 50)) {
                        gatyaflg = 0;
                    }
                }
            }
        }
        break;
    case 5:
        if (CheckSoundMem(bgm1) == 0) {
            PlaySoundMem(bgm1, DX_PLAYTYPE_BACK);
        }
        DrawGraph(0, 0, back[5], TRUE);
        //戻る
        if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            DrawExtendGraph(67, 605, 203, 645, button[4], TRUE);
        }
        else {
            DrawGraph(50, 600, button[4], TRUE);
        }
        {
            int x = 980;
            int y = 590;
            if (Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
                DrawExtendGraph(x + 17, y + 5, x + 153, y + 45, button[9], TRUE);
            }
            else {
                DrawGraph(x, y, button[9], TRUE);
            }
            if (oldMouse) {
                if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
                    gatyaflg = 0;
                    gamemode = 4;
                }
            }
        }
        if (items[0] == -1) {
            jemnum -= 10;
            {
                int temp = getitem();
                if (ilv[temp] < 9) {
                    ilv[temp]++;
                }
                items[0] = temp;
            }
            saved();

        }
        DrawGraph(520, 250, item[items[0]], TRUE);

        break;
    case 6:
        if (CheckSoundMem(bgm1) == 0) {
            PlaySoundMem(bgm1, DX_PLAYTYPE_BACK);
        }
        DrawGraph(0, 0, back[5], TRUE);
        //戻る
        if (Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            DrawExtendGraph(67, 605, 203, 645, button[4], TRUE);
        }
        else {
            DrawGraph(50, 600, button[4], TRUE);
        }
        {
            int x = 980;
            int y = 590;
            if (Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
                DrawExtendGraph(x + 17, y + 5, x + 153, y + 45, button[9], TRUE);
            }
            else {
                DrawGraph(x, y, button[9], TRUE);
            }
            if (oldMouse) {
                if (!Lclick && IsHit(mouseX, mouseY, x, y, x + 170, y + 50)) {
                    gatyaflg = 0;
                    gamemode = 4;
                }
            }
        }
        if (items[0] == -1) {
            jemnum -= 100;
            {
                for (int i = 0; i < 10; i++) {
                    int temp = getitem();
                    if (ilv[temp] < 9) {
                        ilv[temp]++;
                    }
                    items[i] = temp;
                }
            }
            saved();
        }
        for (int i = 0; i < 5; i++) {
            DrawGraph(120 + i * 200, 150, item[items[i]], TRUE);
        }
        for (int i = 0; i < 5; i++) {
            DrawGraph(120 + i * 200, 350, item[items[5 + i]], TRUE);
        }
        break;
    }
    if (oldMouse) {
        if (!Lclick && IsHit(mouseX, mouseY, 50, 600, 220, 650)) {
            tempmg = 1;
            StopSoundMem(bgm0);

            StopSoundMem(bgm2);
            saved();

            if (heightscore < score) {
                heightscore = score;
            };
            if (gamemode < 2) {
                SetAlwaysRunFlag(FALSE);
                StopSoundMem(bgm0);
                StopSoundMem(bgm1);
                StopSoundMem(bgm2);
                gamemode = 0;
                return SceneID::Home;
            }
            else {

                gamemode = 1;


            }
        }
    }

    return SceneID::None; // 何もなければ今の画面を継続
}

void playermove(PLAYER* gPlayer) {
    if (IsKeyPush(KEY_SPACE) && gPlayer->jflg > 0) {
        gPlayer->jump();
        PlaySoundMem(jumps, DX_PLAYTYPE_BACK);

    }

    gPlayer->mPosY -= gPlayer->speed;
    if (gPlayer->mPosY < 360 - gPlayer->size) {
        gPlayer->speed -= 3;
    }
    else {
        gPlayer->speed = 0;
        gPlayer->mPosY = 360 - gPlayer->size;
        gPlayer->jflg = pjump[playnum] + ijump[itemnum] * ilv[itemnum];
    }
    gPlayer->draw();
}
void enemymove(ENEMY gEnemy[], float mag) {
    if (rand() % (int)(80 / mag) == 0) { //確率にヒットしたら
        //全ての敵の中から発生状態にない敵を見つけて、発生状態にし、座標をセット
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            if (!gEnemy[enemyIdx].mIsVisible) {
                gEnemy[enemyIdx].set();
                gEnemy[enemyIdx].mIsVisible = TRUE;
                break;						//１つ発生させたら、ループを抜ける
            }
        }
    }
    //発生状態の敵に対して表示座標の更新
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].mPosX -= gEnemy[enemyIdx].speed * mag * runsp[playnum] * spmag[wepnum] * gEnemy[enemyIdx].move + 5;
            if (gEnemy[enemyIdx].mPosX < -100) {
                gEnemy[enemyIdx].mIsVisible = FALSE;
            }
        }
    }
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].draw();
        }
    }

}
void enemymove2(ENEMY2 gEnemy[], float mag) {
    if (rand() % (int)(120 / mag) == 0) { //確率にヒットしたら
        //全ての敵の中から発生状態にない敵を見つけて、発生状態にし、座標をセット
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            if (!gEnemy[enemyIdx].mIsVisible) {
                gEnemy[enemyIdx].set();
                gEnemy[enemyIdx].mIsVisible = TRUE;
                break;						//１つ発生させたら、ループを抜ける
            }
        }
    }
    //発生状態の敵に対して表示座標の更新
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].mPosX -= gEnemy[enemyIdx].speed * mag * runsp[playnum] * spmag[wepnum] + 1;
            gEnemy[enemyIdx].mPosY += 0.5 * gEnemy[enemyIdx].move;
            if (gEnemy[enemyIdx].mPosX < -100) {
                gEnemy[enemyIdx].mIsVisible = FALSE;
            }
        }
    }
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].draw();
        }
    }

}
void enemymove3(ENEMY3 gEnemy[], float mag) {
    if (rand() % (int)(300 / mag) == 0) { //確率にヒットしたら
        //全ての敵の中から発生状態にない敵を見つけて、発生状態にし、座標をセット
        for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
            if (!gEnemy[enemyIdx].mIsVisible) {
                gEnemy[enemyIdx].set();
                gEnemy[enemyIdx].mIsVisible = TRUE;
                break;						//１つ発生させたら、ループを抜ける
            }
        }
    }
    //発生状態の敵に対して表示座標の更新
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].mPosX -= gEnemy[enemyIdx].speed * mag * runsp[playnum] * spmag[wepnum] + 1;
            if (gEnemy[enemyIdx].mPosX < -100) {
                gEnemy[enemyIdx].mIsVisible = FALSE;
            }
        }
    }
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {
            gEnemy[enemyIdx].draw();
        }
    }

}
void skymove(SKY sky[], float mag) {
    for (int i = 0; i < 10; i++) {
        sky[i].mPosX -= sky[i].speed * spmag[wepnum] * mag * runsp[playnum] * pow(irun[itemnum], ilv[itemnum]);
        if (sky[i].mPosX < -sky[i].size) {
            sky[i].set(sky[(i + 8) % 9].mPosX + (float)sky[0].size);
        }
    }
    for (int i = 0; i < 10; i++) {
        sky[i].draw();
    }

}
void hitcheck(PLAYER* gPlayer, ENEMY gEnemy[]) {
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {

            if (abs((gPlayer->mPosX + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 3 + gEnemy[enemyIdx].size / 2) {
                if (abs(gPlayer->mPosY + gPlayer->size - gEnemy[enemyIdx].mPosY) < 30) {
                    gPlayer->jflg = pjump[playnum];
                    gPlayer->jump();

                    gEnemy[enemyIdx].mIsVisible = FALSE;
                    PlaySoundMem(fum, DX_PLAYTYPE_BACK);
                    ecount1++;
                    scoreup(&score, &mag);
                }
            }
            if (abs((gPlayer->mPosX + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 4 + gEnemy[enemyIdx].size / 2) {
                if (abs((gPlayer->mPosY + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosY + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 2 + gEnemy[enemyIdx].size / 2) {
                    gamemode = 3;
                    StopSoundMem(bgm0);
                    saved();

                }
            }
        }
    }
}
void hitcheck2(PLAYER* gPlayer, ENEMY2 gEnemy[]) {
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {

            if (abs((gPlayer->mPosX + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 2 + gEnemy[enemyIdx].size / 2) {
                if (abs((gPlayer->mPosY + gPlayer->size) - (gEnemy[enemyIdx].mPosY + 50)) < 30) {
                    gPlayer->jflg = pjump[playnum];
                    gPlayer->jump();

                    gEnemy[enemyIdx].mIsVisible = FALSE;
                    scoreup(&score, &mag);
                    PlaySoundMem(fum, DX_PLAYTYPE_BACK);
                    ecount2++;
                }
            }
            if (abs((gPlayer->mPosX + gPlayer->size / 3) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 3 + gEnemy[enemyIdx].size / 2) {
                if (abs((gPlayer->mPosY + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosY + gEnemy[enemyIdx].size / 2 + 50)) < gPlayer->size / 2 + gEnemy[enemyIdx].size / 3) {
                    gamemode = 3;
                    StopSoundMem(bgm0);
                    saved();

                }
            }
        }
    }
}
void hitcheck3(PLAYER* gPlayer, ENEMY3 gEnemy[]) {
    for (int enemyIdx = 0; enemyIdx < 20; enemyIdx++) {
        if (gEnemy[enemyIdx].mIsVisible) {

            if (abs((gPlayer->mPosX + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 3 + gEnemy[enemyIdx].size / 2) {
                if (abs(gPlayer->mPosY + gPlayer->size - gEnemy[enemyIdx].mPosY) < 30) {
                    gPlayer->jflg = pjump[playnum];
                    gPlayer->jump();
                    ecount1++;

                    gEnemy[enemyIdx].mIsVisible = FALSE;
                    PlaySoundMem(fum, DX_PLAYTYPE_BACK);
                    jemnum++;
                    scoreup(&score, &mag);
                }
            }
            if (abs((gPlayer->mPosX + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosX + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 4 + gEnemy[enemyIdx].size / 2) {
                if (abs((gPlayer->mPosY + gPlayer->size / 2) - (gEnemy[enemyIdx].mPosY + gEnemy[enemyIdx].size / 2)) < gPlayer->size / 2 + gEnemy[enemyIdx].size / 2) {
                    gamemode = 3;
                    StopSoundMem(bgm0);
                    saved();

                }
            }
        }
    }
}
void atack(PLAYER* gPlayer, ENEMY gEnemy[], ENEMY2 gEnemy2[], ENEMY3 gEnemy3[], WEP* wep, BULLET* bullet) {
    wep->ct--;

    if (IsKeyPush(KEY_RETURN) && wep->ct <= 0) {
        if (wepnum != 10) {
            PlaySoundMem(atacks, DX_PLAYTYPE_BACK);
        }
        else {
            PlaySoundMem(atacks1, DX_PLAYTYPE_BACK);
        }
        for (int i = 0; i < 10; i++) {
            if (!bullet[i].mIsVisible) {
                bullet[i].mIsVisible = TRUE;
                wep->ct = ct[wepnum] * atacksp[playnum] * pow(iatack[itemnum], ilv[itemnum]);
                bullet[i].time = gtime[wepnum];
                bullet[i].mPosX = gPlayer->mPosX + gPlayer->size;
                bullet[i].mPosY = gPlayer->mPosY;
                bulletset(&bullet[i]);
                break;
            }
        }
    }
    wep->mIsVisible = TRUE;
    if (wepnum == 10) {
        for (int i = 0; i < 10; i++) {
            if (bullet[i].mIsVisible) {
                wep->mIsVisible = FALSE;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        if (bullet[i].mIsVisible) {
            bullet[i].time--;
            bullet[i].move();
            if (wepnum == 10) {
                bullet[i].mPosX = gPlayer->mPosX - 30;
                bullet[i].mPosY = gPlayer->mPosY;
                bullet[i].draw2();
            }
            else {
                bullet[i].draw();
            }
            if (bullet[i].time <= 0) {
                bullet[i].mIsVisible = FALSE;
            }
            if (bullet[i].mPosY > 400) {
                bullet[i].mIsVisible = FALSE;
            }
            for (int j = 0; j < 20; j++) {
                if (gEnemy[j].mIsVisible) {
                    if (abs((bullet[i].mPosX + bullet[i].sizex / 2) - (gEnemy[j].mPosX + gEnemy[j].size / 2)) < bullet[i].sizex / 2 + gEnemy[j].size / 2) {
                        if (abs((bullet[i].mPosY + bullet[i].sizey / 2) - (gEnemy[j].mPosY + gEnemy[j].size / 2)) < bullet[i].sizey / 2 + gEnemy[j].size / 2) {
                            gEnemy[j].mIsVisible = FALSE;
                            scoreup(&score, &mag);
                            PlaySoundMem(die, DX_PLAYTYPE_BACK);
                            ecount1++;
                            if (penet[wepnum] == 0) {
                                bullet[i].mIsVisible = FALSE;
                            }
                        }
                    }
                }
                if (gEnemy2[j].mIsVisible) {
                    if (abs((bullet[i].mPosX + bullet[i].sizex / 2) - (gEnemy2[j].mPosX + gEnemy2[j].size / 2)) < bullet[i].sizex / 2 + gEnemy2[j].size / 2) {
                        if (abs((bullet[i].mPosY + bullet[i].sizey / 2) - (gEnemy2[j].mPosY + gEnemy2[j].size / 2 + 50)) < bullet[i].sizey / 2 + gEnemy2[j].size / 2) {
                            gEnemy2[j].mIsVisible = FALSE;
                            scoreup(&score, &mag);
                            PlaySoundMem(die, DX_PLAYTYPE_BACK);
                            ecount2++;
                            if (penet[wepnum] == 0) {
                                bullet[i].mIsVisible = FALSE;
                            }
                        }
                    }
                }
                if (gEnemy3[j].mIsVisible) {
                    if (abs((bullet[i].mPosX + bullet[i].sizex / 2) - (gEnemy3[j].mPosX + gEnemy3[j].size / 2)) < bullet[i].sizex / 2 + gEnemy3[j].size / 2) {
                        if (abs((bullet[i].mPosY + bullet[i].sizey / 2) - (gEnemy3[j].mPosY + gEnemy3[j].size / 2)) < bullet[i].sizey / 2 + gEnemy3[j].size / 2) {
                            gEnemy3[j].mIsVisible = FALSE;
                            scoreup(&score, &mag);
                            PlaySoundMem(die1, DX_PLAYTYPE_BACK);
                            jemnum++;
                            ecount1++;

                            if (penet[wepnum] == 0) {
                                bullet[i].mIsVisible = FALSE;
                            }
                        }
                    }
                }
            }
        }
    }
    int X = 0;
    if (wepnum == 10) {
        X = -80;
    }
    if (wep->ct > 0 && motion[wepnum] == 1) {
        DrawRotaGraph(gPlayer->mPosX + gPlayer->size, gPlayer->mPosY + gPlayer->size / 2, 1, 0.77, wep->mImgNo, TRUE, FALSE);
    }
    else {
        wep->draw(gPlayer->mPosX + gPlayer->size / 2 + X, gPlayer->mPosY);
    }

}
void bulletset(BULLET* bullet) {
    bullet->sizex = sizex[wepnum];
    bullet->sizey = sizey[wepnum];
    bullet->speedx = speedx[wepnum];
    bullet->speedy = speedy[wepnum];
    bullet->gra = gra[wepnum];
}
void scoreup(int* score, float* mag) {
    *score += 100 * scoremag[wepnum] * *mag * pow(iscore[itemnum], ilv[itemnum]);
}
void dnum(int x, int y, int num) {
    int pic = numtex[num];
    DrawGraph(x, y, pic, TRUE);

}
int getitem() {
    int temp = rand() % 200;
    if (temp == 0) {
        return 5;
    }
    else if (temp < 60) {
        return 1;
    }
    else if (temp < 80) {
        return 2;
    }
    else if (temp < 140) {
        return 3;
    }
    else if (temp < 190) {
        return 4;
    }
    else {
        return 0;
    }
}
void saved() {
    std::ofstream ofs("SaveDataM.txt");
    if (!ofs)return;
    // ステータスとアイテムを保存
    ofs << jemnum << "\n" << ecount1 << "\n" << ecount2 << "\n" << heightscore << "\n";
    for (int i = 0; i < 6; i++) {
        ofs << ilv[i] << "\n";
    }
}
void loadd() {
    std::ifstream ifs("SaveDataM.txt");
    if (!ifs) {
        jemnum = 0;

        ecount1 = 0;
        ecount2 = 0;
        heightscore = 0;
        for (int i = 0; i < 6; i++) {
            ilv[i] = 0;
        }
        return;
    }

    ifs >> jemnum >> pflg[1] >> pflg[2] >> heightscore;
    for (int i = 0; i < 6; i++) {
        ifs >> ilv[i];
    }
}