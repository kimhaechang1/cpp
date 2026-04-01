#include <iostream>
#include <string>
#include <map>
#include <cassert>

// =========================================================================
// [실습] C++17 Structured Binding (구조 분해 할당)
// 
// 🚨 엄격한 백지 정책 적용 🚨
// 1. auto 배열에 어떤 한정자(const, &)를 붙일지 신중히 결정하세요.
// 2. map의 요소 순회 시 "원본 수정"이 필요한 상황과 "읽기 전용"인 상황의 차이를 코드로 증명합니다.
// =========================================================================

struct Player {
    std::string name;
    float hp;
};

// [TODO 1] 임의의 Player 객체를 하나 반환하는 'GetDummyPlayer' 함수를 만드세요.
// -> 여기에 작성
Player GetDummyPlayer() {
    return Player{"kim", 10};
}


int main() {
    std::cout << "[ Structured Binding Practice Start ]\n\n";

    // ---------------------------------------------------------
    // 1. 구조체 분해 실습
    // ---------------------------------------------------------
    // [TODO 2] GetDummyPlayer()를 호출하고, 반환된 구조체 멤버들을 구조 분해 할당으로 받으세요.
    // 이때 원본 데이터는 필요 없고 "가벼운 복사본"으로 이름(playerName)과 체력(playerHp)을 받습니다.
    // -> 여기에 작성
    auto [playerName, playerHp] = GetDummyPlayer();
     assert(playerName != "");
     assert(playerHp > 0.0f);
    std::cout << "1. Struct Binding OK!\n";

    // ---------------------------------------------------------
    // 2. map의 insert 반환값 분해 (std::pair 분해)
    // ---------------------------------------------------------
    std::map<int, Player> serverDB;

    // [TODO 3] serverDB에 임의의 유저를 insert 하세요. 
    // insert()는 "성공 여부"와 "삽입된 위치"의 정보를 담은 std::pair를 반환합니다.
    // 구조 분해 할당을 통해 삽입 성공 여부(success)를 바로 직관적인 변수명으로 받아 이 assert를 통과시키세요!
    // -> 여기에 작성
    auto [iterator, success] = serverDB.insert({ 1000, Player{ "N1", 10 } });

     assert(success == true);
    std::cout << "2. Map Insert Return Binding OK!\n";

    // ---------------------------------------------------------
    // 3. 상수성 전파(Const Propagation) 실습: 원본 조작
    // ---------------------------------------------------------
    serverDB.insert({ 2, {"Knight", 100.0f} });
    serverDB.insert({ 3, {"Mage", 80.0f} });

    // [TODO 4] 서버 이벤트 발생! DB에 있는 모든 유저의 체력(hp)을 2배로 뻥튀기해야 합니다.
    // (힌트: 아까 토론했던 내용입니다. string(Player)을 어떻게 수정 권한을 열어줄지 고민하세요)
    // -> 여기에 작성
    for (auto& [id, player] : serverDB) {
        player.hp *= 2;
    }

    // [검증]
     assert(serverDB[2].hp == 200.0f);
     assert(serverDB[3].hp == 160.0f);
    std::cout << "3. Mutable Binding (Reference) Test OK!\n";

    std::cout << "\n🌟 모든 검증을 통과했습니다! 🌟\n";
    return 0;
}
