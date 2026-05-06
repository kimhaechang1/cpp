#include "GameMessage.h"
#include "ByteSerializer.h"
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