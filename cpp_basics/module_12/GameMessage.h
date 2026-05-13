#pragma once
#include "ByteSerializer.h"
#include <cstdint>
#include <optional>
#include <vector>// [암시적 가이드]
// 캡슐화(은닉)보다는 묶음 데이터로서, 외부에서의 투명한 접근과 단순 데이터
// 전달이 메인 목적입니다. 데이터 손실 방지를 위해 엄격한 초기화를 적용하세요.
/* PlayerInfo, Position, LootInfo 구조체를 구현하세요. */
struct PlayerInfo
{
    // [코드 평가 피드백] (우아함 & 안정성)
    // (문제점) C++에서 구조체 멤버를 초기화하지 않으면 메모리에 남아있던
    // 쓰레기값(Garbage Value)이 그대로 들어갑니다. (왜 발생하는가?) C++은
    // 성능을 위해 변수를 선언할 때 해당 공간을 0으로 지우는 작업을 의도적으로
    // 생략합니다. 이 상태로 직렬화를 해버리면 의도치 않은 메모리 찌꺼기까지
    // 네트워크로 전송되어 보안 취약점이나 파싱 에러를 유발합니다. (수정 방법)
    // C++11/17의 기본 멤버 초기화(Default Member Initializer)를 사용하세요.
    // 수정 예시:
    // char name[64] = {0};
    // int id = 0;
    // int hp = 0;
    // int atk = 0;
    char name[64] = {0};
    int id = 0;
    int hp = 0;
    int atk = 0;
};
struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};
struct LootInfo
{
    int itemId = 0;
    int b = 0;
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
    // [코드 평가 피드백] (아키텍처 패턴 & 메모리 수명)
    // (문제점) 멤버 변수로 구조체를 참조(`&`)로 보관하고 있습니다.
    // (왜 발생하는가?) 어떤 함수 안에서 지역 변수 `PlayerInfo info;`를 만들고
    // 이를 `LoginMessage`에 넘겼다고 가정해봅시다. 함수가 끝나면 지역 변수
    // `info`는 스택 메모리에서 소멸(Destroy)되지만, `LoginMessage` 객체가
    // 어딘가의 큐(Queue)에 남아있다면 어떻게 될까요? `LoginMessage`가 가진
    // `&pInfo`는 이미 파괴된 쓰레기 메모리 주소를 가리키는 허상 참조(Dangling
    // Reference)가 되어, 나중에 이 값을 직렬화하려 할 때 엉뚱한 값을 읽거나
    // Segmentation Fault로 서버가 강제 종료됩니다. (수정 방법) 네트워크
    // 패킷/메시지는 데이터를 복사하여 스스로 완전한 '소유(Ownership)'를 가져야
    // 합니다. 수정 예시: 참조(&)를 떼고 값(Value)으로 저장하도록 변경하세요.
    // `PlayerInfo pInfo;` (생성자도 `LoginMessage(const PlayerInfo& pInfo_) :
    // pInfo(pInfo_)` 로 값 복사가 일어나게 변경)
    PlayerInfo pInfo;

public:
    LoginMessage(PlayerInfo &pInfo_) : GameMessage{MessageType::Login}, pInfo(pInfo_) {}
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<PlayerInfo> Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork = false);
    static std::optional<PlayerInfo> Deserialize(const PayloadView, bool isNetwork = false);
};

class PositionMessage : public GameMessage
{
private:
    Position position;

public:
    PositionMessage(Position &pos) : GameMessage{MessageType::PositionUpdate}, position{pos} {}
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<Position> Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork = false);
    static std::optional<Position> Deserialize(const PayloadView, bool isNetwork = false);
};

class LootMessage : public GameMessage
{
private:
    LootInfo lootInfo;

public:
    LootMessage(LootInfo &lootInfo) : GameMessage{MessageType::LootReport}, lootInfo{lootInfo} {}
    std::vector<uint8_t> Serialize(bool isNetwork = false) const override;
    static std::optional<LootInfo> Deserialize(const std::vector<uint8_t> &byteVec, bool isNetwork = false);
    static std::optional<LootInfo> Deserialize(const PayloadView, bool isNetwork = false);
};
