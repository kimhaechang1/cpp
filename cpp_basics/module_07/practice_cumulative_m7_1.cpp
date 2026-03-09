#include <iostream>
#include <string>
#include <cassert>

// ====================================================================
// [ 누적 실습 (Cumulative Practice) - Module 1 ~ 7 ]
// ====================================================================

// [지시사항]
// 1. 네임스페이스 'Game'
//    - 목적: 아래의 모든 구성 요소를 감싸는 게임 전용 공간

// 2. 열거형 'UnitType'
//    - 목적: 게임 유닛의 타입 분류
//    - 구성: Normal, BossUnit (클래스 이름 Boss와의 충돌 방지!)

// 3. 부모 클래스 'Unit'
//    - 목적: 모든 유닛의 기반 정보
//    - 속성: 
//        > type (유닛 타입, 개인 금고처럼 숨김)
//        > hp, maxHp (자식에게만 열어주는 가업 비법서 형태로 은닉)
//    - 행동:
//        > 생성자: type, hp, maxHp 를 초기화
//        > GetType: type 값을 반환

// 4. 자식 클래스 'Boss'
//    - 목적: Unit을 '공개적'으로 상속받는 보스 몬스터
//    - 속성:
//        > '동적 할당 변수': 쫄다구들의 체력을 담을 배열 (minionHps)
//        > minionCount (쫄다구 수)
//    - 필수 행동:
//        > 생성자: hp, maxHp, 쫄다구 수를 인자로 받음. (부모의 type은 Boss로 고정하여 넘김).
//                  쫄다구 배열을 동적으로 할당하고, 제어문(반복문)을 통해 모든 쫄다구의 체력을 100으로 초기화.
//        > 생명주기 관리: 깊은 복사 생성자, 대입 연산자, 소멸자 (메모리 누수 방지).
//        > ApplyBuff: 매개변수로 '정수형 버프 수치'를 받음. (단, 이 수치 변수는 Call by Reference로 넘겨받을 것).
//                     보스의 hp를 버프 수치만큼 증가시키지만, maxHp를 초과할 수 없음 (조건/분기 로직 사용).
//                     회복이 끝난 후, 전달받은 원본 포션(버프 변수)의 값은 0으로 소진시킴.
//        > GetBossInfo: "[Boss] HP: (현재hp)/(최대hp)" 형태의 문자열(std::string)을 생성하여 반환. (std::to_string 활용)
//        > 교류의 장: 아래의 전역 함수 `CalculateTotalHp`가 이 클래스의 숨겨진 정보에 접근할 수 있도록 친밀권한(friend) 부여.

// 5. 전역 함수 'CalculateTotalHp'
//    - 목적: 보스가 거느린 쫄다구들의 전체 체력 합계 계산
//    - 동작: Boss 객체의 상수를 참조로 받아(수정 방지), 내부의 쫄다구 배열 메모리를 직접 순회하며 체력 총합을 반환.

// =============== [ 여 기 에  코 드 를  작 성 하 세 요 ] ===============
namespace Game{
    enum UnitType {
        Normal, BossUnit
    };
    
    // [현업 리뷰 반영 3] 전방 선언 (Forward Declaration)
    // 컴파일러에게 미리 "이름"을 알려주어 friend 호출 시 경고(Warning)가 발생하지 않도록 조치.
    class Boss;
    int CalculateTotalHp(const Boss& boss);

    class Unit {
    private:
        UnitType type;
    protected:
        int hp;
        int maxHp;
    public:
        Unit(UnitType type, int hp, int maxHp): type(type), hp(hp), maxHp(maxHp) {

        }
        // [현업 리뷰 반영] const 객체(아까 수정한 const Boss&)에서도 호출될 수 있도록 
        // 값을 바꾸지 않는 함수에는 무조건 const를 끝에 붙여줍니다!
        UnitType GetType() const {
            return this->type;
        }
    };

