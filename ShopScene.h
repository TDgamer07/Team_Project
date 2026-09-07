#pragma once
#include "BaseScene.h"

//アクティブリコールクラス
class ShopScene : public BaseScene {
private:
	//-1なら通常のショップ画面。0～7ならその番号の武器の「購入確認画面」を表示
	int selectedWeaponIndex = -1;
	std::string message = ""; //画面右上に購入しましたを表示するため

	int currentDisplayIndex = 0;  //現在画面の真ん中に表示している武器の番号

	int SwordHandle;
	int CaneHandle;
	int SickleHandle;
	int AxeHandle;
	int SpearHandle;
	int BowHandle;
	int HandgunHandle;
	int SniperHandle;
	int DoubleBladeHandle;
	int Sickle2Handle;
	int Spear2Handle;
	int Shopframe2Handle;
	int ItemHandle;

public:
	ShopScene() {
		SwordHandle = LoadGraph("タスク\\アイテム剣.png");
		CaneHandle = LoadGraph("タスク\\杖.png");
		SickleHandle = LoadGraph("タスク\\鎌.png");
		AxeHandle = LoadGraph("タスク\\斧.png");
		SpearHandle = LoadGraph("タスク\\槍.png");
		BowHandle = LoadGraph("タスク\\弓.png");
		HandgunHandle = LoadGraph("タスク\\拳銃.png");
		SniperHandle = LoadGraph("タスク\\スナイパー.png");
		DoubleBladeHandle = LoadGraph("タスク\\ダブルブレードはさみ.png");
		Sickle2Handle = LoadGraph("タスク\\鎌2.png");
		Spear2Handle = LoadGraph("タスク\\槍2.png");
		Shopframe2Handle = LoadGraph("タスク\\しょっぷふれーむ.png");
		ItemHandle = LoadGraph("タスク\\あいてむふれーむ.png");
	}          
	~ShopScene() {
		DeleteGraph(SwordHandle);
		DeleteGraph(CaneHandle);
		DeleteGraph(SickleHandle);
		DeleteGraph(AxeHandle);
		DeleteGraph(SpearHandle);
		DeleteGraph(BowHandle);
		DeleteGraph(HandgunHandle);
		DeleteGraph(SniperHandle);
		DeleteGraph(DoubleBladeHandle);
		DeleteGraph(Sickle2Handle);
		DeleteGraph(Spear2Handle);
		DeleteGraph(Shopframe2Handle);
		DeleteGraph(ItemHandle);
	}         
	SceneID UpdateAndDraw(Player& player, bool leftClicked, int mouseX, int mouseY, int timer) override;
};

