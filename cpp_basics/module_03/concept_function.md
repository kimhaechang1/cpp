# MODULE 03: 함수와 메모리 (Function & Memory)

## 1. 함수의 기초
함수(Function)는 "입력(인자)"을 받아 "처리"를 하고 "출력(반환값)"을 내뱉는 마법 상자입니다.
이미 `main()`이라는 함수를 쓰고 계셨고, `std::max`, `std::cout` 등도 모두 함수입니다.

### 1.1. 왜 함수를 쓰나요?
1. **재사용성**: 같은 코드를 복사/붙여넣기 할 필요가 없습니다.
2. **가독성**: `DamageCalculationWithDefenseAndBuff()` 처럼 이름만으로 기능을 알 수 있습니다.
3. **유지보수**: 버그가 생기면 함수 하나만 고치면 모든 곳에 적용됩니다.

---

## 2. 코딩 스탠다드: 이름 짓는 법 (Naming Convention) 🖊️

C++은 역사가 긴 만큼 다양한 스타일이 공존합니다. "무엇이 맞다"보다는 **"하나를 정해서 일관성 있게 쓰는 것"**이 정답입니다.

### 2.1. 대표적인 스타일 비교

| 스타일 | 설명 | 예시 | 주로 쓰이는 곳 |
| :--- | :--- | :--- | :--- |
| **Snake Case** | 소문자와 언더바(`_`) | `get_player_hp()`, `push_back()` | **C++ 표준 라이브러리 (STL)**, Google, Python |
| **Pascal Case** | 첫 글자 대문자 | `GetPlayerHp()`, `Attack()` | **게임 업계 (Unreal Engine)**, C# |
| **Camel Case** | 첫 글자 소문자 | `getPlayerHp()`, `calculateDamage()` | Java, 일반적인 SW 업계 |

### 2.2. 우리의 규칙 (Game Dev Style)
이 프로젝트에서는 **게임 개발 업계(언리얼 엔진 등)**에서 가장 많이 쓰이는 스타일을 따르겠습니다.

- **변수**: Camel Case (`playerHp`, `monsterCount`)
- **함수**: **Pascal Case** (`Attack()`, `GetExp()`) 👈 **이것이 질문하신 내용의 답변입니다!**
- **구조체/클래스**: Pascal Case (`Player`, `Monster`)

---

## 3. 매개변수 전달 방식 (가장 중요 ⭐)
C++에서 함수에 값을 전달하는 방법은 크게 3가지가 있습니다.
이 차이를 모르면 게임 데이터가 복사되어 성능이 느려지거나, 수정한 Hp가 반영이 안 되는 버그가 발생합니다.

### 3.1. Call by Value (값에 의한 전달)
- **특징**: 원본 데이터를 **복사**해서 함수에 줍니다.
- **비유**: 친구에게 숙제를 보여줄 때, **복사본**을 찍어서 주는 것과 같습니다.
- **결과**: 친구가 복사본에 낙서를 해도(함수 안에서 값을 바꿔도), 내 원본(함수 밖의 변수)은 변하지 않습니다.

```cpp
void Heal(int hp) { 
    hp += 10; // 복사된 hp만 증가함
}
int main() {
    int myHp = 50;
    Heal(myHp);
    // myHp는 여전히 50!
}
```

### 3.2. Call by Reference (참조에 의한 전달) `&`
- **특징**: 원본 데이터의 **별명(Reference)**을 함수에 줍니다.
- **비유**: 친구에게 내 숙제 노트 **원본**을 잠시 넘겨주는 것입니다.
- **결과**: 친구가 낙서하면 내 노트가 더러워집니다. (함수 안의 수정이 원본에 반영됨)

```cpp
void RealHeal(int& hp) { // &가 붙음!
    hp += 10; // 원본 myHp를 직접 건드림
}
int main() {
    int myHp = 50;
    RealHeal(myHp);
    // myHp는 60이 됨!
}
```

### 3.3. Const Reference (`const &`)
- **특징**: 원본을 참조하되, **수정은 못하게** 막습니다.
- **용도**: `Vector`나 `String` 처럼 큰 데이터를 함수에 보낼 때, **복사 비용을 없애면서도** 값을 안전하게 읽기만 하고 싶을 때 씁니다.
- **국룰**: `int`, `float`, `bool` 처럼 작은 건 그냥 보내고, **구조체나 클래스는 `const &`로 보내는 것**이 C++의 정석입니다.

---

## 4. 함수 선언과 정의 (헤더 파일의 맛보기)
C++ 컴파일러는 위에서 아래로 코드를 읽습니다.
그래서 `main` 함수보다 뒤에 있는 함수를 `main`에서 호출하면 "이게 뭔데?" 하고 에러가 납니다.

### 4.1. 전방 선언 (Forward Declaration)
"나중에 이런 함수가 나올 거야"라고 미리 알려주는 것입니다.

```cpp
// 선언 (Declaration)
void Attack(); 

int main() {
    Attack(); // OK! 컴파일러가 Attack의 존재를 앎
}

// 정의 (Definition)
void Attack() {
    cout << "때리기!";
}
```

---

## 5. 앞으로 더 배울 내용 (Part 2 예고)
함수에는 아직 다루지 않은 고급 기능들이 많이 있습니다. 일단 위의 기초를 실습한 뒤 다음 내용들을 다룰 예정입니다:

1.  **함수 오버로딩 (Overloading)**: 같은 이름(`Attack`)인데 매개변수가 다른 경우 (`Attack(int)`, `Attack(float)`)
2.  **디폴트 매개변수 (Default Parameter)**: `void Attack(int dmg = 10)`
3.  **스택 메모리 (Stack Frame)**: 함수가 호출될 때 메모리에 어떤일이 일어나는지 (지역 변수의 수명)

지금은 **Function & Memory Part 1**의 내용인 **" 값 전달 vs 참조 전달"**을 확실히 익히는 것이 최우선입니다!

---

## 6. 실습 목표
이번 실습(`function_practice.cpp`)에서는 다음을 수행합니다.
1. `Swap` 함수를 Value 버전과 Reference 버전으로 만들어 비교하기
2. `Monster` 구조체를 `const &`로 받아 정보 출력하기 (복사 방지)
3. 간단한 전투 로직 함수화하기 (Naming Convention 준수!)

준비되셨으면 **"함수 실습 진행"**이라고 말씀해주세요!