    class Boss : public Unit {
    private:
        int* minionHps;
        int minionCount;
    public:
        Boss(int hp, int maxHp, int minionCount) : Unit{UnitType::BossUnit, hp, maxHp} {
            this->minionCount = minionCount;
            minionHps = new int[this->minionCount];
            for (int i = 0; i < this->minionCount; i++) {
                minionHps[i] = 100;
            }
        }
        // [현업 리뷰 반영 1] 복사 생성체의 매개변수는 항상 `const 참조`를 사용합니다!
        // (원본 객체를 실수로 변경하는 것을 방지하기 위함)
        Boss(const Boss& other) : Unit{ other.GetType() , other.hp, other.maxHp } {
            std::cout << "복사 생성자 호출" << std::endl;
            this->minionCount = other.minionCount;
            minionHps = new int[this->minionCount];
            for (int i = 0; i < this->minionCount; i++) {
                minionHps[i] = other.minionHps[i];
            }
        }
        
        // [현업 리뷰 반영 1 & 2] 대입 연산자 역시 `const 참조`를 사용하고, 자가 대입을 방어합니다.
        Boss& operator=(const Boss& other) {
            // [자가 대입(Self-Assignment) 방어]
            // 만약 나 자신(this)의 메모리 주소와 넘어온 원본(&other)의 메모리 주소가 같다면?
            // "이미 내 정보인데 굳이 지배하고 덮어씌울 필요가 없다!"라는 뜻이므로 바로 종료.
            // (이 코드가 없다면 아래 delete[] 에서 자기 자신의 메모리를 날려버려 대참사가 일어납니다!)
            if (this == &other) {
                return *this;
            }

            if (this->minionHps != nullptr) {
                delete[] this->minionHps;
            }
            this->hp = other.hp;
            this->maxHp = other.maxHp;
            this->minionCount = other.minionCount;
            this->minionHps = new int[this->minionCount];
            for (int i = 0; i < this->minionCount; i++) {
                this->minionHps[i] = other.minionHps[i];
            }

            return *this;
        }
        ~Boss() {
            if (this->minionHps != nullptr) {
                delete[] this->minionHps;
            }
        }
        void ApplyBuff(int& buff) {
            hp += buff;
            if (hp > maxHp) {
                hp = maxHp;
            }
            buff = 0;
        }
        std::string GetBossInfo() {
            return "[Boss] HP: " + std::to_string(this->hp) + "/" + std::to_string(this->maxHp);
        }
        friend int CalculateTotalHp(const Game::Boss& boss);
        
    };
}

int Game::CalculateTotalHp(const Game::Boss& boss){
    int sum = 0;
    for (int i = 0; i < boss.minionCount; i++) {
        sum += boss.minionHps[i];
    }
    return sum;
}

// ====================================================================

int main() {
    // ----------------------------------------------------------------
    // [ TDD 검증 코드 ] (절대 수정하지 마세요!)
    // ----------------------------------------------------------------
    using namespace Game;
    
    // 1. 객체 생성 및 상속, 초기화 검증
    Boss bossA(500, 1000, 5); // hp: 500, maxHp: 1000, 쫄다구 5마리

    assert(bossA.GetType() == UnitType::BossUnit);
    assert(bossA.GetBossInfo() == "[Boss] HP: 500/1000");

    // 2. 동적 할당 및 수학산술 기반 누적 합산 검증
    int totalHps = CalculateTotalHp(bossA);
    //std::cout << totalHps << std::endl;
    assert(totalHps == 500); // 100 * 5

    // 3. Call by Reference 검증 (버프 포션)
    int healPotion = 600;
    bossA.ApplyBuff(healPotion);

    // 500+600 이지만 최대체력(1000)을 넘을 수 없음
    assert(bossA.GetBossInfo() == "[Boss] HP: 1000/1000");

    // 참조로 넘겨진 원본 포션은 0이 되어야 함
    assert(healPotion == 0);

    // 4. Rule of 3/5/0 (복사 및 대입 - 메모리 분리) 검증
    Boss bossB = bossA; // 깊은 복사 생성자
    Boss bossC(100, 100, 1);
    bossC = bossA;      // 대입 연산자

    bossA.ApplyBuff(healPotion); // 포션이 0이므로 변화는 없음

    // * 메모리 해제 오류나 값 덮어씌움 현상(Double Free)이 발생하지 않고,
    // * 프로그램이 정상 종료되면 메모리 관리에 성공한 것입니다.

    std::cout << "[SUCCESS] 누적 복습 실습 완벽 통과! C++ 기본기가 매우 탄탄합니다!\n";
    return 0;
}
