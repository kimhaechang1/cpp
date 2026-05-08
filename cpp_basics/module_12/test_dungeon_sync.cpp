#include "GameMessage.h"
#include "PacketBuilder.h"
#include "SessionManager.h"
#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// =======================================================================
// [안내] 이 테스트가 모두 통과하도록 구현 파일(.h, .cpp)을 완성하세요.
// =======================================================================

// 1. PlayerInfo 객체를 LoginMessage로 감싸 직렬화
TEST(DungeonSync, SerializePlayerLogin)
{
    PlayerInfo pInfo;
    pInfo.id = 100;
    pInfo.hp = 150;
    pInfo.atk = 20;
    std::strncpy(pInfo.name, "Hero", sizeof(pInfo.name));

    LoginMessage msg(pInfo);

    EXPECT_EQ(msg.GetType(), MessageType::Login);

    std::vector<uint8_t> serializedData = msg.Serialize();
    EXPECT_EQ(serializedData.size(), sizeof(PlayerInfo));

    PlayerInfo recovered;
    std::memcpy(&recovered, serializedData.data(), sizeof(PlayerInfo));
    EXPECT_EQ(recovered.id, 100);
    EXPECT_EQ(std::string(recovered.name), "Hero");
}

// 2. 바이트 배열을 역직렬화하여 객체 복원
TEST(DungeonSync, DeserializePlayerLogin)
{
    PlayerInfo pInfo;
    pInfo.id = 200;
    pInfo.hp = 80;
    pInfo.atk = 50;
    std::strncpy(pInfo.name, "Mage", sizeof(pInfo.name));

    std::vector<uint8_t> buffer(sizeof(PlayerInfo));
    std::memcpy(buffer.data(), &pInfo, sizeof(PlayerInfo));

    auto optPlayer = LoginMessage::Deserialize(buffer);
    ASSERT_TRUE(optPlayer.has_value());
    EXPECT_EQ(optPlayer->id, 200);
    EXPECT_EQ(optPlayer->hp, 80);
    EXPECT_EQ(std::string(optPlayer->name), "Mage");

    // 잘못된 크기의 버퍼 테스트
    std::vector<uint8_t> badBuffer(10);
    auto badOpt = LoginMessage::Deserialize(badBuffer);
    EXPECT_FALSE(badOpt.has_value());
}

// 3. 다형성(Polymorphism)을 이용한 직렬화
TEST(DungeonSync, PolymorphicSerialization)
{
    Position pos{10.5f, 20.0f};
    LootInfo loot{999, 5};

    std::vector<std::unique_ptr<GameMessage>> messages;
    messages.push_back(std::make_unique<PositionMessage>(pos));
    messages.push_back(std::make_unique<LootMessage>(loot));

    EXPECT_EQ(messages[0]->GetType(), MessageType::PositionUpdate);
    EXPECT_EQ(messages[1]->GetType(), MessageType::LootReport);

    auto posData = messages[0]->Serialize();
    auto lootData = messages[1]->Serialize();

    EXPECT_EQ(posData.size(), sizeof(Position));
    EXPECT_EQ(lootData.size(), sizeof(LootInfo));
}

// 4. 패킷 빌더를 이용한 프로토콜 헤더 조립 및 파싱
TEST(DungeonSync, BuildAndParsePacket)
{
    LootInfo loot{101, 1};
    LootMessage msg(loot);

    PacketBuilder builder;
    auto packetPtr = builder.BuildPacket(msg);
    ASSERT_NE(packetPtr, nullptr);

    // Header size: 1 byte (Type) + 4 bytes (Length) = 5 bytes
    size_t expectedSize = 5 + sizeof(LootInfo);
    EXPECT_EQ(packetPtr->size(), expectedSize);

    auto [parsedType, parsedPayload] = PacketBuilder::ParsePacket(*packetPtr);
    EXPECT_EQ(parsedType, MessageType::LootReport);
    EXPECT_EQ(parsedPayload.size(), sizeof(LootInfo));

    auto recoveredLoot = LootMessage::Deserialize(parsedPayload, true);
    ASSERT_TRUE(recoveredLoot.has_value());
    EXPECT_EQ(recoveredLoot->itemId, 101);
}

