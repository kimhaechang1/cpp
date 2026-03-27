#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <cassert>

// ==============================================================================
// 연습문제 1: 동적 배열 (std::vector)
// [요구사항]
// 1. 함수 이름: RecordDamage
// 2. 파라미터: 비어있는 데미지 기록부(std::vector<int>)를 인자로 받아야 합니다.
// 3. 기능: 해당 기록부의 맨 뒤쪽(끝부분)에 3, 7, 10 이라는 숫자를 차례대로 연속해서 밀어 넣으세요.
// 4. 힌트: 호출이 끝난 뒤 원본 기록부 `main()` 쪽 데이터가 보존되어 있으려면 
//           매개변수를 어떻게(과거 모듈 3단원: 주소 vs 참조) 전달해야 할지 떠올려보세요!
// ==============================================================================

// 코드 작성
void RecordDamage(std::vector<int>& record) {
    record.push_back(3);
    record.push_back(7);
    record.push_back(10);
}



// ==============================================================================
// 연습문제 2: 잡동사니 주머니 (std::pair)
// [요구사항]
// 1. 함수 이름: GetSpawnLocation
// 2. 기능: 몬스터가 소환될 x 좌표(10)와 y 좌표(20)를 단 하나의 덩어리로 예쁘게 묶어서, 
//          이 함수를 호출한 쪽으로 1개의 데이터 덩어리(std::pair) 타입으로 반환합니다.
// 3. [Blank Slate]: 반환 타입 구조를 정확히 유추해 보세요.
// ==============================================================================

// 코드 작성
std::pair<int, int>* GetSpawnLocation() {
    // auto pp = std::pair<int, int>(10, 20);
    // auto* rf = &pp;
    // return rf;
    // 위의 코드는 댕글링 포인터를 일으킬 수 있다.
    // 왜냐하면 스택프레임이 종료되면서 해당 스택메모리가 다 날라가기 때문이다.
    // 그러면 pp 의 주소에 있는 값은 존재하지 않게 되고 rf 도 이상한값을 가져오려고 할수 있다.

    auto* pp = new std::pair<int, int>{10, 20};
    return pp;
}




// ==============================================================================
// 연습문제 3: 초고속 아이템 도감 (std::unordered_map)
// [요구사항]
// 1. 함수 이름: AddItemDictionary
// 2. 파라미터: 아이템 ID(int)를 열쇠(Key)로, 아이템 이름(string)을 값(Value)으로 갖는 
//             비어있는 '초고속 맵' 하나를 인자로 받습니다.
// 3. 기능: 
//    - Key `1` 번 위치에 `"Red Potion"` 문자열을 추가합니다.
//    - Key `99` 번 위치에 `"Legendary Sword"` 문자열을 추가합니다.
// 4. [Blank Slate]: 연습문제 1과 마찬가지로 원본 데이터를 조작해야 하므로 참조 지시자를 놓치지 마세요!
// ==============================================================================

// 코드 작성
void AddItemDictionary(std::unordered_map<int, std::string>& umap) {
    umap[1] = "Red Potion";
    umap[99] = "Legendary Sword";
}


// ---------------- C++ 테스트 코드 (Main) ----------------
// 여기 아래의 테스트(Main) 코드는 절대 수정하지 마세요!
// --------------------------------------------------------
int main() {
    std::cout << "========= 테스트 시작 =========" << std::endl;

    // [테스트 1: RecordDamage]
    std::vector<int> myLogs;
    RecordDamage(myLogs);
    assert(myLogs.size() == 3 && "RecordDamage 실패: 3개가 기록되어야 합니다.");
    assert(myLogs[0] == 3 && myLogs[1] == 7 && myLogs[2] == 10 && "RecordDamage 실패: 데이터 순서가 틀립니다.");
    std::cout << "[PASS] 테스트 1: std::vector 완료" << std::endl;

    // [테스트 2: GetSpawnLocation]
    auto loc = GetSpawnLocation();
    std::pair<int, int>* ptr = GetSpawnLocation();
    assert(loc->first == 10 && loc->second == 20 && "GetSpawnLocation 실패: 좌표 값이 틀립니다.");
    std::cout << "[PASS] 테스트 2: std::pair 완료" << std::endl;

    // [테스트 3: AddItemDictionary]
    std::unordered_map<int, std::string> dex;
    AddItemDictionary(dex);
    assert(dex.size() == 2 && "AddItemDictionary 실패: 2개의 데이터가 들어있어야 합니다.");
    assert(dex[1] == "Red Potion" && "AddItemDictionary 실패: 1번 아이템 매칭 실패");
    assert(dex[99] == "Legendary Sword" && "AddItemDictionary 실패: 99번 아이템 매칭 실패");
    std::cout << "[PASS] 테스트 3: std::unordered_map 완료" << std::endl;

    std::cout << "========= 모든 컨테이너 기초 훈련 완벽하게 통과! =========" << std::endl;
    delete loc;
    delete ptr;
    return 0; // 정상 종료
}
