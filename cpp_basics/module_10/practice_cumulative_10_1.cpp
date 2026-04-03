#include <cassert>
#include <fmt/core.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

// -- [M10 Core] Multi-file Project --
// 아래 헤더 파일들은 아직 존재하지 않습니다.
// 이 실습의 핵심은 이 파일들을 "직접 설계하고 생성"하는 것입니다.
// CMakeLists.txt도 직접 수정하여 이 실습 전체가 빌드되도록 구성하세요.

#include "Registry.h"

using json = nlohmann::json;

// =====================================================================
// [MODULE 10 Cumulative Practice] Guild Equipment Registry
//
// JSON으로 수신된 장비 데이터를 객체로 변환하고,
// 길드 등록소(Registry)에 등록/정렬/조회하는 시스템을 구축합니다.
//
// [Core: M10] 이 실습은 단일 파일이 아닌 "멀티 파일 프로젝트"입니다.
// 헤더(.h) / 소스(.cpp) 파일을 직접 작성하고,
// CMakeLists.txt를 수정하여 빌드 타겟을 등록하세요.
//
// =====================================================================
//
// [생성해야 할 파일 목록]
//
// --- 1. Equipment.h ---
//
//   [M1] 장비의 종류를 나타내는 카테고리:
//     오류 원천 차단을 위해, 상태들을 한정된 카테고리로 묶어
//     안전한 기호 집합으로 정의하세요. (Sword, Shield, Staff)
//
//   [M7] 장비의 추상적 설계도 (Equipment 클래스):
//     자식마다 행동이 달라야 하므로, 부모에선 명세만 남기고
//     본체는 비워두어 인스턴스화를 막아야 합니다.
//     필요한 행동: GetName, GetPower, Describe
//     (부모 포인터로 자식을 삭제할 때의 안전 처리도 반드시 고려하세요.)
//
//   [M6] Weapon 클래스 (Equipment 상속):
//     생성 시 이름, 위력, 카테고리 정보를 받아 저장합니다.
//     중괄호로 진입하기 전, 선언과 동시에 최우선으로 초기화하여
//     무의미한 대입 비용을 제거하세요. (Initializer List)
//     Describe() 호출 시 fmt::print로 장비 정보를 출력합니다.
//
//   Armor 클래스 (Equipment 상속):
//     생성 시 이름과 방어력을 받아 저장합니다.
//     GetPower()는 방어력 값을 반환합니다.
//
// --- 2. Registry.h / Registry.cpp ---
//
//   [M6] Registry 클래스:
//     수많은 인스턴스가 생성되더라도, "전체 등록 장비 수"는
//     메모리 상에 오직 단 하나만 존재하여 전역적으로 공유되어야 합니다.
//
//   [M8-M9] 장비 저장소:
//     스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는
//     똑똑한 독점 메모리 래퍼로 감싸인 장비들을
//     가변 길이 연속 저장소에 보관합니다.
//
//   Register():
//     외부에서 독점 메모리 래퍼를 넘겨받아 저장소에 추가합니다.
//     무거운 데이터를 복사하지 말고,
//     내부 자원의 껍데기만 빠르고 난폭하게 강탈하세요.
//
//   [M3] CalcTotalPower():
//     함수 호출이 끝난 후에도 외부에 있는 원본 데이터가
//     수정된 결과를 온전히 보존해야 합니다. (총 전투력 합산)
//
//   [M8] SortByPower():
//     함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서
//     이름 없는 1회용 로직 덩어리를 만들어 정렬 알고리즘에 던지세요.
//     위력이 높은 것이 앞으로 오도록 내림차순 정렬합니다.
//
//   PrintAll(): 모든 장비의 Describe()를 호출하여 정보를 출력합니다.
//   GetSize(): 보관 중인 장비 수를 반환합니다.
//   GetItemAt(index): 특정 위치의 장비를 복사 없이 참조로 반환합니다.
//
// --- 3. CMakeLists.txt (직접 수정) ---
//   이 파일(practice_cumulative_10_1.cpp)과 Registry.cpp를 포함하는
//   새로운 실행 타겟을 등록하고, 필요한 외부 라이브러리를 링크하세요.
//
// =====================================================================
// [Covered Modules]
// M1:  enum class (EquipCategory), Uniform Initialization
// M2:  switch (GetCategoryLabel)
// M3:  Call by Reference (CalcTotalPower)
// M4:  const T& (StringToCategory 매개변수)
// M5:  Heap allocation (smart pointer 경유)
// M6:  static member (totalRegistered), Initializer List
// M7:  Pure virtual / Abstract class, Virtual Destructor
// M8:  Lambda (SortByPower), vector
// M9:  unique_ptr + std::move (ownership transfer)
// M10: Multi-file CMake project + vcpkg (fmt, nlohmann-json)
// =====================================================================