// 5. 메시지 종류에 따른 라우팅 결정
TEST(DungeonSync, MessageTypeRouting)
{
    SessionManager manager;
    EXPECT_EQ(manager.RouteMessage(MessageType::Login), ProtocolType::TCP);
    EXPECT_EQ(manager.RouteMessage(MessageType::PositionUpdate),
              ProtocolType::UDP);
    EXPECT_EQ(manager.RouteMessage(MessageType::LootReport), ProtocolType::TCP);
}

// 6. 스레드 안전한 세션 관리와 람다를 이용한 필터링
TEST(DungeonSync, SessionManagement)
{
    SessionManager manager;
    manager.RegisterSession(1, "Alice", 100);
    manager.RegisterSession(2, "Bob", 30);
    manager.RegisterSession(3, "Charlie", 80);

    auto nameOpt = manager.GetSessionName(2);
    ASSERT_TRUE(nameOpt.has_value());
    EXPECT_EQ(*nameOpt, "Bob");

    manager.RemoveSession(2);
    EXPECT_FALSE(manager.GetSessionName(2).has_value());

    // 체력이 50 초과인 세션 필터링
    auto healthyIds =
        manager.FilterSessions([](int id, int hp) { return hp > 50; });

    EXPECT_EQ(healthyIds.size(), 2);
    // 1번과 3번이 남아있어야 함
    bool hasAlice = false, hasCharlie = false;
    for (int id : healthyIds)
    {
        if (id == 1)
            hasAlice = true;
        if (id == 3)
            hasCharlie = true;
    }
    EXPECT_TRUE(hasAlice && hasCharlie);
}

// 7. jthread를 이용한 동시성 제어 검증
TEST(DungeonSync, ThreadSafeSessionAccess)
{
    SessionManager manager;
    std::vector<std::jthread> threads;

    for (int i = 0; i < 100; ++i)
    {
        threads.emplace_back([&manager, i]() {
            manager.RegisterSession(i, "User" + std::to_string(i), 100);
        });
    }

    // jthread 소멸 시 자동 join 됨
    threads.clear();

    // 100명이 모두 안전하게 등록되었는지 확인 (단순 카운트용 필터)
    auto allIds = manager.FilterSessions([](int, int) { return true; });
    EXPECT_EQ(allIds.size(), 100);
}

// 8. [도전] 실제 UDP 루프백 통신 테스트
TEST(DungeonSync, UdpLoopbackPositionSync)
{
    WSADATA wsaData;
    ASSERT_EQ(WSAStartup(MAKEWORD(2, 2), &wsaData), 0);

    SOCKET udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ASSERT_NE(udpSock, INVALID_SOCKET);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // 바인드 (수신용)
    ASSERT_NE(bind(udpSock, (sockaddr *)&addr, sizeof(addr)), SOCKET_ERROR);

    // 데이터 준비
    Position pos{15.0f, -5.5f};
    PositionMessage msg(pos);

    PacketBuilder builder;
    auto packetPtr = builder.BuildPacket(msg);

    // 송신 (자신에게 보내기)
    int sendResult =
        sendto(udpSock, reinterpret_cast<const char *>(packetPtr->data()),
               static_cast<int>(packetPtr->size()), 0, (sockaddr *)&addr,
               sizeof(addr));
    EXPECT_EQ(sendResult, packetPtr->size());

    // 수신 대기 (타임아웃 설정)
    DWORD timeout = 1000; // 1 second
    setsockopt(udpSock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
               sizeof(timeout));

    std::vector<uint8_t> recvBuffer(1024);
    sockaddr_in fromAddr;
    int fromLen = sizeof(fromAddr);

    int recvResult =
        recvfrom(udpSock, reinterpret_cast<char *>(recvBuffer.data()),
                 static_cast<int>(recvBuffer.size()), 0, (sockaddr *)&fromAddr,
                 &fromLen);

    ASSERT_GT(recvResult, 0);
    recvBuffer.resize(recvResult);

    // 파싱 및 역직렬화 검증
    auto [type, payload] = PacketBuilder::ParsePacket(recvBuffer);
    EXPECT_EQ(type, MessageType::PositionUpdate);

    auto recoveredOpt = PositionMessage::Deserialize(payload, true);
    ASSERT_TRUE(recoveredOpt.has_value());
    EXPECT_FLOAT_EQ(recoveredOpt->x, 15.0f);
    EXPECT_FLOAT_EQ(recoveredOpt->y, -5.5f);

    closesocket(udpSock);
    WSACleanup();
}
