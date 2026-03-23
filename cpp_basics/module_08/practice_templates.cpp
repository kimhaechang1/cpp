#include <iostream>
#include <string>
#include <cassert>
#include <format>

// ==============================================================================
//  연습문제 1: 함수 템플릿 (SwapValues)
// [요구사항]
// 1. 이름: SwapValues
// 2. 기능: 두 개의 데이터를 받아 서로의 보관된 '값'을 교환합니다.
// 3. 힌트: 함수가 끝난 후에도 내부 값이 실제로 바뀌어져 있으려면 
//           매개변수를 어떻게 전달(과거 Module 3 참조)해야 하는지 떠올려보세요!
// 4. [ Blank Slate 적용]: 반환 타입, 매개변수 타입, 템플릿 선언문은 전부 스스로 판단하여 작성해야 합니다.
// ==============================================================================

// 여기에 함수 템플릿을 작성하세요.
template<typename T>
void SwapValues(T& a, T& b) {
    T temp = b;
    b = a;
    a = temp;
}





// ==============================================================================
//  연습문제 2: 클래스 템플릿 (ItemSlot)
// [요구사항]
// 1. 이름: ItemSlot
// 2. 템플릿: 데이터 타입 1개를 미지수로 받습니다.
// 3. 멤버 변수 (private): 
//    - 아이템을 보관할 데이터 1개
// 4. 생성자: 
//    - 포인터 타입이 들어왔다고 가정하고, 기본적으로 메모리를 nullptr로 0 초기화합니다.
// 5. 메서드 (public):
//    - 아이템 설정 함수 (이름 자유, 외부 데이터를 안에 보관)
//    - 아이템 반환 함수 (이름 자유, 설정된 데이터를 외부로 반환)
// ==============================================================================

// 여기에 클래스 템플릿을 작성하세요.
template<typename T>
class ItemSlot {
private:
    T item;
public:
    ItemSlot() {
        item = nullptr;
    }
    void SetItem(T value) {
        item = value;
    }
    T GetItem() {
        return (this->item);
    }
};


// ==============================================================================
//  연습문제 3: 다중 템플릿 파라미터 (MakeDictionaryString)
// [요구사항]
// 1. 이름: MakeDictionaryString
// 2. 기능: 서로 다른 2개의 타입(Key, Value)을 파라미터로 받아서 
//          "[Key: X, Value: Y]" 형태의 std::string 문장 1개를 완성하여 반환합니다.
// 3. 힌트: Module 1에서 새롭게 배웠던 C++20 표준 `std::format`을 적극 활용하세요!
//          (원한다면 std::to_string 이나 std::stringstream 을 써도 무방합니다.)
// 4. [ Blank Slate 적용]: 2개의 템플릿 파라미터 선언 및 자료구조 작성은 자유.
// ==============================================================================

// 여기에 복수 템플릿 파라미터 함수를 작성하세요.
template<typename Key, typename Value>
std::string MakeDictionaryString(Key key, Value value) {
    return std::format("[Key: {}, Value: {}]", key, value);
}




// ---------------- C++ 테스트 코드 (Main) ----------------
// 여기 아래의 테스트(Main) 코드는 절대 수정하지 마세요!
// 에러가 나지 않고 "모든 테스트 통과!"가 출력되면 성공입니다.
// --------------------------------------------------------
int main() {
    std::cout << "========= 테스트 시작 =========" << std::endl;

    // [테스트 1: SwapValues]
    int a = 10;
    int b = 20;
    SwapValues(a, b);
    assert(a == 20 && b == 10 && "SwapValues(int) 실패!");

    std::string s1 = "Hello";
    std::string s2 = "World";
    SwapValues(s1, s2);
    assert(s1 == "World" && s2 == "Hello" && "SwapValues(string) 실패!");
    std::cout << "[PASS] 테스트 1: SwapValues 완료" << std::endl;

    // [테스트 2: ItemSlot]
    // 여기서 T 의 타입은 int* (즉, 포인터) 입니다.
    ItemSlot<int*> intPtrSlot;
    int* numTarget = new int(100);
    //std::cout << numTarget << std::endl;
    intPtrSlot.SetItem(numTarget);
    assert(*intPtrSlot.GetItem() == 100 && "ItemSlot<int*> 실패!");
    delete numTarget;

    // 여기서 T 의 타입은 std::string* (문자열 포인터) 입니다.
    ItemSlot<std::string*> strPtrSlot;
    std::string* strTarget = new std::string("Epic Sword");
    strPtrSlot.SetItem(strTarget);
    assert(*strPtrSlot.GetItem() == "Epic Sword" && "ItemSlot<std::string*> 실패!");
    delete strTarget;
    std::cout << "[PASS] 테스트 2: ItemSlot 완료" << std::endl;

    // [테스트 3: MakeDictionaryString]
    std::string dict1 = MakeDictionaryString<int, std::string>(1, "Goblin");
    std::cout << dict1 << std::endl;
    std::string dict2 = MakeDictionaryString<std::string, double>("Gold", 99.9);

    assert(dict1 == "[Key: 1, Value: Goblin]" && "MakeDictionaryString 1 실패!");
    assert(dict2 == "[Key: Gold, Value: 99.9]" && "MakeDictionaryString 2 실패!");
    std::cout << "[PASS] 테스트 3: MakeDictionaryString 완료 (다중 템플릿)" << std::endl;

    std::cout << "=========  모든 테스트 완벽하게 통과! =========" << std::endl;
    return 0; // 정상 종료
}
