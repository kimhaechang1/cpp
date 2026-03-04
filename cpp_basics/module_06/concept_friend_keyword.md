# Friend 키워드: 캡슐화를 깨는 합법적인 백도어(Backdoor)

## 1. 실무 프로젝트 상황: "왜 남의 private 영역을 털러 가야 할까?"

회창님, 우리는 클래스를 배울 때 **캡슐화(Encapsulation)**를 무척 강조했습니다. 외부에서 함부로 조작하면 안 되는 데이터(HP 변화, 인벤토리 포인터 등)는 `private`에 숨기고, 오직 **안전띠가 둘러진 Getter/Setter 함수**를 통해서만 접근하도록 강제했죠.

하지만 실무에서는 성능(Performance)이나 특정 패턴 구현을 위해 এই 캡슐화를 의도적으로 뚫어야 할 때가 생깁니다.

1.  **연산자 오버로딩 (Operator Overloading)**: 저번 시간에 `std::ostream& operator<<` 외부 함수를 구현할 때, 어쩔 수 없이 `Player`의 `private` 변수인 `name`, `hp`에 접근하기 위해 Getter를 도배해야만 했습니다. 살짝 번거롭고 직관성이 떨어지죠.
2.  **초고속 수학 연산 (Math Libraries)**: 3D 그래픽스 프로그래밍을 할 때 `Vector3` 클래스와 `Matrix4` 클래스는 극한의 성능을 위해 수백만 번 단위로 곱해집니다. 이 둘이 계산될 때마다 일일이 Getter를 호출한다면 오버헤드가 발생할 수 있습니다. 즉, **"비밀을 터놓을 수 있는 베스트 프렌드(Best Friend)"**가 필요해집니다.

---

## 2. 해결책: "나랑 친한 친구(friend)만 열어줄게"

C++에는 `friend`라는 독특한 키워드가 있습니다.
이 키워드는 클래스 내부에서 **"특정 외부 함수나 특정 외부 클래스에게는 내 private 금고의 열쇠를 복사해 주겠다"**고 선언하는 것입니다.

```cpp
class Player {
private:
    std::string name{"Hero"};
    int hp{100};

    // 1. 특정 외부 "함수"에게 열쇠 허용
    friend void CheatHeal(Player& p);

    // 2. 특정 외부 "클래스"에게 열쇠 허용
    friend class GM_Console; // GM_Console 클래스는 Player의 모든 속을 들여다볼 수 있음!
};

// Player 클래스 외부에 있는 평범한 함수입니다.
void CheatHeal(Player& p) {
    // 본래라면 p.hp 접근 시 컴파일 에러! (private이니까)
    // 하지만 friend 선언이 되어 있으므로 합법적으로 직접 조작 가능.
    p.hp = 9999; 
}
```

---

## 3. 메모리 및 하드웨어 관점: "friend 객체가 메모리를 더 먹나요?"

### 결론: 메모리 오버헤드 = 0

`sizeof(Player)`를 찍어보아도, `friend`를 1개 설정하든 100개 설정하든 메모리 크기는 1바이트도 늘어나지 않습니다.
왜일까요?

> **friend는 그저 "컴파일러 검사 때 접근 에러를 띄우지 말라"는 컴파일 타임(Compile-time)의 예외 조항일 뿐입니다.**

실행(Runtime) 시간에 작동하는 동작이 아니므로, 메모리 구조나 CPU 레지스터에 어떠한 영향도 미치지 않습니다.

---

## 4. 실무 활용 예시: `friend`가 정당화되는 순간들

`friend`는 남발하면 객체지향을 망치지만, **특정 디자인 패턴**이나 **C++의 구조적 한계**를 극복하기 위해선 필수적으로 사용됩니다.

### ① 연산자 오버로딩 (`operator<<` 등)
**의도(Intention): "내가 만든 클래스도 `std::cout << p;` 처럼 우아하게 출력하고 싶다!"**

우리가 원하는 목표는 아주 단순합니다. `Player` 클래스 내부의 정보를 콘솔 화면(`std::cout`)에 직관적으로 출력하고 싶은 것이죠.

```cpp
Player p;
std::cout << p;  // 목표: cout 객체를 통해 내 클래스 정칙를 바로 출력하고 싶다!
```

그런데 여기서 치명적인 **구조적 딜레마**가 발생합니다.

1.  `<<` 연산자를 기준으로 **왼쪽의 주인은 `std::cout` (즉, `std::ostream` 클래스)**이고, 오른쪽은 `Player`입니다.
2.  이것이 동작하려면 연산자의 왼쪽 주인인 `std::ostream` 클래스 내부에 `Player`를 처리하는 `operator<<` 멤버 함수를 우리가 새롭게 만들어 넣어야 합니다.
3.  하지만 **`std::ostream`은 C++ 표준 라이브러리**이기 때문에 우리가 그 원본 코드를 뜯어고칠 수 없죠! ⛔
4.  만약 우리가 뜯어고칠 수 있는 `Player` 클래스 내부에 이 멤버 함수를 만든다면? `p << std::cout;` 이라는 문법적으로 매우 기괴한 코드를 써야만 합니다.

결국 이 딜레마를 극복하는 완벽한 해결책은, **어느 클래스에도 소속되지 않은 독립적인 '전역 함수'**를 하나 새로 만드는 것입니다. 그리고 이 전역 함수가 출력에 필요한 `Player`의 `private` 변수(`name`, `hp`)를 읽어올 수 있도록, **`Player` 측에서 `friend` 허가증을 발급해 주는 것**입니다.

