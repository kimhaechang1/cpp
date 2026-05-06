#pragma once
#include "ByteSerializer.h"
#include "GameMessage.h"
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include <winsock.h>

// [암시적 가이드]
// 프로토콜 헤더(메시지 종류 + 페이로드 길이)를 붙여 완전한 네트워크 패킷을
// 조립합니다. 내부 버퍼는 스택 수명을 벗어나는 즉시 자동으로 해제되는 독점
// 메모리 래퍼로 관리하세요.
/* PacketBuilder 클래스를 구현하세요. */
struct _Packet
{
  private:
    std::vector<uint8_t> _dats;
    MessageType tp;

  public:
    _Packet(std::vector<uint8_t> dt, MessageType type) : tp{type}
    {
        _dats.reserve(sizeof(MessageType) + sizeof(uint32_t) + dt.size());
        _dats.push_back(static_cast<uint8_t>(type));
        uint32_t datSize = htonl(static_cast<uint32_t>(dt.size()));
        uint8_t *bytes = reinterpret_cast<uint8_t *>(&datSize);
        _dats.insert(_dats.end(), bytes, bytes + sizeof(uint32_t));
        _dats.insert(_dats.end(), dt.begin(), dt.end());
    }
    size_t size()
    {
        return this->_dats.size();
    }
    std::vector<uint8_t> &GetData()
    {
        return this->_dats;
    }
};
class PacketBuilder
{
  public:
    std::unique_ptr<_Packet> BuildPacket(GameMessage &msg)
    {
        return std::make_unique<_Packet>(msg.Serialize(true), msg.GetType());
    }
    static std::pair<MessageType, std::vector<uint8_t>>
    ParsePacket(std::vector<uint8_t> &byteVec)
    {
        int idx = 0;
        ByteStream stream(byteVec, true);
        stream.BeginRead(idx);
        auto msgTypeVal = stream.ReadMsgType(idx);
        MessageType type = msgTypeVal.value();
        auto payloadSizeVal = stream.ReadInt(idx);
        std::vector<uint8_t> payload;
        int payloadSize = payloadSizeVal.value();
        payload.reserve(payloadSize);
        payload.insert(payload.end(), byteVec.begin() + idx,
                       byteVec.begin() + idx + payloadSize);
        stream.EndRead();
        return std::make_pair(type, payload);
    }
    static std::pair<MessageType, std::vector<uint8_t>>
    ParsePacket(_Packet &packet)
    {
        int idx = 0;
        std::vector<uint8_t> &packetBytes = packet.GetData();
        ByteStream stream(packetBytes, true);
        stream.BeginRead(idx);
        auto msgTypeVal = stream.ReadMsgType(idx);
        MessageType type = msgTypeVal.value();
        auto payloadSizeVal = stream.ReadInt(idx);
        std::vector<uint8_t> payload;
        int payloadSize = payloadSizeVal.value();
        payload.reserve(payloadSize);
        payload.insert(payload.end(), packetBytes.begin() + idx,
                       packetBytes.begin() + idx + payloadSize);
        stream.EndRead();
        return std::make_pair(type, payload);
    }
};