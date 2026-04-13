#include "Monster.h"
// [Review] <optional>은 Monster.h에서 이미 include 하므로 중복 제거

// ---------------------------------------------------------
// [Monster.cpp]
//
// 여기에 Monster.h에서 선언한 클래스들의 메서드들을 구현하세요.
// 팁: Goblin과 Dragon의 생성자에서 HP를 각각 50, 500으로 초기화해야 합니다.
// ---------------------------------------------------------

// (구현부 - 학습자가 작성)
std::optional<std::string> Dragon::GetDropItem()
{
    return "Dragon Heart";
}

std::optional<std::string> Goblin::GetDropItem()
{
    return std::nullopt;
}
