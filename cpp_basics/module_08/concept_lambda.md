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

### 💡 람다의 구조: `[Capture] (Parameters) { Body }`
람다는 데이터가 드나드는 **두 개의 독립된 문(Door)**을 가지고 있습니다. 

1.  **`[Capture]` (외부 변수 공유 창고)**: 람다 함수 **바깥풍경(`main` 등)**의 변수들을 어떻게 빌려올 것인가? 
    - `[&]`: "바깥 변수들을 참조로 빌려와서 원본을 쓰겠다!" (효율적)
    - `[=]`: "바깥 변수들을 복사해서 내 꺼로 만들겠다!" (독립적)

2.  **`(Parameters)` (입구 벨트)**: 람다가 **실행(Call)될 때** 넘겨받는 재료를 어떻게 받을 것인가? 
    - `(Monster m)`: "들어오는 데이터를 **복사해서** 쓰겠다!" (비효율)
    - `(const Monster& m)`: "들어오는 데이터의 **원본 주소만** 받아서 쓰겠다!" (효율 ⭐)

**💡 핵심**: `[&]`를 쓴다고 해서 `(Monster m)`으로 들어오는 데이터가 자동으로 참조가 되는 것이 아닙니다! 

---

## 4. 람다의 모양(Signature): "Java의 Predicate 같은 건 없나요?"
Java에는 `Predicate<T>` 처럼 람다의 모양을 명시하는 인터페이스가 있듯, C++ 알고리즘도 특정 규격을 원합니다.

### 💡 C++20 Concepts가 해결합니다!
이제 C++도 **Concepts**를 통해 "너가 넣는 람다는 반드시 **bool을 반환**해야 해!"라고 강제합니다. (예: `std::indirect_unary_predicate`) 만약 틀린 모양의 람다를 넣으면, 예전처럼 암호 같은 에러가 아니라 **"Predicate 조건을 만족하지 않습니다"**라고 명확히 알려줍니다.

---

## 5. 🧲 캡처(Capture)의 위력: 변수 블랙홀

일반 함수와 람다가 차원이 다른 이유는 바로 이 **캡처(Capture) 블록 `[]`** 때문입니다.
람다는 자신이 작성된 지역(함수 안)의 지역 변수들을 자기 뱃속으로 끌고 들어와 로직 안에 합쳐버릴 수 있습니다.

```cpp
void CaptureExample() {
    int goldCondition = 5000;
    
    // 1. [=] : 값 복사 (내부에서 원본 수정 불가)
    auto filter1 = [=](int myGold) { return myGold >= goldCondition; };
    
    // 2. [&] : 참조 복사 (내부에서 조작하면 외부의 원본도 즉시 바뀜!)
    auto filter2 = [&](int myGold) { 
        return myGold >= goldCondition; 
    };
}
```

---

## 6. 🔍 하드웨어 검증: 람다의 실체

람다를 작성하면 **컴파일러가 몰래 함수 객체(Functor, 클래스)를 자동 생성해 줍니다.** 캡처한 변수들은 그 숨겨진 클래스의 `멤버 변수`가 됩니다.

```cpp
void VisualizeLambda() {
    // 1. 아무것도 캡처하지 않은 람다
    auto emptyLambda = []() { std::cout << "Hi\n"; };
    std::cout << "빈 람다 크기: " << sizeof(emptyLambda) << " bytes\n"; 
    // 출력: 1 bytes (C++ 규약 상 최소 크기)
    
    // 2. int(4 byte) 지역 변수를 값 복사로 캡처한 람다
    int hp = 100;
    auto captureLambda = [hp]() { std::cout << hp << "\n"; };
    
    std::cout << "캡처한 람다 크기: " << sizeof(captureLambda) << " bytes\n";
    // 출력: 4 bytes (int hp 멤버를 가진 클래스와 동일)
}
```

## 🎯 요약
- **목적**: STL 알고리즘에게 나의 커스텀 규칙(정렬, 필터링 등)을 전해주기 위해.
- **구조**: `[Capture]`는 주변 환경 공유, `(Parameters)`는 들어오는 재료 대접. (서로 독립적!)
- **시그니처**: `filter` 등은 `bool`을 반환하는 람다를 원하며, 이는 **C++20 Concepts**를 통해 검증됨.
