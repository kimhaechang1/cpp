#include "GameMessage.h"
#include "ByteSerializer.h"
#include <cstddef>
#include <cstring>
#define REQUIRE_OR_NULL(opt_val)                                                                                       \
    if (!(opt_val).has_value())                                                                                        \
    {                                                                                                                  \
        return std::nullopt;                                                                                           \
    }

// 이 파일에 GameMessage와 파생 클래스들의 구현을 작성하세요.
std::vector<uint8_t> LoginMessage::Serialize(bool isNetwork) const
{
    ByteWriter writer(isNetwork);
    writer.WriteCharArray(this->pInfo.name, sizeof(this->pInfo.name));
    writer.WriteInt(this->pInfo.id);
    writer.WriteInt(this->pInfo.hp);
    writer.WriteInt(this->pInfo.atk);
    return writer.GetData();
}
std::optional<PlayerInfo> LoginMessage::Deserialize(const PayloadView pView, bool isNetwork)
// const Class parameter 시에 주의할점으로
// const 는 타입의 변수의 값을 절대 수정하지 않는다는것이다.
// 근데 타입이 클래스여서 가지고있는 메소드를 호출했다가 수정이 발생하면 어긋나버린다.
// 따라서 함수호출 또한 내부 자기자신을 변경하지 않는 함수들만호출해야한다.
{
    ByteReader reader(pView.GetPayloadPtr(), pView.GetPayloadSize(), isNetwork);
    PlayerInfo info;
    size_t nameLen = sizeof(info.name);
    reader.ReadCharArray(info.name, nameLen);
    auto idVal = reader.ReadInt();
    REQUIRE_OR_NULL(idVal);
    info.id = idVal.value();
    auto hpVal = reader.ReadInt();
    REQUIRE_OR_NULL(hpVal);
    info.hp = hpVal.value();
    auto atkVal = reader.ReadInt();
    REQUIRE_OR_NULL(atkVal);
    info.atk = atkVal.value();
    return info;
}
std::optional<PlayerInfo> LoginMessage::Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork)
{
    return Deserialize(PayloadView{byteVec.data(), byteVec.size()}, isNetwork);
}

std::vector<uint8_t> PositionMessage::Serialize(bool isNetwork) const
{
    ByteWriter writer(isNetwork);
    writer.WriteFloat(this->position.x);
    writer.WriteFloat(this->position.y);
    return writer.GetData();
}
std::optional<Position> PositionMessage::Deserialize(const PayloadView pView, bool isNetwork)
{
    ByteReader reader(pView.GetPayloadPtr(), pView.GetPayloadSize(), isNetwork);
    Position pos;
    auto xDesRt = reader.ReadFloat();
    REQUIRE_OR_NULL(xDesRt);
    pos.x = xDesRt.value();
    auto yDesRt = reader.ReadFloat();
    REQUIRE_OR_NULL(yDesRt);
    pos.y = yDesRt.value();
    return pos;
}
std::optional<Position> PositionMessage::Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork)
{
    return Deserialize(PayloadView{byteVec.data(), byteVec.size()}, isNetwork);
}

std::vector<uint8_t> LootMessage::Serialize(bool isNetwork) const
{
    ByteWriter writer(isNetwork);

    writer.WriteInt(this->lootInfo.itemId);
    writer.WriteInt(this->lootInfo.b);

    return writer.GetData();
}

std::optional<LootInfo> LootMessage::Deserialize(const PayloadView pView, bool isNetwork)
{
    LootInfo info;
    ByteReader reader(pView.GetPayloadPtr(), pView.GetPayloadSize(), isNetwork);
    auto itemIdVal = reader.ReadInt();
    REQUIRE_OR_NULL(itemIdVal);
    info.itemId = itemIdVal.value();
    auto bVal = reader.ReadInt();
    REQUIRE_OR_NULL(bVal);
    info.b = bVal.value();
    return info;
}
std::optional<LootInfo> LootMessage::Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork)
{
    return Deserialize(PayloadView{byteVec.data(), byteVec.size()}, isNetwork);
}