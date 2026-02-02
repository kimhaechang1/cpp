#include <iostream>
#include <string>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

using namespace std;

// =================================================================================
// [1] 열거형(Enum) 정의
// ---------------------------------------------------------------------------------
// [Spec] Name: Element
// [Spec] Values: FIRE, WATER, EARTH
// =================================================================================
enum Element {
    FIRE, WATER, EARTH
};

// =================================================================================
// [2] 구조체(Struct) 정의
// ---------------------------------------------------------------------------------
// [Spec] Name: Stats
// [Spec] Members: 
// - string name
// - int hp, maxHp, attackPower
// - Element element
// =================================================================================
struct Stats {
    string name;
    int hp;
    int maxHp;
    int attackPower;
    Element element;
};


// =================================================================================
// [3] 함수 원형 선언 (Function Prototypes)
// ---------------------------------------------------------------------------------
// [Spec] Create initialized Player/Monster (No args -> Returns Stats)
Stats CreatePlayer();
Stats CreateMonster();

// [Spec] Attack logic: Reduce defender's HP by attacker's power
// Params: attacker (modifiability?), defender (modifiability?)
void Attack(Stats& attacker, Stats& defender);

// [Spec] Check if character is dead (HP <= 0)
// Params: character (modifiability?) -> Returns bool
bool IsDead(Stats& player);

// [Spec] Print current status of both
// Params: player, monster (modifiability?) -> Returns void
void PrintBattleInfo(Stats& player, Stats& monster);


int main() {
    srand((unsigned int)time(NULL));

    cout << "[TEXT RPG BATTLE SYSTEM START]" << endl;

    // 1. 캐릭터 생성
    Stats player; 
    Stats monster; 

    // (임시 코드: 나중에 Create 함수 내부로 이동 추천)
    player.name = "Hero";
    player.hp = 100;
    player.maxHp = 100;
    player.attackPower = 15;
    player.element = FIRE; 

    monster.name = "Orc";
    monster.hp = 80;
    monster.maxHp = 80;
    monster.attackPower = 10;
    monster.element = EARTH; 

    
    // 2. 전투 루프
    int turn = 1;
    while (true) {
        cout << "\n================ [TURN " << turn << "] ================" << endl;
        
        PrintBattleInfo(player, monster);

        cout << "[Player Attack]" << endl;
        Attack(player, monster);

        if (IsDead(monster)) {
            cout << "\n[VICTORY] Monster Defeated!" << endl;
            break;
        }

        cout << "[Monster Counter Attack]" << endl;
        Attack(monster, player);

        if (IsDead(player)) {
            cout << "\n[DEFEAT] Player Defeated..." << endl;
            break;
        }

        turn++;
    }

    return 0;
}

// =================================================================================
// [4] 함수 구현 (Function Implementations)
// =================================================================================

Stats CreatePlayer() {
    return Stats(); // TODO: Implement initialization inside here
}

Stats CreateMonster() {
    return Stats(); // TODO: Implement initialization inside here
}

void Attack(Stats& attacker, Stats& defender) {
    defender.hp = defender.hp - attacker.attackPower;
    if (defender.hp <= 0) {
        defender.hp = 0;
    }
}

bool IsDead(Stats& player) {
    return player.hp == 0;
}

void PrintBattleInfo(Stats& player, Stats& monster) {
    // Note: 'element' prints as integer (0, 1, 2)
    cout << "Player[name=" << player.name << ", hp=" << player.hp << "/" << player.maxHp 
         << ", Atk=" << player.attackPower << ", Elm=" << player.element << "]" << endl;
    cout << "Monster[name=" << monster.name << ", hp=" << monster.hp << "/" << monster.maxHp 
         << ", Atk=" << monster.attackPower << ", Elm=" << monster.element << "]" << endl;
}