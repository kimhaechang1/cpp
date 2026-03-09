#include <iostream>
#include <string>
#include <cassert>

// [지시사항]
// 1. 'Player' 클래스를 만드세요.
//    - 목적: 게임 안의 모든 플레이어 직업군의 공통된 기본 상태를 정의
//    - private 변수: std::string name (이름은 자식도 함부로 못 바꾸게 꽁꽁 숨김!)
//    - protected 변수: int hp, int mp (자식 클래스들은 스킬을 쓸 때 hp, mp를 자유롭게 깎을 수 있어야 함)
//    - public 함수: 생성자(초기화 리스트로 3개 변수 셋팅), std::string GetName() (이름을 반환), int GetHp(), int GetMp()
class Player {
private:
    std::string name;
protected:
    int hp;
    int mp;
public:
    Player(int hp, int mp, std::string name):hp(hp), mp(mp), name(name) {

    }
    std::string GetName() {
        return this->name;
    }
    int GetHp() {
        return this->hp;
    }
    int GetMp() {
        return this->mp;
    }
};

// 2. 'Mage' 클래스를 만드세요.
//    - 목적: Player를 'public'으로 상속받는 마법사
//    - public 함수 1: 생성자 (name, hp, mp를 받아서 부모에게 토스)
//    - public 함수 2: void CastSpell() 
//      -> 마법을 쓰면 부모님이 물려주신 mp(protected)를 20 깎습니다. (-20)
//      -> 단, 여기서 부모님의 name(private)을 직접 수정하려고 하면 어떻게 되는지 
//         주석으로 적어보고 직접 경험해보세요! (예: // name = "Gandalf";)

// =============== [ 여 기 에  코 드 를  작 성 하 세 요 ] ===============
class Mage : public Player {
public:
    Mage(std::string name, int hp, int mp) : Player{hp, mp, name} {

    }
    void CastSpell() {
        this->mp -= 20;
        // this->name = "Gandalf";
        // private member 에 대해 access 할 수 없음
        // 왜냐하면 애초에 부모에서 private 이고 자식에서 public 이면 범위 좁아지는것 없이 그대로 상속됨
    }
};



// ====================================================================

int main() {
    // -------------------------------------------------------------
    // [테스트] Mage 객체 생성 및 protected/private 접근 검증
    // -------------------------------------------------------------

    // 설명: 이름 "Merlin", 체력 100, 마나 50을 가진 마법사 생성
    Mage mage("Merlin", 100, 50);

    // 1. 부모의 public 함수들이 정상 작동하는지 확인
    assert(mage.GetName() == "Merlin");
    assert(mage.GetHp() == 100);
    assert(mage.GetMp() == 50);

    // 2. 자식(Mage)의 고유 행동(CastSpell)이 부모의 protected 리소스(mp)를 
    //    성공적으로 조작하는지 확인
    mage.CastSpell();
    assert(mage.GetMp() == 30); // 50에서 20이 깎였어야 함!

    // 3. (확인용) main 함수에서는 부모의 protected 변수에 직접 접근할 수 없어야 합니다!
    // 아래 코드를 주석 해제(Ctrl+/) 해보고 컴파일 에러(접근 불가)가 뜨는 것을 눈으로 확인 후, 
    // 다시 주석 처리해 주세요.
    // mage.mp = 9999; 

    std::cout << "[SUCCESS] 캡슐화와 protected 상속 접근 제어 테스트 완벽 통과!\n";
    std::cout << "자식은 부모의 재산(protected)을 다룰 수 있지만, 세상(main)은 접근할 수 없습니다.\n";

    return 0;
}
