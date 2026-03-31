# [Module 09] 스마트 포인터: 메모리를 스스로 치우는 똑똑한 포인터

## 1. 왜 스마트 포인터가 필요한가? — 오늘의 고통을 복습

Module 05에서, 그리고 오늘 세션에서 직접 경험했죠?

```cpp
// ❌ Raw Pointer의 저주
std::pair<int, int>* GetSpawnLocation() {
    auto* pp = new std::pair<int, int>{10, 20};  // 힙 할당
    return pp;  // 반환은 했는데...
}

int main() {
    auto* loc = GetSpawnLocation();
    // loc 사용...
    // ← delete loc; 잊어버리면? 메모리 영원히 누수!
}
```

**문제의 본질**: `new`로 태어난 객체의 생사(生死) 책임이 **프로그래머에게 100% 전가**됩니다.  
`delete`를 빠뜨리거나, 예외(Exception)가 발생하거나, 함수를 중간에 `return`하면 — 그 순간 메모리는 고아가 됩니다.

**스마트 포인터의 약속**: "내가 대신 책임질게. 내 스코프(범위)가 끝나면 알아서 `delete` 호출해줄게."

이 철학을 **RAII (Resource Acquisition Is Initialization)** 이라고 부릅니다.
> 자원(Resource)의 획득(Acquisition)과 초기화(Initialization)를 묶어,  
> **스코프 종료 시 자동으로 자원을 해제**하는 C++의 핵심 이디엄.

---

## 2. `std::unique_ptr` — 독점 소유권 포인터 💎

### 핵심 철학: "이 메모리의 주인은 오직 나 하나"

```cpp
#include <memory>  // 스마트 포인터는 여기서!

{
    // new 대신 make_unique 사용 (권장)
    std::unique_ptr<Monster> boss = std::make_unique<Monster>("Dragon", 9999);

    boss->Attack();  // 포인터처럼 -> 로 접근 가능
    (*boss).PrintStatus();  // 역참조도 가능

}  // ← 이 중괄호(스코프)가 끝나는 순간, boss의 소멸자가 자동 호출
   //   = delete가 자동으로 실행됨. 메모리 누수 ZERO!
```

### 소유권 이전: `std::move`

`unique_ptr`은 **복사가 금지**됩니다. 이 세상에 단 하나의 소유자만 허용하기 때문입니다.

```cpp
std::unique_ptr<Monster> boss = std::make_unique<Monster>("Dragon");

// ❌ 복사 불가 — 컴파일 에러!
// std::unique_ptr<Monster> copy = boss;

// ✅ 소유권 "이전(Move)"은 가능 — 이전 후 boss는 nullptr이 됨
std::unique_ptr<Monster> newOwner = std::move(boss);
// 이제 boss == nullptr, newOwner가 Dragon 소유
```

### 함수에서 활용

```cpp
// 반환: 힙 객체의 소유권을 호출자에게 넘겨줌
std::unique_ptr<std::pair<int,int>> GetSpawnLocation() {
    return std::make_unique<std::pair<int,int>>(10, 20);
}  // ← 이제 delete 걱정 전혀 없음!

int main() {
    auto loc = GetSpawnLocation();
    std::cout << loc->first;  // 10
}  // ← loc 스코프 종료 → 자동 delete
```

---

## 3. `std::shared_ptr` — 공유 소유권 포인터 🤝

### 핵심 철학: "여러 명이 같이 쓰되, 마지막 사람이 나갈 때 불 끄고 나가"

`unique_ptr`은 혼자만 소유할 수 있었지만, 게임에서는 같은 몬스터를 **여러 시스템(렌더, AI, 물리)이 동시에 참조**해야 하는 경우가 있습니다.

```cpp
#include <memory>

class Monster { /* ... */ };

auto monsterA = std::make_shared<Monster>("Slime");
// 내부적으로 "참조 카운트(Reference Count)" = 1

{
    auto monsterB = monsterA;  // ← shared_ptr은 복사 허용!
    // 참조 카운트 = 2 (monsterA와 monsterB 둘 다 가리킴)

    std::cout << monsterA.use_count();  // 2 출력
}  // monsterB 스코프 종료 → 참조 카운트 = 1 (아직 살아있음!)

// monsterA 스코프 종료 → 참조 카운트 = 0 → 비로소 delete!
```

