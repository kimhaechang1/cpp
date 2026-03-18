# 🚀 데이터 지향 설계 (Data-Oriented Design, DOD)

객체지향 프로그래밍(OOP)이 "객체가 무엇인가"에 집중한다면, 데이터 지향 설계(DOD)는 **"데이터가 어떻게 메모리에 놓여 있는가"**와 **"CPU가 어떻게 데이터를 가져오는가"**에 집중합니다. 

현대 게임 엔진(Unity DOTS, Unreal Engine 등)의 성능 최적화 핵심 철학입니다.

---

### 🧠 1. CPU 캐시와 캐시 라인 (Cache Line)

CPU는 메모리(RAM)에서 데이터를 가져올 때, 딱 필요한 4바이트(int 등)만 가져오지 않습니다. 주변의 데이터 **64바이트(Cache Line)**를 한 번에 뭉뚱그려 캐시 메모리에 끌고 옵니다. 이를 **Pre-fetch**라고 합니다.

*   **캐시 적중(Cache Hit)**: 다음에 필요한 데이터가 이미 캐시에 있을 때. (매우 빠름!)
*   **캐시 미스(Cache Miss)**: 다음 데이터가 저 멀리 RAM에 있어서 다시 가지러 가야 할 때. (매우 느림!)

---

### 🏗️ 2. AoS (Array of Structures) vs SoA (Structure of Arrays)

#### ✅ AoS: 구조체의 배열 (OOP 방식)
"몬스터라는 객체" 중심으로 설계합니다.

```cpp
struct Monster {
    int id;           // 4 bytes
    char name[64];    // 64 bytes (준비만 함)
    float hp;         // 4 bytes
    float posX, posY; // 8 bytes
};

// 1. 몬스터 1만 마리 생성 (메모리에 Monster 덩어리가 쭈욱 붙어 있음)
std::vector<Monster> monsters(10000); 

// 2. HP만 업데이트하고 싶을 때
for(auto& m : monsters) { m.hp += 1.0f; }
```
**문제점**: CPU가 `hp`를 하나 가져올 때, 주변에 있는 `name`과 `id`도 같이 캐시 라인(64바이트)에 들어옵니다. 정작 다음 몬스터의 `hp`는 아직 RAM에 있을 가능성이 큽니다. 캐시 미스가 빈번히 발생합니다.

#### ✅ SoA: 배열의 구조체 (DOD 방식)
"데이터의 속성" 중심으로 설계합니다.

```cpp
struct MonsterWorld {
    std::vector<int> ids;
    std::vector<float> hps;         // HP만 따로 모은 배열
    std::vector<std::string> names;
    std::vector<Vector2> positions;
};

// HP만 업데이트하고 싶을 때
for(auto& hp : world.hps) { hp += 1.0f; }
```
**장점**: `hps` 배열은 메모리에 `float`들만 따닥따닥 붙어 있습니다. CPU가 `hp[0]`을 가져오면 캐시 라인(64바이트)에 **다음 15마리의 HP 정보**(4*16=64)가 굴줄이 딸려옵니다! 캐시 적중률이 극대화됩니다.

---

### 📊 성능 차이 비교

| 특징 | AoS (OOP) | SoA (DOD) |
| :--- | :--- | :--- |
| **중심** | 객체 (Monster) | 데이터 (HP, Position) |
| **강점** | 코드 가시성, 직관적 설계 | **메모리 효율성, 압도적 성능** |
| **캐시 효율** | 낮음 (쓸모없는 데이터 로딩) | 높음 (필요한 데이터만 선별 로딩) |
| **적합한 곳** | 복잡한 상속/논리가 필요한 곳 | 수 만개의 개체를 동시 처리할 때 |

---

### 💡 게임 프로그래머가 가져야 할 시야

상속과 다형성은 설계의 유연성을 주지만, 하드웨어(CPU/RAM) 입장에서는 "지그재그로 뛰어다녀야 하는 불친절한 코드"가 될 수 있습니다. 

**"어떻게 하면 CPU가 한 번 손을 뻗었을 때 가장 많은 데이터를 한입에 먹게 해줄까?"** 

이 질문이 바로 고성능 게임 프로그래밍의 시작입니다.