// -----------------------------------------------------------------
// [M2] 카테고리에 따른 라벨을 반환합니다.
// Sword -> "Blade", Shield -> "Guard", Staff -> "Magic"
// 상태에 따라 분기하는 제어문(switch)을 사용하세요.
// 해당하지 않는 경우 "Unknown"을 반환합니다.
//
// (함수 이름: GetCategoryLabel)
// (매개변수: 카테고리 값 1개)
// (반환값: 해당하는 라벨 문자열)
// ------- 이 아래에 함수를 직접 작성하세요 -------
std::string GetCategoryLabel(EquipCategory category) {
  switch (category) {
  case EquipCategory::Shield:
    return "Guard";
  case EquipCategory::Staff:
    return "Magic";
  case EquipCategory::Sword:
    return "Blade";
  default:
    return "Unknown";
  }
}

// ------- 이 위에 함수를 직접 작성하세요 -------

// -----------------------------------------------------------------
// [M2+M4] JSON 문자열을 카테고리 값으로 변환합니다.
// "Sword" -> EquipCategory::Sword 등
// 원본 문자열을 훼손하지 않아야 합니다.
//
// (함수 이름: StringToCategory)
// (매개변수: 카테고리를 나타내는 문자열 1개)
// (반환값: 해당하는 카테고리 값)
// ------- 이 아래에 함수를 직접 작성하세요 -------
EquipCategory StringToCategory(const std::string &category_str) {
  // 사용처를 보면 RValue 를 그대로 삽입하고 있다.
  // 즉 수정이 애초에 불가능한 단순 값만을 넘기고 있기 때문에
  // const 로 수정이 안할거라는걸 명시해야한다.
  if (category_str == "Sword") {
    return EquipCategory::Sword;
  } else if (category_str == "Shield") {
    return EquipCategory::Shield;
  } else {
    return EquipCategory::Staff;
  }
}
// ------- 이 위에 함수를 직접 작성하세요 -------

int main() {
  // === [M10] JSON 데이터 파싱 (vcpkg: nlohmann-json) ===
  std::string rawData = R"({
        "equipment": [
            { "type": "weapon", "name": "Excalibur", "power": 100, "category": "Sword" },
            { "type": "weapon", "name": "Oak Staff", "power": 45, "category": "Staff" },
            { "type": "armor",  "name": "Dragon Shield", "defense": 80 },
            { "type": "weapon", "name": "Iron Dagger", "power": 20, "category": "Sword" }
        ]
    })";

  auto data = json::parse(rawData);
  Registry registry;

  // JSON 데이터를 순회하며 장비 객체를 생성하여 등록소에 넣습니다.
  for (const auto &entry : data["equipment"]) {
    if (entry["type"] == "weapon") {
      EquipCategory cat =
          StringToCategory(entry["category"].get<std::string>());
      auto weapon = std::make_unique<Weapon>(entry["name"].get<std::string>(),
                                             entry["power"].get<int>(), cat);
      registry.Register(std::move(weapon));
    } else if (entry["type"] == "armor") {
      auto armor = std::make_unique<Armor>(entry["name"].get<std::string>(),
                                           entry["defense"].get<int>());
      registry.Register(std::move(armor));
    }
  }

  // === [TDD] ===

  // TEST 1: [M6] static counter + size
  assert(registry.GetSize() == 4);
  assert(Registry::GetTotalRegistered() == 4);

  // TEST 2: [M8] sort descending by power
  registry.SortByPower();
  assert(registry.GetItemAt(0).GetPower() == 100); // Excalibur
  assert(registry.GetItemAt(1).GetPower() == 80);  // Dragon Shield
  assert(registry.GetItemAt(2).GetPower() == 45);  // Oak Staff
  assert(registry.GetItemAt(3).GetPower() == 20);  // Iron Dagger

  // TEST 3: [M3] Call by Reference
  int totalPower{0}; // [M1] Uniform Initialization
  registry.CalcTotalPower(totalPower);
  assert(totalPower == 245); // 100 + 45 + 80 + 20

  // TEST 4: [M2] switch-based category label
  assert(GetCategoryLabel(EquipCategory::Sword) == "Blade");
  assert(GetCategoryLabel(EquipCategory::Staff) == "Magic");
  assert(GetCategoryLabel(EquipCategory::Shield) == "Guard");

  // === [M10] (vcpkg: fmt) ===
  fmt::print("=== Guild Equipment Registry ===\n");
  registry.PrintAll();
  fmt::print("Total Power: {}\n", totalPower);
  fmt::print("Total Registered (static): {}\n", Registry::GetTotalRegistered());
  fmt::print("================================\n");

  fmt::print("[ALL TESTS PASSED]\n");
  return 0;
}