```cpp
class Player {
private:
    std::string name{"Hero"};
    int hp{100};

    // 전역 함수에게 내 private 멤버에 대한 접근 권한을 줌!
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};

// Player의 멤버가 아닌 전역 함수. 
// 덕분에 os(왼쪽) << p(오른쪽) 순서로 인자를 받을 수 있음.
std::ostream& operator<<(std::ostream& os, const Player& p) {
    // p.name, p.hp 등 private 멤버에 마음껏 직접 접근!
    os << "Player: " << p.name << " (HP: " << p.hp << ")";
    return os;
}
```

### ② 팩토리 패턴 (Factory Pattern) / 빌더 패턴 (Builder Pattern)
오직 **특수한 관리자만 나를 생성하고 조작할 수 있게 만들고 싶을 때**가 있습니다.
예를 들어, 몬스터 정보는 무조건 `MonsterSpawner` 클래스(생성기)를 통해서만 만들 수 있어야 한다면 생성자 자체를 `private`으로 닫아버리고 스패너(`MonsterSpawner`)만 `friend`로 허용해 줍니다.

```cpp
class Monster {
private: // 외부에서는 절대 못 만듦!
    int hp;
    
    // 오직 나만 만들 수 있도록 생성자를 private에 숨김
    Monster(int _id, int _hp) : id{_id}, hp{_hp} {}

    // 하지만 생성자(Spawner)에게는 권한 부여!
    friend class MonsterSpawner; 
};

class MonsterSpawner {
private:
    int _nextId{1};
public:
    // Spawner는 Monster의 private 속성(생성자)에 접근 가능
    Monster CreateSlime() {
        return Monster(_nextId++, 50); 
    }
};

int main() {
    // Monster m1; // 에러! 생성자가 private이라 접근 불가
    MonsterSpawner spawner;
    Monster slime = spawner.CreateSlime(); // 성공! 합법적인 우회 방법
}
```

### ③ 특정 클래스의 특정 함수만 콕 집어서 허용하기
방금 위에서는 `friend class MonsterSpawner;` 라고 하여 클래스 전체에 통째로 권한을 열어주었습니다. 하지만 객체지향의 원칙(최소 권한의 법칙)에 따르면, **해당 클래스의 특정 함수만 콕 집어서 허용해 주는 것**이 더 좋은 설계입니다.

```cpp
// 1. 전방 선언: System 클래스가 암튼 어딘가에 있다고 알려줌
class System;

class Player {
private:
    int hp{100};
    
    // 2. System 클래스의 전체가 아니라, 오직 Reset() 함수만 내 친구야!
    friend void System::Reset(Player& p);
};
```
이렇게 하면 `System` 클래스의 다른 함수들(예: `System::Attack()`)은 `Player`의 `private`에 접근할 수 없습니다. 오직 `Reset()` 함수에게만 핀셋으로 권한을 준 것이죠!

### ④ 특정 네임스페이스(Namespace)에 속한 함수 허용하기
함수가 전역 함수가 아니라 특정 네임스페이스 안에 갇혀있을 때도 동일한 방식으로 소속을 명시해서 허가해 줄 수 있습니다.

```cpp
namespace GameEngine {
    void FastUpdate(class Entity& e);
}

class Entity {
private:
    float x, y;

    // 네임스페이스 소속을 정확히 밝혀서 그 함수만 허용
    friend void GameEngine::FastUpdate(Entity& e);
};
```
---

### 👉 [생각해보기]
회창님, 만약 A 클래스가 B 클래스를 `friend`로 선언했다면, **반대로 B 클래스도 A 클래스의 private에 마음대로 접근할 수 있을까요?** (친구니까 상호 적용?)
직관적으로 어떻게 생각하시나요?

---

### 🗨️ 회창님의 생각과 피드백 (Q&A)

**Q. 회창님의 답변:**
> "B 클래스만 일방적으로 A를 접근 가능하고, A 클래스는 `friend class B`를 하지 않는 이상 서로 접근이 안 될 것 같은데? 그리고 만약에 서로 컴포지트(객체가 서로를 포함)하는 상황이면 무한 루프 돌 수도 있는 거 아닌가?"

**A. 멘토의 피드백:**
100점 만점에 100점짜리 직관입니다! 

1. **친구 관계는 철저한 단방향(일방통행)입니다.**
   A가 B를 `friend`로 선언했다고 해서 나 자신(A)도 자동으로 친구가 되는 것은 아닙니다. 짝사랑만 존재할 뿐, 서로 `friend`를 맺고 싶다면 양쪽 클래스 모두 명시해 주어야 합니다.

2. **컴포지트(Composite)의 무한 루프 문제와 포인터**
   회창님 말씀대로 두 클래스가 서로를 객체 멤버(컴포지트)로 소유하는 것은 `friend`의 문제를 떠나, **C++ 메모리 할당(`sizeof`) 단계에서 치명적인 에러**를 발생시킵니다.
   ```cpp
   class B;
   class A { B myB; }; // A 크기 = B 크기...?
   class B { A myA; }; // B 크기 = A 크기...?
   ```
   컴파일러가 `sizeof(A)`를 계산하려다 뫼비우스의 띠처럼 무한 루프(Infinite Recursion)에 빠져 컴파일을 거부해 버리죠.
   이 딜레마를 완벽하게 끊어내는 해결책이 바로 우리가 이전에 배웠던 **포인터(Pointer)**입니다!
   
   서로를 참조해야 하는 경우, 몸통 전체를 포함하는 것이 아니라 64비트 환경 기준 8바이트로 크기가 고정된 **"메모리 주소가 적힌 쪽지(포인터)"**만 들고 있으면 무한 루프가 완벽하게 끊어지게 됩니다. 
   
   ```cpp
   class B;
   class A { B* ptrB; }; // 크기: 확실하게 8바이트!
   class B { A* ptrA; }; // 크기: 확실하게 8바이트! 
   // -> 계산 종료 (성공)
   ```
