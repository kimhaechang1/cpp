#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <cassert>
#include<format>

// ===========================================================================
// [Q] 오류 원천 차단을 위해, 상태(State)들을 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요.
// ===========================================================================
// TODO: JobClass 선언 - 항목: WARRIOR, ARCHER, MAGE
enum JobClass {
    WARRIOR, ARCHER, MAGE
};


// ===========================================================================
// [Q] 서로 연관된 데이터를 하나의 이름 아래 묶어 새로운 타입을 만드세요.
// [Q] 외부 노출은 철저히 막되, 오직 나를 상속받은 자식 클래스에게만 데이터 접근 권한을 물려주어야 합니다.
// ===========================================================================
// TODO: Hero 클래스 선언
// - 멤버 변수:
//   - 이름: name    (문자열)
//   - 이름: power   (정수, 공격력)
//   - 이름: jobClass (위에서 만든 JobClass 타입)
//
// - 생성자 (매개변수: name, power, jobClass)
// - Getter: GetName, GetPower, GetJobClass
//
// [Q] 자료구조의 겉모습이 배열인지 트리인지 무관하게, 내부 요소를 가리키고 전진할 수 있는
//     보편적 순회용 추상화 포인터를 사용하여 모든 Hero를 출력하는 함수를 선언하세요.
// - 함수명: PrintAllHeroes (매개변수: Hero 컬렉션)
//
// [Q] 두 객체의 '크기'를 비교할 수 있어야 STL 정렬 알고리즘에 합격할 수 있습니다.
//     비교 기준: power 오름차순
// - operator< 선언
class Hero {
private:
    std::string name;
    int power;
    JobClass jobClass;
public:
    Hero(std::string name, int power, JobClass jobClass) : name(name), power(power), jobClass(jobClass){

    }
    std::string GetName() const {
        return this->name;
    }
    int GetPower() const {
        return this->power;
    }
    JobClass GetJobClass() const {
        return this->jobClass;
    }
    std::string GetJobClassStr() {
        switch (this->jobClass) {
        case JobClass::ARCHER:
            return "ARCHER";
        case JobClass::MAGE:
            return "MAGE";
        default:
            return "WARRIOR";
        }
    }
    bool operator<(const Hero& other) const {
        return this->power < other.power;
    }
};
void PrintAllHeroes(std::vector<Hero>& heros) {
    for (auto it = heros.begin(); it != heros.end(); ++it) {
        it->GetJobClassStr();
        it->GetName();
        std::cout << std::format("[{}] NAME={} POWER={}", it->GetJobClassStr(), it->GetName(), it->GetPower()) << std::endl;
    }
}

// ===========================================================================
// [Q] 타입 T가 '공격력(power) 멤버'를 가지고 있어야 한다는 제약 조건을 정의하세요.
// ===========================================================================
// TODO: HasPower Concept 선언
template<typename T>
concept HasPower = requires(T v) {
    v.GetPower();
};


// ===========================================================================
// [Q] 컬렉션의 크기에 구애받지 않고 처음부터 끝까지 모든 원소를 안전하게 훑는 순회 방식을 쓰되,
//     기존 데이터 원본을 건드리지 않고 원하는 기호로 데이터의 흐름을 연결하는 방식을 사용하세요.
// [Q] 유효한 타입임을 검증하는 제약 조건(Concept)을 적용하면서, 피아 식별(power >= threshold)
//     이 가능한 녀석만 골라, 이름만 뽑아내는 파이프라인 함수를 만드세요.
// ===========================================================================
// TODO: 함수 선언 - FilterStrongNames
// - 매개변수 1: Hero 컬렉션 (원본)
// - 매개변수 2: 기준 공격력 (정수)
// - 반환: 기준 이상의 Hero 이름(문자열) 목록
template<HasPower T>
std::vector<std::string> FilterStrongNames(std::vector<T>& entities, int threshold) {
    auto results =  entities
    | std::views::filter([&](T t) {
            return t.GetPower() >= threshold;
        })
    | std::views::transform([](T t) {
            return t.GetName();
        });

    // ranges 에 views 를 돌리면 괴상한 타입이 나오니까 vector 에 이터레이터 를 넣어서 변환해야한다.
    return std::vector<std::string>(results.begin(), results.end());
}

template<HasPower T>
auto fn = [](std::vector<T> ts) {
    std::ranges::sort(ts);
    return ts[ts.size() - 1];
};

int main() {
    // ===========================================================================
    // [TEST] TDD 테스트 구역 (수정 금지)
    // ===========================================================================

    std::vector<Hero> party = {
        {"Aragorn",  80, JobClass::WARRIOR},
        {"Legolas",  60, JobClass::ARCHER},
        {"Gandalf",  95, JobClass::MAGE},
        {"Frodo",    20, JobClass::WARRIOR},
        {"Gimli",    70, JobClass::WARRIOR},
    };

    // [TEST 1] 이터레이터 기반 전체 출력
    std::cout << "=== All Heroes ===" << std::endl;
    PrintAllHeroes(party);

    // [TEST 2] Ranges 파이프라인 - power 65 이상 Hero 이름 필터링
    auto strongNames = FilterStrongNames(party, 65);
    std::cout << "\n=== Strong Heroes (power >= 65) ===" << std::endl;
    for (const auto& name : strongNames) {
        std::cout << name << std::endl;
    }
    assert(strongNames.size() == 3); // Aragorn, Gandalf, Gimli

    // [TEST 3] Concepts 검증 - HasPower 가 적용된 함수로 power 확인
    // (std::ranges::max_element + HasPower 적용 람다 활용)

    /*
     * 💡 [Mentor's Note: std::totally_ordered 와 람다 활용]
     * 1. std::ranges::max_element 는 내부적으로 'std::totally_ordered' 라는 매우 엄격한 조건을 검사합니다.
     *    - 이 조건은 operator< 뿐만 아니라 ==, !=, >, <=, >= 모든 연산자가 완벽히 구현되어야 함을 의미합니다.
     * 2. 하지만 실무에서 모든 클래스에 이 많은 연산자를 다 구현하는 것은 번거롭고 실수할 확률이 높습니다.
     * 3. [해결책]: '비교 로직(람다)'을 직접 던져주면, 컴파일러는 클래스 자체의 완성도를 따지지 않고 
     *    오직 그 람다의 실행 결과(bool)만 보고 "오케이, 비교 가능하네!"라고 즉시 인정해 줍니다.
     * 4. 또한, 상황에 따라 '공격력 순'이 아닌 '이름 순'으로 뽑고 싶을 때도 클래스 수정 없이 유연하게 대처 가능합니다.
     */
    auto strongest = std::ranges::max_element(party, [](const Hero& a, const Hero& b) {
        return a.GetPower() < b.GetPower(); // strict_weak_order 만족
        });
    assert(strongest->GetPower() == 95);
    std::cout << "\n=== Strongest Hero ===" << std::endl;
    std::cout << strongest->GetName() << " (Power: " << strongest->GetPower() << ")" << std::endl;

    std::cout << "\n[SUCCESS] Cumulative Practice 8-3 Passed!" << std::endl;
    return 0;
}
