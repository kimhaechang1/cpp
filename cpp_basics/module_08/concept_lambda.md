# 🧩 함수 객체(Functor)와 람다(Lambda) 함수: 로직을 부품처럼 던지기

## 1. 🎮 실무 프로젝트 상황 (Why Lambda?)

이전 단계에서 우리는 배열(vector)이든 트리(map)든 일관되게 훑고 지나갈 수 있는 `Iterator`를 배웠습니다.
이 강력한 무기를 바탕으로 C++ 표준 라이브러리(STL)는 `<algorithm>` 이라는 헤더 안에 엄청나게 빠르고 검증된 수십 개의 알고리즘 함수(Find, Sort, Count 등)를 미리 만들어 두었습니다.

> **개발자**: "플레이어의 인벤토리(`vector<Item>`)를 O(NlogN) 속도로 정렬하고 싶어! 내가 직접 퀵 정렬 짜긴 귀찮으니 STL의 `std::sort()`를 써야지."
> **컴파일러**: "잠깐, `Item` 객체를 줄 세우라는데 누가 더 크고 작은지 내가 어떻게 알아? 등급(Grade)순으로 할지, 이름(Name)순으로 할지 **네가 정렬 규칙 원리(로직)를 내게 넘겨줘야지!**"

그래서 등장한 개념이 **"함수 덩어리 자체를 변수처럼 다른 함수(알고리즘)의 매개변수로 집어던져 넣는 기술"** 입니다.
- 과거 (C++98): 함수 객체 (Functor / `operator()`)
- **현재 (C++11~25 트렌드): 이름 없는 즉석 함수 (Lambda, 람다)**

---

## 2. 🏛️ 함수 객체 (Functor): 람다의 조상

람다를 완벽히 이해하려면 이것이 하드웨어적으로 무엇인지 알아야 합니다.
클래스/구조체 내부에 **괄호 연산자 `operator()` 를 오버로딩**해서, 마치 **객체를 함수처럼 호출**할 수 있게 만든 것을 Functor라고 부릅니다.

```cpp
#include <iostream>
#include <vector>
#include <algorithm> // std::sort

// 1. 함수 객체 (Functor) 정의
struct GradeSorter {
    // 괄호 연산자를 오버로딩!
    bool operator()(int a, int b) const {
        return a > b; // 내림차순 정렬 규칙
    }
};

void LegacyWay() {
    std::vector<int> levels = { 10, 50, 30, 99 };
    
    // std::sort에게 반복자 구간(begin~end)과 함께, 
    // 정렬의 규칙을 담은 '객체 인스턴스(GradeSorter())'를 함께 던집니다.
    std::sort(levels.begin(), levels.end(), GradeSorter());
}
```
**단점**: 고작 정렬 한 번 하려고 클래스나 구조체를 저 멀리서 일일이 `GradeSorter`처럼 정의해 줘야 하니 가독성이 심각하게 떨어집니다.

---

## 3. ✨ 람다 함수 (Lambda): 모던 C++의 꽃

위 문제를 완벽히 해결하기 위해 등장한 것이 **람다(Lambda)**입니다.
람다는 **"외부에 클래스를 정의할 필요 없이, 그 현장에서 즉석으로 찍어내는 이름 없는 1회용 로직 덩어리"**입니다.

### 💡 람다의 기본 구조: `[캡처](파라미터) { 본문 }`
- `[]`: 캡처 (Capture) - 외부 변수를 진공청소기처럼 빨아들이는 공간
- `()`: 매개변수 (Parameters)
- `{}`: 알고리즘 본문 (Body)

```cpp
void ModernWay() {
    std::vector<int> levels = { 10, 50, 30, 99 };
    
    // Functor가 하던 짓을 람다가 그 자리에서 한 줄로 끝내버립니다!
    // std::sort의 3번째 인자로 즉석에서 이름 없는 함수를 만들어 던짐.
    std::sort(levels.begin(), levels.end(), 
        [](int a, int b) { 
            return a > b; // a가 b보다 크면 true (내림차순 정렬)
        }
    );
}
```

---

## 4. 🧲 캡처(Capture)의 위력: 변수 블랙홀

일반 함수와 람다가 차원이 다른 이유는 바로 이 **캡처(Capture) 블록 `[]`** 때문입니다.
람다는 자신이 작성된 지역(함수 안)의 지역 변수들을 자기 뱃속으로 끌고 들어와 로직 안에 합쳐버릴 수 있습니다.

```cpp
void CaptureExample() {
    int goldCondition = 5000;
    
    // 만약 일반 함수라면, 외부 지역 변수인 goldCondition에 접근할 방법이 없습니다.
    // 하지만 람다는 캡처 블록 [] 을 통해 빨아들입니다!
    
    // 1. [=] : 값 복사 (내부에서 원본 수정 불가)
    auto filter1 = [=](int myGold) { return myGold >= goldCondition; };
    
    // 2. [&] : 참조 복사 (내부에서 조작하면 외부의 원본도 즉시 바뀜! 속도도 빠름)
    auto filter2 = [&](int myGold) { 
        // 캡처한 외부 변수(goldCondition)와 매개변수(myGold)를 섞어서 로직 구현
        return myGold >= goldCondition; 
    };
    
    // 실무에서는 [=] 전체 복사나 [&] 전체 참조보단,
    // [&goldCondition] 처럼 필요한 변수만 콕 집어서 참조 복사하는 것을 권장합니다!
}
```

---

## 5. 🔍 하드웨어 검증: 람다는 사실 마법이 아니다

람다의 실체는 무엇일까요?
사실 람다를 작성하면 **컴파일러가 몰래 2번 항목에서 배운 함수 객체(Functor, 클래스)를 자동 생성해 줍니다.** 캡처한 변수들은 그 숨겨진 클래스의 `멤버 변수`로 둔갑합니다. 

눈으로 증명해보겠습니다.

```cpp
#include <iostream>

void VisualizeLambda() {
    std::cout << "=== 람다 하드웨어 크기 검증 ===\n";
    
    // 1. 아무것도 캡처하지 않은 람다
    auto emptyLambda = []() { std::cout << "Hi\n"; };
    std::cout << "빈 람다 크기: " << sizeof(emptyLambda) << " bytes\n"; 
    // 출력: 1 bytes (빈 클래스는 C++ 규약 상 최소 1바이트를 차지함)
    
    // 2. int(4 byte) 지역 변수를 값 복사로 캡처한 람다
    int hp = 100;
    auto captureLambda = [hp]() { std::cout << hp << "\n"; };
    
    std::cout << "캡처한 람다 크기: " << sizeof(captureLambda) << " bytes\n";
    // 출력: 4 bytes (마치 내부에 int hp 멤버 변수를 가진 구조체 클래스와 크기가 똑같아짐!)
}
```

## 🎯 요약
- **목적**: `std::sort`나 `std::find_if` 처럼, STL 알고리즘이 "어떻게 행동할지" 나의 커스텀 규칙(무기)을 끼워넣어 주기 위해.
- **최신 트렌드**: 옛날 Functor 방식은 버리고, 현장에 바로 쓰고 버리는 **익명 람다(Lambda)**를 사용
- **캡처(`[]`)**: 람다 내부로 주변 변수들을 끌어들이는 블랙홀. 게임 개발에서는 보통 **참조(`[&변수명]`)**를 통해 메모리 카피 비용을 줄입니다.

문서를 읽어보시고 이 **마법 같은 람다의 문법이나 동작 원리**와 관련하여 궁금한 점이 있으시면 자유롭게 질문해주세요. 완벽히 이해되셨다면 **"람다 실습 진행하자"**고 말씀해주세요! 😊
