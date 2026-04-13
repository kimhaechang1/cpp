# Module 07: 객체 슬라이싱 (Object Slicing) - 다형성의 치명적인 함정

## 🎮 실무 프로젝트 상황 (샌드위치 교육법)
몬스터들을 관리하는 `BattleManager` 시스템을 만들었습니다. 이곳에는 모든 몬스터를 일괄 공격하게 하는 함수가 있습니다.

```cpp
// ❌ 아주 위험한 코드!!
void BroadCastAttack(Monster monster) { 
    monster.Attack(); 
}
```

개발자는 "Monster를 상속받은 드래곤이나 오크를 넣으면 다형성으로 잘 작동하겠지?"라고 기대하며 `Dragon` 객체를 넘겼습니다. 하지만 결과는 드래곤의 브레스가 아닌 **"몬스터의 평타"**만 나갔습니다. 심지어 드래곤의 특수 스탯(`breathPower` 등)도 모두 사라져 버렸습니다. 

이것이 바로 C++의 가장 유명한 함정 중 하나인 **객체 슬라이싱(Object Slicing)** 입니다.

---

## 🏗️ 1. 원인: 메모리 "싹둑" (Slicing)

Module 07 기초에서 배웠듯, 자식 객체의 메모리 구조는 **[부모 영역] + [자식 고유 영역]** 으로 이루어집니다.

```cpp
class Monster { int hp; };                 // 4 bytes
class Dragon : public Monster { int power; }; // 4 + 4 = 8 bytes
```

만약 함수가 객체를 **값(By-Value)**으로 받는다면(`void Func(Monster m)`), 컴파일러는 넘겨받은 `Dragon` 객체에서 **Monster 크기(4바이트)만큼만 쏙 떼어내서(Slicing) 복사**해버립니다.

### 💀 슬라이싱의 결과
1. **데이터 유실**: 자식만의 고유 변수(`power`)가 담긴 뒷부분 메모리가 물리적으로 잘려나갑니다.
2. **정체성 상실 (vptr의 배신)**: 복사된 `m`은 이제 완전한 `Monster` 타입이 되어버립니다. 뱃속의 `vptr`은 `Monster`의 vtable을 가리키게 되어, 더 이상 `Dragon`의 기능을 수행할 수 없습니다.

---

## 🛠️ 2. 해결책: 주소(Address)로 관리하라

다형성을 유지하고 데이터 유실을 막으려면, 객체 자체가 아닌 **객체의 주소**를 넘겨야 합니다. 주소만 넘기면 메모리를 덩어리째 복사하지 않으므로 잘려나갈 일도 없습니다.

```cpp
// ✅ 안전한 코드 (참조 사용)
void SafeAttack(const Monster& monster) { // 원본의 주소만 참조함
    monster.Attack(); // vptr이 살아있어 드래곤의 브레스가 나감!
}

// ✅ 안전한 코드 (포인터/스마트 포인터 사용)
void SafeAttack(Monster* monster) {
    monster->Attack();
}
```

---

## 🏛️ 3. 실무 아키텍처: 방어적 설계 (Defensive Design)

실무에서는 동료 개발자가 실수로라도 다형성 객체를 복사하는 것을 막기 위해 아예 **복사 기능을 물리적으로 삭제**해버립니다.

```cpp
class Monster {
public:
    Monster() = default;
    virtual ~Monster() = default;

    // 🚨 절대 규칙: 다형성 객체는 복사를 금지한다.
    Monster(const Monster&) = delete;            
    Monster& operator=(const Monster&) = delete; 
    
    virtual void Attack() = 0;
};
```

이렇게 `= delete`를 선언해두면, 누군가 `void Func(Monster m)` 처럼 값으로 넘기려 할 때 컴파일러가 **"이 객체는 복사할 수 없습니다!"** 라고 빌드를 막아줍니다. 런타임 사고를 컴파일 타임에 예방하는 **C++ 시니어의 노하우**입니다.

---

## 🏆 결론: 100점짜리 요약

> **"다형성(Virtual)이 들어간 모든 객체는 절대로 값(Value)으로 다루지 마라. 무조건 포인터(`*`)나 참조(`&`), 혹은 스마트 포인터(`unique_ptr`)로만 유통시켜라."**

---

### 🔍 하드웨어 검증 (직접 확인해보기)
파일: `cpp_basics/module_07/practice_object_slicing.cpp`
`sizeof`를 통해 실제 객체가 복사될 때 자식 영역이 얼마나 줄어드는지, 그리고 멤버 함수의 호출 주소가 어떻게 변하는지 눈으로 직접 확인해보는 것을 추천합니다.
