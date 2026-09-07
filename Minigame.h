#pragma once
#include "BaseScene.h"
//プレイヤーの大きさ
#define PLAYER_SIZE (100)
#define ENEMY_SIZE (56)



//ミニゲーム関連のクラス　使うかは未定
class MiniGameScene : public BaseScene {
private:
    int miniGameScore = 0;

public:
    MiniGameScene() {}
    ~MiniGameScene() {}

    SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};
class PLAYER {

public:
    int mImgNo;
    float mPosX;
    float mPosY;
    float speed;
    int size;
    int jflg;
    // プレイヤー関連の画像ファイルのロード
    PLAYER() {
        //プレイヤー

        mPosX = 100;
        size = PLAYER_SIZE;
        mPosY = 360 - size;
    }
    ~PLAYER() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void jump() {
        jflg--;
        speed = 30;
    }
    void set(int n) {
        mImgNo = n;
        mPosX = 100;
        mPosY = 360 - size;
    }
};
class ENEMY {
public:
    int mImgNo;
    float mPosX;
    float mPosY;
    float speed;
    int size;
    int mIsVisible;
    float move;

    ENEMY() {
        if ((mImgNo = LoadGraph("images\\enemy.png")) == -1) {
        }
        mPosX = 2000;
        size = ENEMY_SIZE;
        mPosY = 360 - 39;
        speed = 5;
        mIsVisible = 0;
    }
    ~ENEMY() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void set() {
        mPosX = 2000;
        mPosY = 360 - 39;
        speed = 5;
        mIsVisible = 0;
        move = (1 + ((float)(rand() % 5) / 10));

    }
};
class ENEMY2 {
public:
    int mImgNo;
    float mPosX;
    float mPosY;
    float speed;
    int size;
    int mIsVisible;

    float move;
    ENEMY2() {
        if ((mImgNo = LoadGraph("images\\enemy2.png")) == -1) {
        }
        mPosX = 2000;
        size = 50;
        mPosY = 360 - size;
        speed = 5;
        mIsVisible = 0;
    }
    ~ENEMY2() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void set() {
        mPosX = 2000;
        mPosY = 100 + rand() % 100;
        speed = 5;
        mIsVisible = 0;

        move = ((rand() % 2) * 2 - 1) * (((float)(rand() % 10)) / 10);
    }
};
class ENEMY3 {
public:
    int mImgNo;
    float mPosX;
    float mPosY;
    float speed;
    int size;
    int mIsVisible;

    ENEMY3() {
        if ((mImgNo = LoadGraph("images\\enemy3.png")) == -1) {
        }
        mPosX = 2000;
        size = ENEMY_SIZE;
        mPosY = 360 - 50;
        speed = 5;
        mIsVisible = 0;
    }
    ~ENEMY3() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void set() {
        mPosX = 2000;
        mPosY = 360 - 50;
        speed = 5;
        mIsVisible = 0;
    }
};
class SKY {
public:
    int mImgNo;
    float mPosX;
    float mPosY;
    float speed;
    int size;
    int mIsVisible;

    // プレイヤー関連の画像ファイルのロード
    SKY() {
        //プレイヤー
        if ((mImgNo = LoadGraph("images\\sky.png")) == -1) {
        }
        mPosX = 0;
        size = 399;
        mPosY = 0;
        speed = 5;

    }
    ~SKY() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void set(float n) {
        mPosX = n;
        mPosY = 0;
        speed = 5;

    }
};
class WEP {
public:
    int mImgNo;
    int ct;
    int mIsVisible;
    int wnum;
    // プレイヤー関連の画像ファイルのロード
    WEP() {}
    ~WEP() {}
    void draw(float X, float Y) {
        if (mIsVisible) {
            DrawGraph(X, Y, mImgNo, TRUE);
        }
    }
    void set(int n, int n2) {
        ct = 0;
        mIsVisible = TRUE;

        mImgNo = n;
        wnum = n2;
    }
};
class BULLET {
public:
    int mImgNo;
    int mImgNo2;
    int wnum;

    int time;
    float mPosX;
    float mPosY;

    int sizex;
    int sizey;
    int speedx;
    float speedy;
    float gra;

    int mIsVisible;
    int c10;
    // プレイヤー関連の画像ファイルのロード
    BULLET() {}
    ~BULLET() {}
    void draw() {
        DrawGraph(mPosX, mPosY, mImgNo, TRUE);
    }
    void draw2() {
        if (c10 == 0) {
            DrawGraph(mPosX, mPosY, mImgNo, TRUE);
            c10++;

        }
        else {
            DrawGraph(mPosX, mPosY, mImgNo2, TRUE);
            c10--;

        }
    }
    void set(int n) {
        char buff[100];
        wnum = n;
        sprintf_s(buff, 100, "images\\bullet%d.png", n);
        if ((mImgNo = LoadGraph(buff)) == -1) {
        }
        if (n == 10) {
            c10 = 0;
            if ((mImgNo2 = LoadGraph("images\\bullet10+.png")) == -1) {
            }
        }

    }
    void move() {
        mPosX += speedx;
        mPosY -= speedy;
        speedy -= gra;
    }
};
void playermove(PLAYER* gPlayer);
void enemymove(ENEMY gEnemy[], float mag);
void enemymove2(ENEMY2 gEnemy[], float mag);
void enemymove3(ENEMY3 gEnemy[], float mag);
void skymove(SKY sky[], float mag);
void hitcheck(PLAYER* gPlayer, ENEMY gEnemy[]);
void hitcheck2(PLAYER* gPlayer, ENEMY2 gEnemy[]);
void hitcheck3(PLAYER* gPlayer, ENEMY3 gEnemy[]);
void atack(PLAYER* gPlayer, ENEMY gEnemy[], ENEMY2 gEnemy2[], ENEMY3 gEnemy3[], WEP* wep, BULLET* bullet);
void bulletset(BULLET* bullet);
void scoreup(int* score, float* mag);
void dnum(int x, int y, int num);
int getitem();
void saved();
void loadd();