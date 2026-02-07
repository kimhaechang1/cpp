#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Monster {
    int hp;
    string name;

    // 1. 기본 생성자
    Monster(int h, string n) : hp(h), name(n) {
        cout << "[생성] " << name << " (HP: " << hp << ")" << endl;
    }

    // 2. 복사 생성자
    Monster(const Monster& other) : hp(other.hp), name(other.name) {
        cout << "[복사] " << name << " (원본 -> 사본)" << endl;
    }

    // 3. 이동 생성자 (move semantics)
    Monster(Monster&& other) noexcept : hp(other.hp), name(move(other.name)) {
        cout << "[이동] " << name << " (이사 완료)" << endl;
    }

    // 4. 소멸자
    ~Monster() {
        if (name != "") {
             cout << "[소멸] " << name << " (Bye)" << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    vector<Monster> v;
    v.reserve(10); // 벡터 크기 확장으로 인한 재복사를 방지 (순수 push vs emplace 비교를 위해)

    cout << "\n=== 1. push_back 테스트 ===" << endl;
    // 임시 객체 생성 -> 벡터로 이동(Move) -> 임시 객체 소멸
    v.push_back(Monster(100, "Slime_Push")); 

    cout << "\n=== 2. emplace_back 테스트 ===" << endl;
    // 벡터 내부에서 바로 생성 (복사/이동 X, 소멸 X)
    v.emplace_back(200, "Orc_Emplace");

    cout << "\n=== 프로그램 종료 ===" << endl;
    return 0;
}
