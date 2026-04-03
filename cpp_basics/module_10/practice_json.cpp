#include <cassert>
#include <fmt/core.h>
#include <iostream>
#include <nlohmann/json.hpp> // 👈 vcpkg로 설치한 라이브러리!

/**
 * [MODULE 10 실습] vcpkg 라이브러리 연동 (JSON)
 *
 * 목표: nlohmann-json 라이브러리를 사용하여 무기 데이터를 JSON 객체로 만들고
 * 검증합니다.
 */

using json = nlohmann::json;

int main() {
  // 1. TODO: json 객체를 사용하여 아래 정보를 담은 'IronSword' 데이터를
  // 만드세요.
  // - name: "Iron Sword"
  // - power: 15
  // - owner: { "name": "Antigravity", "level": 10 }

  json weaponData;
  // (이곳에 코드를 작성하세요)
  weaponData["name"] = "Iron Sword";
  weaponData["power"] = 15;
  weaponData["owner"]["name"] = "Antigravity";

  // 2. 검증 코드 (TDD-style)
  assert(weaponData["name"] == "Iron Sword");
  assert(weaponData["power"] == 15);
  assert(weaponData["owner"]["name"] == "Antigravity");

  // 3. 출력 (fmt 라이브러리와 JSON 조화!)
  // weaponData.dump(4) 함수는 4칸 들여쓰기된 문자열을 반환합니다.
  fmt::print("Successfully created JSON weapon data!\n");
  fmt::print("{}\n", weaponData.dump(4));

  return 0;
}
