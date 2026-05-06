#pragma once
#include <cstdint>
#include <optional>
#include <vector>
// [암시적 가이드]
// 캡슐화(은닉)보다는 묶음 데이터로서, 외부에서의 투명한 접근과 단순 데이터
// 전달이 메인 목적입니다. 데이터 손실 방지를 위해 엄격한 초기화를 적용하세요.
/* PlayerInfo, Position, LootInfo 구조체를 구현하세요. */
struct PlayerInfo
{
    char name[64];
    int id;
    int hp;
    int atk;
};
struct Position
{
    float x;
    float y;
};
struct LootInfo
{
    int itemId;
    int b;
};

// [암시적 가이드]
// 오류 원천 차단을 위해, 메시지의 종류를 한정된 카테고리로 묶어 안전한 기호
// 집합으로 정의하세요.
/* MessageType 열거형을 구현하세요. */
enum class MessageType : uint8_t { Login, PositionUpdate, LootReport };

// [암시적 가이드]
// 전송 방식(신뢰성 우선 vs 속도 우선)을 안전한 기호 집합으로 구분하세요.
/* ProtocolType 열거형을 구현하세요. */
enum class ProtocolType { TCP, UDP };

// [암시적 가이드]
// 모든 메시지가 반드시 구현해야s 하는 직렬화 인터페이스를 강제하세요. 이
// 인터페이스는 직접 인스턴스화될 수 없습니다.
/* GameMessage 클래스를 구현하세요. */
class GameMessage
{
  protected:
    GameMessage(const GameMessage &other) = delete;
    GameMessage &operator=(const GameMessage &other) = delete;
    GameMessage(MessageType _type) : type{_type} {}

  public:
    virtual ~GameMessage() = default;
    virtual std::vector<uint8_t> Serialize(bool isNetwork = false) const = 0;
    MessageType GetType()
    {
        return this->type;
    }

  private:
    MessageType type;
};

// [암시적 가이드]
// 직렬화 함수: 게임 객체의 내부 데이터를 연속적인 바이트 배열로 변환합니다.
// 결과를 1차원으로 순차적, 연속적으로 보관하는 가변 길이 컨테이너에 담아
// 반환하세요.
// 역직렬화 함수: 바이트 배열로부터 게임 객체를 복원합니다. 복원
// 실패 가능성이 있으므로 '값이 있을 수도 비어있을 수도 있는' 안전상자로
// 포장하여 반환하세요.
/* LoginMessage, PositionMessage, LootMessage 클래스를 구현하세요. */
class LoginMessage : public GameMessage
{
  private:
    PlayerInfo &pInfo;

  public:
    LoginMessage(PlayerInfo &pInfo_)
        : GameMessage{MessageType::Login}, pInfo(pInfo_)
    {
    }
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<PlayerInfo> Deserialize(std::vector<uint8_t> &byteVec,
                                                 bool isNetwork = false);
};

class PositionMessage : public GameMessage
{
  private:
    Position &position;

  public:
    PositionMessage(Position &pos)
        : GameMessage{MessageType::PositionUpdate}, position{pos}
    {
    }
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<Position> Deserialize(std::vector<uint8_t> &byteVec,
                                               bool isNetwork = false);
};

class LootMessage : public GameMessage
{
  private:
    LootInfo &lootInfo;

  public:
    LootMessage(LootInfo &lootInfo)
        : GameMessage{MessageType::LootReport}, lootInfo{lootInfo}
    {
    }
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<LootInfo> Deserialize(std::vector<uint8_t> &byteVec,
                                               bool isNetwork = false);
};