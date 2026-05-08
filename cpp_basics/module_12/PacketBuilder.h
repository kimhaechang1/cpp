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
        
        // [코드 평가 피드백] (객체 지향 - 단일 책임 원칙 SRP)
        // (문제점) 클래스 이름은 패킷의 헤더와 페이로드를 붙이는 'Builder(조립공)'인데, 내부에서 `htonl`이라는 '바이트 직렬화(엔디안 변환)' 연산을 직접 수행하고 있습니다.
        // (왜 발생하는가?) 헤더(데이터 길이)를 4바이트 정수로 바꿔서 리틀엔디안/빅엔디안을 뒤집는 로직을 빌더 안에 섞어버렸기 때문입니다. 이렇게 되면 나중에 엔디안 처리 방식이 바뀌거나 플랫폼이 변경되었을 때 `ByteSerializer`뿐만 아니라 `PacketBuilder` 코드까지 뜯어고쳐야 하는 '강결합(Tight Coupling)' 문제가 발생합니다.
        // (수정 방법) 헤더 작성 자체도 `ByteSerializer`의 `ByteStream` 객체에게 책임(Responsibility)을 완전히 위임해야 합니다.
        // 수정 예시: 
        // ByteStream headerStream(true); // isNetwork = true
        // headerStream.WriteMsgType(type);
        // headerStream.WriteInt(static_cast<int>(dt.size())); // 여기서 자동으로 htonl 처리가 됨
        // 이 headerStream 버퍼 뒤에 dt 버퍼를 이어붙임.
        uint32_t datSize = htonl(static_cast<uint32_t>(dt.size()));
        uint8_t *bytes = reinterpret_cast<uint8_t *>(&datSize);
        _dats.insert(_dats.end(), bytes, bytes + sizeof(uint32_t));
        _dats.insert(_dats.end(), dt.begin(), dt.end());
    }
    size_t size()
    {
        return this->_dats.size();
    }
    uint8_t *data()
    {
        return this->_dats.data();
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
        // [코드 평가 피드백] (Zero-Copy 뷰 반환 아키텍처)
        // (문제점) `payload`를 리턴하기 위해 새로운 `std::vector`를 생성하고 `insert`를 통해 버퍼 메모리를 통째로 반복 복사하고 있습니다.
        // (왜 발생하는가?) 리턴 타입을 물리적인 데이터 컨테이너인 `std::vector`로 정했기 때문에, 데이터를 넘겨주려면 새 벡터를 만들고 Heap 메모리 할당(malloc) 및 `memcpy`를 할 수밖에 없는 구조입니다. 하지만 단순히 역직렬화(읽기)만 할 데이터라면 복사할 필요 없이 원본 버퍼의 "시작 주소부터 어디까지"라는 범위 정보만 넘겨주면 메모리 할당 비용이 0이 됩니다.
        // (수정 방법) C++17에서는 `std::string_view`를 응용하거나(이름은 string이지만 바이트 배열 뷰로 쓸 수 있음), 혹은 나만의 `ArrayView` 구조체(포인터+크기)를 만들어 복사 없이 반환하도록 설계해야 고성능 서버가 됩니다.
        // 수정 예시 (C++17 구조체 뷰 사용):
        // 구조체 선언: `struct PayloadView { const uint8_t* data; size_t size; };`
        // 리턴 타입 및 반환 로직 변경: `return { type, PayloadView{ byteVec.data() + idx, payloadSize } };`
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
        // [코드 평가 피드백]
        // 위와 동일하게 Zero-Copy를 지향하는 구조로 설계하는 것이 실무 고성능 서버의 핵심입니다.
        payload.reserve(payloadSize);
        payload.insert(payload.end(), packetBytes.begin() + idx,
                       packetBytes.begin() + idx + payloadSize);
        stream.EndRead();
        return std::make_pair(type, payload);
    }
};