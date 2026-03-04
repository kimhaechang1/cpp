#include <iostream>
#include <cassert>

// 전방 선언 (Slime 클래스 안에서 SlimeSpawner를 friend로 쓰기 위함)
class SlimeSpawner;

class Slime {
    // TODO: 아래 요구사항을 만족하도록 클래스를 완성하세요.

    // 1. [변수명: totalCount] 
    // 목적: 만들어진 "모든" 슬라임의 개수를 누적해서 추적하는 변수 (객체 단위가 아님)
    // 조건: private에 숨길 것
private:
    //static int inline totalCount = 0;
    static int totalCount;

    // 2. 클래스 생성자
    // 목적: 생성될 때마다 totalCount를 1 증가시킴.
    // 조건: 아무나 슬라임을 만들 수 없도록 생성자를 private으로 숨길 것!
    Slime() {
        totalCount++;
    }
    // 3. 클래스 소멸자
    // 목적: 객체가 소멸할 때마다 totalCount를 1 감소시킴.
    // 조건: public이어야 객체가 자연스럽게 사라질 수 있음.

    // 4. friend 선언
    // 목적: 'SlimeSpawner'라는 다른 클래스에게는 내 private 금고(생성자)를 열어준다!
    friend class SlimeSpawner;

public:
    // 5. [함수명: GetTotalCount]
    // 목적: 슬라임이 한 마리도 없어도(객체가 없어도) 외부에서 현재 마릿수를 조회할 수 있게 하는 함수
    ~Slime() {
        totalCount--;
    }

    static int GetTotalCount() {
        return totalCount;
    }
};

// 6. [static 변수 외부 초기화]
// 목적: Slime 클래스의 totalCount 메모리 공간을 잡아주고 0으로 초기화할 것.
int Slime::totalCount = 0;


class SlimeSpawner {
public:
    // 7. [함수명: CreateSlime] 
    // 목적: 새로운 Slime을 생성하여 반환(Return)해주는 팩토리 함수.
    // 힌트: 내부에서 return Slime(); 형태로 만들어서 주면 됩니다.
    Slime CreateSlime() {
        return Slime();
    }
};


// ===============================================
// 🚨 아래 main 함수는 절대 코드 수정 금지! 🚨
// ===============================================
int main() {
    // 1. 시작할 때는 슬라임이 0마리여야 함.
    assert(Slime::GetTotalCount() == 0);

    SlimeSpawner spawner;

    // 블록 스코프 (이 중괄호 안에서 만들어진 변수는 괄호가 끝나면 소멸합니다)
    {
        // 2. 스패너를 통해서만 합법적으로 슬라임을 2마리 생성
        Slime s1 = spawner.CreateSlime();
        Slime s2 = spawner.CreateSlime();

        // 3. 두 마리가 생성되었으므로, static 공유 변수는 2가 되어야함.
        assert(Slime::GetTotalCount() == 2);
    }
    // 4. 스코프가 끝났으므로 s1, s2의 소멸자가 호출되면서 개수가 감소했을 것임.

    // 5. 모두 죽었으므로 다시 마릿수는 0이 맞아야 함!
    assert(Slime::GetTotalCount() == 0);

    std::cout << "[SUCCESS] static 멤버와 friend 팩토리 패턴 디버깅 성공!" << std::endl;
    return 0;
}
