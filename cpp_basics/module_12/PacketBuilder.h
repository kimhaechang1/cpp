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

class Packet
{
private:
    std::vector<uint8_t> _dats;
    MessageType tp;

public:
    Packet(std::vector<uint8_t> dt, MessageType type) : tp{type}
    {
        // 1. 헤더 만들기
        ByteWriter headerWriter(true);
        headerWriter.WriteMsgType(type);
        headerWriter.WriteInt(static_cast<int>(dt.size()));

        // 2. 헤더 버퍼 소유권 가져오기 (이동)
        _dats = headerWriter.GetData();

        // 3. 메모리 재할당 방지를 위해 필요한 총 공간 미리 예약 (핵심 최적화⭐)
        _dats.reserve(_dats.size() + dt.size());

        // 4. 페이로드 이어 붙이기 (이 부분만 유일하게 1회 덩어리 복사가 발생함)
        _dats.insert(_dats.end(), dt.begin(), dt.end());
    }
    size_t size()
    {
        return _dats.size();
    }
    uint8_t *data()
    {
        return _dats.data();
    }
    std::vector<uint8_t> &GetData()
    {
        return _dats;
    }
};

class PacketBuilder
{
public:
    std::unique_ptr<Packet> BuildPacket(GameMessage &msg)
    {
        return std::make_unique<Packet>(msg.Serialize(true), msg.GetType());
    }
    static std::pair<MessageType, PayloadView> ParsePacket(std::vector<uint8_t> &byteVec)
    {

        ByteReader reader(byteVec.data(), byteVec.size(), true);
        auto msgTypeVal = reader.ReadMsgType();
        MessageType type = msgTypeVal.value();
        auto payloadSizeVal = reader.ReadInt();
        int payloadSize = payloadSizeVal.value();

        // [코드 평가 피드백] (Zero-Copy 뷰 반환 아키텍처)
        // (문제점) `payload`를 리턴하기 위해 새로운 `std::vector`를 생성하고 `insert`를 통해 버퍼 메모리를 통째로 반복
        // 복사하고 있습니다. (왜 발생하는가?) 리턴 타입을 물리적인 데이터 컨테이너인 `std::vector`로 정했기 때문에,
        // 데이터를 넘겨주려면 새 벡터를 만들고 Heap 메모리 할당(malloc) 및 `memcpy`를 할 수밖에 없는 구조입니다. 하지만
        // 단순히 역직렬화(읽기)만 할 데이터라면 복사할 필요 없이 원본 버퍼의 "시작 주소부터 어디까지"라는 범위 정보만
        // 넘겨주면 메모리 할당 비용이 0이 됩니다. (수정 방법) C++17에서는 `std::string_view`를 응용하거나(이름은
        // string이지만 바이트 배열 뷰로 쓸 수 있음), 혹은 나만의 `ArrayView` 구조체(포인터+크기)를 만들어 복사 없이
        // 반환하도록 설계해야 고성능 서버가 됩니다. 수정 예시 (C++17 구조체 뷰 사용): 구조체 선언: `struct PayloadView
        // { const uint8_t* data; size_t size; };` 리턴 타입 및 반환 로직 변경: `return { type, PayloadView{
        // byteVec.data() + idx, payloadSize } };`
        ;
        return std::make_pair(
            type, PayloadView{byteVec.data() + reader.GetCurrentCursor(), static_cast<size_t>(payloadSize)});
    }
    static std::pair<MessageType, PayloadView> ParsePacket(Packet &packet)
    {
        std::vector<uint8_t> &packetBytes = packet.GetData();
        ByteReader reader(packetBytes.data(), packetBytes.size(), true);
        auto msgTypeVal = reader.ReadMsgType();
        MessageType type = msgTypeVal.value();
        auto payloadSizeVal = reader.ReadInt();
        int payloadSize = payloadSizeVal.value();
        return std::make_pair(
            type, PayloadView{packetBytes.data() + reader.GetCurrentCursor(), static_cast<size_t>(payloadSize)});
    }
};