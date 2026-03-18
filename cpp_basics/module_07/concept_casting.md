# 🎭 C++ 네 가지 캐스팅 (Casting in Modern C++)

C++에서는 C 스타일의 괄호 캐스팅 `(Type)Value`를 지양합니다. 대신 목적에 따라 명시적인 4가지 캐스팅 도구를 사용합니다.

---

### 🧐 핵심 개념: 정적 타입(Static) vs 동적 타입(Dynamic)

*   **정적 타입 (Static Type)**: 컴파일 타임에 결정되는 포인터나 변수의 타입 (예: `BaseEntity* p`)
*   **동적 타입 (Dynamic Type)**: 런타임에 실제 메모리(Heap/Stack)에 존재하는 객체의 타입 (예: `Character`)
*   **실제 타입**: "메모리상의 데이터 덩어리"가 어떤 클래스의 설계도대로 만들어졌는지를 의미합니다. `new` 할 때 결정된 그 정체입니다.

---

### 1. static_cast (논리적 형변환)
- **언제**: 서로 관련 있는 타입 간의 변환 (부모-자식 상속 관계, int-float 등).
- **특징**: 컴파일 타임에 결정되므로 매우 빠릅니다. **"개발자가 안전하다고 보장한다"**는 의미가 강합니다.
- **위험**: 부모 포인터를 자식 포인터로 강제 변환할 때, 실제 객체가 그 자식 타입이 아니더라도 컴파일러는 믿고 변환합니다. (결과는 런타임 에러나 쓰레기 값).

```cpp
float f = 3.14f;
int i = static_cast<int>(f); // 데이터 유실을 감수하고 변환

Base* b = new Character();
Character* c = static_cast<Character*>(b); // 안전 (진짜 Character니까)
```

### 2. dynamic_cast (안전한 다형성 형변환) ⭐
- **언제**: 상속 관계에서 **"진짜 이 타입이 맞는지"** 런타임에 확인하며 변환할 때.
- **특징**: 런타임에 **RTTI(Run-Time Type Information)**를 체크합니다. 변환 실패 시 **`nullptr`**를 리턴합니다.
- **🚨 왜 가상 함수가 최소 하나 있어야 하나요?**:
    - C++는 성능을 위해 객체 내부에 타입 정보를 저장하지 않습니다.
    - 하지만 **가상 함수가 생기면 `vptr` (가상 함수 테이블 포인터)이 생성**됩니다.
    - 컴파일러는 이 `vtable`의 정보 속에 RTTI(타입 정보)를 몰래 끼워 놓습니다.
    - `dynamic_cast`는 바로 이 `vptr`을 타고 들어가서 RTTI를 읽어야만 정체를 알 수 있습니다. 즉, **`vptr`이 없는 클래스는 물어볼 주소가 없는 것**과 같습니다.

```cpp
BaseEntity* entity = GetAnyEntity(); // Character일 수도, NPC일 수도 있음.
Character* character = dynamic_cast<Character*>(entity);

if (character) {
    character->Attack(); // Character가 맞을 때만 실행
} else {
    // Character가 아님 (NPC 등)
}
```

### 3. reinterpret_cast (비트 수준 형변환)
- **언제**: 전혀 관계없는 타입 간의 강제 변환 (포인터 -> 정수 포인터 등).
- **특징**: "내가 맞으니까 그냥 이 메모리 주소를 이 타입으로 해석해!"라고 컴파일러에게 강요합니다. 
- **예시**: 메모리 주소값을 디버깅용으로 출력하거나, 소켓 통신에서 패킷 데이터를 바이트 배열로 읽을 때 사용합니다.

```cpp
int* p = new int(65);
char* ch = reinterpret_cast<char*>(p); // 'A' (65)로 해석될 가능성. 매우 위험!
```

### 4. const_cast (상수성 제거)
- **언제**: `const`로 선언된 객체의 상수성을 잠시 제거해야 할 때.
- **주의**: 원래 `const`인 데이터를 이 캐스팅으로 바꿔서 값을 수정하는 것은 금기사항입니다. 주로 레거시 라이브러리의 인자가 `const`를 지원하지 않을 때 "잠시만 상수가 아닌 것처럼 행동해"라고 할 때 씁니다.

```cpp
void LegacyFunction(char* str); // const를 안 받는 옛날 함수

const char* myStr = "Hello";
LegacyFunction(const_cast<char*>(myStr)); // 잠시 상수성 해제
```
