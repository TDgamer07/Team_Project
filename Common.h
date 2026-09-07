#pragma once
#include <string>
#include <vector>
#include <fstream>

//画面遷移ID
enum class SceneID {
    None,          //画面切り替えなし
    Home,          //ホーム画面
    MiniGame,      //ミニゲーム画面
    Timer,         //タイマー機能画面
    ActiveRecall,  //アクティブリコール画面
    Shop,          //ショップ画面
    Title,         //タイトル画面
    scar
};

//クエスト
enum class QuestType {
    Daily,
    Weekly
};

//クエスト構造体
struct Quest {
    std::string title;
    bool isCompleted;
    QuestType type;
    int rewardXP;
    int rewardCoins;

    Quest(const std::string& t, QuestType tp, int xp, int coins)
        : title(t), isCompleted(false), type(tp), rewardXP(xp), rewardCoins(coins) {
    }
};

//武器構造体
struct Weapon {
    std::string name; // 武器名
    int price;        // 価格
    bool isOwned;     // ショップで購入済み
    bool isEquipped;  // 装備しているか

    Weapon(std::string n, int pr, bool owned, bool eq)
        : name(n), price(pr), isOwned(owned), isEquipped(eq) {
    }
};

//アクティブリコール1件分のデータ構造
struct AR {
    std::string date;     //日時
    std::string content;  //内容
    AR(const std::string& d, const std::string& c) : date(d), content(c) {}   //コンストラクタ
};

//プレイヤーのステータス管理クラス
class Player {
public:
    int xp = 0;
    int level = 1;
    int coins = 0;
    int completedCount = 0;
    //経験値の合計
    int MAXxp = 0;

    int HomeHandle;

    int mainBGM;

    std::vector<AR> studyMemos;
    std::vector<Quest> dailyQuests;
    std::vector<Quest> weeklyQuests;

    std::vector<Weapon> weapons;
    //コンストラクタ
    Player() {
        weapons.push_back(Weapon("剣", 10, false, false)); 
        weapons.push_back(Weapon("杖", 100, false, false));
        weapons.push_back(Weapon("鎌", 300, false, false));
        weapons.push_back(Weapon("斧", 500, false, false));
        weapons.push_back(Weapon("槍", 1000, false, false));
        weapons.push_back(Weapon("弓", 1500, false, false));
        weapons.push_back(Weapon("拳銃", 2000, false, false));
        weapons.push_back(Weapon("スナイパー", 3000, false, false));
        weapons.push_back(Weapon("ダブルブレードハサミ", 5000, false, false));
        weapons.push_back(Weapon("尾張四頭龍", 10000, false, false));
        weapons.push_back(Weapon("貫槍", 15000, false, false));

        HomeHandle = LoadGraph("タスク\\ホームへ戻るボタン.png");
        mainBGM = LoadSoundMem("サウンド\\ゆらりくらり.mp3");
    }

    ~Player() {
        DeleteGraph(HomeHandle);
        StopSoundMem(mainBGM);
    }

    //ショップで購入した武器の数
    int GetWeaponCnt() const {
        int cnt = 0;
        for (const auto& w : weapons) {
            if (w.isOwned) {
                cnt++;
            }
        }
        return cnt;
    }

    //報酬(経験値とコイン)
    void AddReward(int addXp, int addCoins) {
        xp += addXp;
        coins += addCoins;
        MAXxp += addXp;
        while (xp >= 100) {
            level++;
            xp -= 100;
        }
    }

    //データをセーブする為のテキストファイルを作成
    void Save() {
        std::ofstream ofs("SaveData.txt");
        if (!ofs)return;
        // ステータスとアイテムを保存
        ofs  << MAXxp << "\n" << level << "\n" << coins << "\n" << completedCount << "\n";
        // メモの数と、メモの内容を保存
        ofs << studyMemos.size() << "\n";
        for (const auto& memo : studyMemos) {
            ofs << memo.date << "\n";
            ofs << memo.content << "\n";
        }
        //デイリークエストの保存
        ofs << dailyQuests.size() << "\n";
        for (const auto& q : dailyQuests) {
            ofs << q.title << "\n";
        }
        //ウィークリークエストの保存
        ofs << weeklyQuests.size() << "\n";
        for (const auto& q : weeklyQuests) {
            ofs << q.title << "\n";
        }
        //武器の所持・装備状態をセーブ
        ofs << weapons.size() << "\n";
        for (const auto& w : weapons) {
            ofs << w.isOwned << " " << w.isEquipped << "\n";
        }

    }

    //SaveData.txtから読み込む関数
    void Load() {
        std::ifstream ifs("SaveData.txt");
        if (!ifs)return;
        ifs  >> MAXxp >> level >> coins >> completedCount;
        size_t memoSize = 0;
        ifs >> memoSize;
        std::string dummy;
        // 改行を読み飛ばす
        std::getline(ifs, dummy); 

        studyMemos.clear();
        for (size_t i = 0; i < memoSize; i++) {
            std::string d, c;
            std::getline(ifs, d); // 日付を読み込む
            std::getline(ifs, c); // 内容を読み込む
            studyMemos.push_back(AR(d, c));
        }
        //デイリークエストを読み込む
        size_t dailySize = 0;
        if (ifs >> dailySize) { 
            std::getline(ifs, dummy);
            dailyQuests.clear();
            for (size_t i = 0; i < dailySize; i++) {
                std::string t;
                std::getline(ifs, t);
                dailyQuests.push_back(Quest(t, QuestType::Daily, 10, 5));
            }
        }
        //ウィークリークエストを読み込む
        size_t weeklySize = 0;
        if (ifs >> weeklySize) {
            std::getline(ifs, dummy);
            weeklyQuests.clear();
            for (size_t i = 0; i < weeklySize; i++) {
                std::string t;
                std::getline(ifs, t);
                weeklyQuests.push_back(Quest(t, QuestType::Weekly, 50, 30));
            }
        }
        //武器の所持・装備状態を読み込む
        size_t weaponSize = 0;
        if (ifs >> weaponSize) {
            for (size_t i = 0; i < weaponSize && i < weapons.size(); i++) {
                ifs >> weapons[i].isOwned >> weapons[i].isEquipped;
            }
        }
    }
};

// 座標の中にマウスがあるか判定する関数
inline bool IsHit(int mx, int my, int x1, int y1, int x2, int y2) {
    return (mx >= x1 && mx <= x2 && my >= y1 && my <= y2);
}