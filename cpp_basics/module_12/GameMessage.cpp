#include "GameMessage.h"
#include "ByteSerializer.h"
// [코드 평가 피드백] (의존성 관리)
// (문제점) `GameMessage.h`에서 이미 `<optional>`, `<vector>`, `<cstdint>`를 인클루드하고 있습니다.
// 헤더에 있는 내용을 cpp에서 또 인클루드하는 것은 컴파일 시간을 늘리는 주요 원인입니다. (C++ 프로젝트가 커질수록 컴파일 속도 저하 체감이 큼)
// (개선 방향) 헤더에서 보장된 인클루드는 cpp에서 과감히 지우는 것이 깔끔합니다.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <vector>

// 이 파일에 GameMessage와 파생 클래스들의 구현을 작성하세요.
std::vector<uint8_t> LoginMessage::Serialize(bool isNetwork) const
{
    ByteStream stream(isNetwork);
    stream.BeginWrite();
    stream.WriteCharArray(this->pInfo.name, sizeof(this->pInfo.name));
    stream.WriteInt(this->pInfo.id);
    stream.WriteInt(this->pInfo.hp);
    stream.WriteInt(this->pInfo.atk);
    stream.EndWrite();
    return stream.GetData();
}
std::optional<PlayerInfo>
LoginMessage::Deserialize(std::vector<uint8_t> &byteVec, bool isNetwork)
{
    ByteStream stream(byteVec, isNetwork);
    int lastIdx = 0;
    stream.BeginRead(lastIdx);
    PlayerInfo info;
    size_t nameLen = sizeof(info.name);
    stream.ReadCharArray(info.name, nameLen, lastIdx);
    auto idVal = stream.ReadInt(lastIdx);
    // [코드 평가 피드백] (코드 우아함 & 패턴)
    // (문제점) 파싱에 실패할 때마다 `has_value()`를 검사하는 보일러플레이트(반복 코드)가 끝없이 등장하여 가독성이 크게 떨어집니다.
    // (왜 발생하는가?) `std::optional` 껍질을 벗기기 전에 안전한지 확인해야 하기 때문에 구조상 어쩔 수 없이 발생하는 `if`문 도배 현상입니다.
    // (수정 방법) C++17 환경에서는 매크로(Macro)나 헬퍼 함수를 만들어 이 반복을 한 줄로 줄이는 것이 현업의 정석입니다.
    // 수정 예시: 파일 상단에 매크로 정의
    // #define REQUIRE_READ(opt_val, stream_obj) if(!(opt_val).has_value()) { (stream_obj).EndRead(); return std::nullopt; }
    // 
    // 사용 예시:
    // auto idVal = stream.ReadInt(lastIdx);
    // REQUIRE_READ(idVal, stream);
    // info.id = idVal.value();
    if (!idVal.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    info.id = idVal.value();
    auto hpVal = stream.ReadInt(lastIdx);
    if (!hpVal.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    info.hp = hpVal.value();
    auto atkVal = stream.ReadInt(lastIdx);
    if (!atkVal.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    info.atk = atkVal.value();
    stream.EndRead();
    return info;
}

std::vector<uint8_t> PositionMessage::Serialize(bool isNetwork) const
{
    ByteStream stream(isNetwork);
    stream.BeginWrite();
    stream.WriteFloat(this->position.x);
    stream.WriteFloat(this->position.y);
    stream.EndWrite();
    return stream.GetData();
}
std::optional<Position>
PositionMessage::Deserialize(std::vector<uint8_t> &byteVec, bool isNetwork)
{
    int idx = 0;
    ByteStream stream(byteVec, isNetwork);
    stream.BeginRead(idx);
    Position pos;
    auto xDesRt = stream.ReadFloat(idx);
    if (!xDesRt.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    pos.x = xDesRt.value();
    auto yDesRt = stream.ReadFloat(idx);
    if (!yDesRt.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    pos.y = yDesRt.value();
    stream.EndRead();
    return pos;
}

std::vector<uint8_t> LootMessage::Serialize(bool isNetwork) const
{
    ByteStream stream(isNetwork);
    stream.BeginWrite();
    stream.WriteInt(this->lootInfo.itemId);
    stream.WriteInt(this->lootInfo.b);
    stream.EndWrite();
    return stream.GetData();
}

std::optional<LootInfo> LootMessage::Deserialize(std::vector<uint8_t> &byteVec,
                                                 bool isNetwork)
{
    int idx = 0;
    LootInfo info;
    ByteStream stream(byteVec, isNetwork);
    stream.BeginRead(idx);
    auto itemIdVal = stream.ReadInt(idx);
    if (!itemIdVal.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    info.itemId = itemIdVal.value();
    auto bVal = stream.ReadInt(idx);
    if (!bVal.has_value())
    {
        stream.EndRead();
        return std::nullopt;
    }
    info.b = bVal.value();
    stream.EndRead();
    return info;
}