| 항목 | `unique_ptr` | `shared_ptr` |
|------|-------------|-------------|
| 소유자 수 | 1명만 | 여러 명 가능 |
| 복사 | ❌ 불가 | ✅ 가능 |
| 오버헤드 | 거의 없음 (권장) | 참조 카운트 관리 비용 있음 |

> **실무 지침**: 특별한 이유가 없으면 `unique_ptr`을 기본으로 사용하세요.  
> `shared_ptr`은 진짜 공유가 필요할 때만 꺼냅니다.

---

## 4. `std::weak_ptr` — 관찰 전용 포인터 👁️

### 핵심 철학: "소유권은 없지만, 살아있는지는 확인할 수 있어"

`shared_ptr`만 쓰다 보면 **순환 참조(Circular Reference)** 문제가 발생합니다:

```
Player → shared_ptr<Guild>
Guild  → shared_ptr<Player>   ← 서로가 서로를 소유!
→ 참조 카운트가 영원히 0이 되지 않아 메모리 누수!
```

`weak_ptr`은 이 문제를 해결합니다:

```cpp
struct Guild;
struct Player {
    std::shared_ptr<Guild> guild;  // Player가 Guild를 소유
};

struct Guild {
    std::weak_ptr<Player> master;  // Guild는 Player를 "관찰"만 (소유 X)
};
```

`weak_ptr`로 실제 접근하려면 `lock()`으로 임시 `shared_ptr`을 빌려와야 합니다:

```cpp
if (auto player = guild.master.lock()) {  // 살아있으면 shared_ptr 반환
    player->Attack();
} else {
    // 이미 삭제된 객체 → 안전하게 처리
}
```

---

## 5. 세 가지 스마트 포인터 선택 가이드

```
새 힙 객체를 만들어야 한다
        │
        ▼
   독점 소유인가? ──Yes──▶ unique_ptr (기본 선택)
        │
       No
        │
        ▼
   여러 시스템이 공유하나? ──Yes──▶ shared_ptr
        │
       No
        │
        ▼
   소유 없이 관찰만? ──Yes──▶ weak_ptr
```

---

## 6. Raw Pointer vs Smart Pointer 최종 비교

```cpp
// ❌ 구시대 방식 (Raw Pointer)
Monster* m = new Monster();
m->Attack();
delete m;       // 잊으면? 누수!
m = nullptr;    // 잊으면? 댕글링 포인터!

// ✅ 현대 C++ 방식 (Smart Pointer)
auto m = std::make_unique<Monster>();
m->Attack();
// delete? nullptr? 그런 거 없음. 스코프 끝나면 알아서 다 됨.
```

> 💡 **현대 C++ 규칙**: `new`와 `delete`를 직접 쓰는 코드를 작성하지 마세요.  
> 반드시 `make_unique` 또는 `make_shared`를 사용하세요.

---

## 다음 단계

스마트 포인터의 개념을 익혔다면, 직접 손으로 써봐야 진짜 내 것이 됩니다!  
👉 **[실습]** `practice_smart_pointers.cpp` 에서 직접 구현해봅시다.

---

## 7. [Q&A] 실전 에러와 오해 해결

**Q1. `std::construct_at` (또는 `use of deleted function`) 에러가 뜹니다!**
*   **A**: C++ 컴파일러의 아주 간절한 외침입니다: **"복사할 수 없도록 설계된 unique_ptr를 강제로 복사(Copy)하려고 시도하고 있습니다!"**
*   `std::vector::push_back(ptr)` 처럼 포인터 자체를 복사해서 넣으려 할 때 자주 발생합니다. `push_back(std::move(ptr))`로 소유권을 명시적으로 넘겨주어야만 해결됩니다.

**Q2. `const unique_ptr<T>` 인데 어떻게 `->Use()`로 대상 객체를 수정할 수 있나요?**
*   **A**: 포인터(리모컨)의 `const`와 대상(TV)의 `const`는 다르기 때문입니다!
*   `unique_ptr` 앞에 붙은 `const`는 **"리모컨(`unique_ptr`) 자체를 버리거나 새 리모컨으로 바꿔치기할 수는 없다"**는 뜻입니다. (포인터 자체의 상수화)
*   하지만 자물쇠가 걸려있어도 리모컨의 전원 버튼을 누르는 것(`->Use()`)은 가능합니다. 리모컨이 가리키는 실제 TV(`T`)에는 `const`가 걸려있지 않기 때문입니다. 대상 기기 조작 자체를 원천적으로 막고 싶다면 템플릿 안쪽을 `unique_ptr<const T>` 로 선언해야 합니다.
