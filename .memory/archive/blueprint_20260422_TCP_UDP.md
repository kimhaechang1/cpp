# Practice Blueprint

## Meta
- **Type**: cumulative_practice
- **Module**: 12
- **Topic**: TCP/UDP Game Data Serialization & Sync
- **Persona**: basic_tutor
- **TDD Mode**: gtest

## Integrated Concepts (통합 출제 맵)

- **[Core - M12]** TCP/UDP 소켓 통신: 메시지 타입에 따라 TCP(`send`/`recv`) 또는 UDP(`sendto`/`recvfrom`)로 라우팅. UDP 루프백으로 실제 전송/수신 검증.
- **[M1]** `struct` + Uniform Init: 게임 데이터 구조체(`PlayerInfo`, `Position`, `LootInfo`)를 `{}`로 안전하게 초기화.
- **[M2]** `switch`문: 수신된 패킷의 메시지 타입(enum)에 따라 처리 분기.
- **[M3]** Call by Reference: 직렬화 함수가 외부 바이트 버퍼를 참조로 받아 내용을 채움.
- **[M4]** 포인터 캐스팅 + `memcpy`: 구조체를 `char*` 바이트 배열로 변환 (직렬화의 핵심).
- **[M5]** Stack vs Heap: 가변 길이 페이로드를 힙에 할당하되 RAII로 감싸 누수 방지.
- **[M6]** 클래스 RAII + 생성자/소멸자: `PacketBuilder` 클래스가 내부 버퍼의 수명을 관리.
- **[M7]** 다형성 + 순수 가상 함수: `GameMessage` 추상 클래스 → `LoginMessage`, `PositionMessage`, `LootMessage` 파생. 각각 자신만의 직렬화 규칙 구현.
- **[M8]** `std::vector` + 람다: 세션 목록에서 특정 조건의 플레이어를 필터링.
- **[M9]** `std::unique_ptr` + `std::optional`: 메시지 객체를 독점 소유권으로 관리, 역직렬화 실패 시 빈 안전상자 반환.
- **[M10]** GTest: 테스트 프레임워크.
- **[M11]** `std::mutex` + `std::jthread`: 세션 목록 동시 접근 보호.

## Scenario (시나리오)

### "Dungeon Sync Protocol (DSP)" - 던전 데이터 동기화 프로토콜

당신은 멀티플레이어 던전 크롤러의 **데이터 직렬화 레이어**를 구축하는 미들웨어 개발자입니다.
게임 객체(플레이어, 몬스터, 위치)가 네트워크를 통해 전송되려면, 먼저 **평탄한 바이트 배열(Flat Byte Array)**로 변환(직렬화)되어야 하고, 도착지에서 다시 원래의 객체로 복원(역직렬화)되어야 합니다.

**스토리 흐름:**
1. 플레이어가 서버에 **로그인**합니다 → `LoginMessage`를 TCP로 전송 (신뢰성 필수)
2. 던전 내에서 플레이어의 **실시간 위치**가 업데이트됩니다 → `PositionMessage`를 UDP로 전송 (속도 우선)
3. 몬스터를 처치하면 **전리품 보고서**가 생성됩니다 → `LootMessage`를 TCP로 전송 (데이터 무결성)
4. 서버의 `SessionManager`가 모든 접속자를 관리하며, 특정 조건의 플레이어를 검색합니다

**핵심 데이터 흐름:**
```
GameData(struct) → GameMessage(polymorphic) → PacketBuilder(serialize + header)
    → NetworkBridge(TCP or UDP) → [Network] → PacketBuilder(parse)
    → GameMessage(deserialize) → GameData(struct)
```

## File Structure (파일 구조)

- `GameMessage.h`: 추상 메시지 클래스 + 파생 메시지 클래스 선언 + 게임 데이터 구조체 (학습자 구현)
- `GameMessage.cpp`: 각 메시지 타입의 직렬화/역직렬화 구현 (학습자 구현)
- `PacketBuilder.h`: 프로토콜 패킷 조립/분해 클래스 선언 (학습자 구현)
- `PacketBuilder.cpp`: 패킷 빌드/파싱 로직 구현 (학습자 구현)
- `SessionManager.h`: 스레드 안전 세션 관리자 선언 (학습자 구현)
- `SessionManager.cpp`: 세션 등록/조회/필터링 구현 (학습자 구현)
- `test_dungeon_sync.cpp`: GTest 테스트 파일 (에이전트 제공)
- `CMakeLists.txt`: 기존 파일 수정 (새 타겟 추가)

## Test Cases (테스트 케이스 설계)

> 모든 테스트는 하나의 던전 시나리오 안에서 스토리 순서대로 진행됩니다.

1. **TEST(DungeonSync, SerializePlayerLogin)**
   - 목적: PlayerInfo 구조체를 LoginMessage로 감싸고 바이트 배열로 직렬화
   - 기대 동작: 직렬화된 바이트 배열의 크기가 PlayerInfo의 sizeof와 일치. 원본 데이터와 바이트 비교 일치.

2. **TEST(DungeonSync, DeserializePlayerLogin)**
   - 목적: 직렬화된 바이트 배열을 다시 PlayerInfo로 복원 (std::optional 활용)
   - 기대 동작: 복원된 PlayerInfo의 id, hp, atk가 원본과 일치. 잘못된 데이터(빈 버퍼)로 역직렬화 시 빈 optional 반환.

3. **TEST(DungeonSync, PolymorphicSerialization)**
   - 목적: GameMessage 포인터(unique_ptr)를 통해 Login, Position, Loot 메시지를 각각 직렬화
   - 기대 동작: 각 메시지 타입이 자신의 GetType()을 올바르게 반환. 다형적 Serialize() 호출이 각각 다른 크기의 바이트 배열 생성.

4. **TEST(DungeonSync, BuildAndParsePacket)**
   - 목적: PacketBuilder가 [MessageType(1B)][PayloadLength(4B)][Payload] 형태로 패킷 조립 및 파싱
   - 기대 동작: 빌드된 패킷의 총 크기 = 5 + payload 크기. 파싱 후 메시지 타입과 페이로드가 원본과 일치.

5. **TEST(DungeonSync, MessageTypeRouting)**
   - 목적: switch문으로 메시지 타입에 따라 TCP/UDP 프로토콜 결정
   - 기대 동작: Login → TCP, PositionUpdate → UDP, LootReport → TCP. 모든 enum 값 소비 검증.

6. **TEST(DungeonSync, SessionManagement)**
   - 목적: SessionManager에 플레이어 등록/조회/제거 + 람다로 HP 기준 필터링
   - 기대 동작: 3명 등록 후 ID로 조회 성공, 제거 후 조회 실패. HP > 50인 플레이어만 필터링.

7. **TEST(DungeonSync, ThreadSafeSessionAccess)**
   - 목적: 여러 jthread가 동시에 세션을 등록/조회할 때 레이스 컨디션 없음을 검증
   - 기대 동작: 10개의 스레드가 각각 세션을 등록한 후, 총 세션 수가 정확히 10개.

8. **TEST(DungeonSync, UdpLoopbackPositionSync)**
   - 목적: 실제 UDP 루프백(127.0.0.1)을 통해 PositionMessage를 직렬화 → 전송 → 수신 → 역직렬화
   - 기대 동작: 송신한 Position(x, y)과 수신 후 복원한 Position이 정확히 일치.

## Comment Guide (주석 가이드 - Blank Slate Policy)

> 아래 문구를 스켈레톤 코드의 주석에 사용합니다. 정답 키워드를 노출하지 않습니다.

- **게임 데이터 구조체**: "캡슐화(은닉)보다는 묶음 데이터로서, 외부에서의 투명한 접근과 단순 데이터 전달이 메인 목적입니다. 데이터 손실 방지를 위해 엄격한 초기화를 적용하세요."
- **메시지 타입 분류**: "오류 원천 차단을 위해, 메시지의 종류를 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요."
- **프로토콜 타입 분류**: "전송 방식(신뢰성 우선 vs 속도 우선)을 안전한 기호 집합으로 구분하세요."
- **추상 메시지 클래스**: "모든 메시지가 반드시 구현해야 하는 직렬화 인터페이스를 강제하세요. 이 인터페이스는 직접 인스턴스화될 수 없습니다."
- **직렬화 함수**: "게임 객체의 내부 데이터를 연속적인 바이트 배열로 변환합니다. 결과를 1차원으로 순차적, 연속적으로 보관하는 가변 길이 컨테이너에 담아 반환하세요."
- **역직렬화 함수**: "바이트 배열로부터 게임 객체를 복원합니다. 복원 실패 가능성이 있으므로 '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장하여 반환하세요."
- **PacketBuilder**: "프로토콜 헤더(메시지 종류 + 페이로드 길이)를 붙여 완전한 네트워크 패킷을 조립합니다. 내부 버퍼는 스택 수명을 벗어나는 즉시 자동으로 해제되는 독점 메모리 래퍼로 관리하세요."
- **SessionManager**: "접속한 플레이어를 특정 Key를 통해 극강의 속도로 등록/조회/제거합니다. 병렬 실행되는 작업자들이 동시에 접근해도 데이터가 오염되지 않아야 합니다."
- **세션 필터링**: "함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용 로직 덩어리를 만들어 필터링 조건으로 던져 넣으세요."
- **메시지 라우팅**: "메시지 종류에 따라 전송 프로토콜을 결정합니다. 한정된 카테고리 각각에 대해 빠짐없이 분기 처리하세요."
- **메시지 소유권**: "생성된 메시지 객체는 스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는 독점 메모리 래퍼로 관리하세요."

## Constraints (제약 사항)

- **금지 키워드 (주석/가이드에서)**: `vector`, `map`, `unique_ptr`, `optional`, `mutex`, `jthread`, `virtual`, `override`, `abstract`, `lambda`, `enum class` 등 구체적 C++ 키워드를 직접 노출하지 않음.
- **패킷 프로토콜 규격**: 헤더는 정확히 5바이트 ([MessageType: 1B][PayloadLength: 4B]), 페이로드는 가변 길이.
- **구조체 직렬화 방식**: 단순 `memcpy` 기반 (endianness 변환은 이번 실습에서는 localhost 전용이므로 생략 가능, 단 개념적으로 실제 환경에서는 필요함을 주석으로 언급).
- **이름 필드**: `char name[32]` 고정 크기 배열 사용 (std::string은 직렬화가 복잡하므로 이 단계에서는 회피).
- **난이도 조절**: 8개 테스트 중 1~5번은 핵심(직렬화/패킷), 6~7번은 세션관리, 8번은 도전(실제 네트워크). 학습자가 순서대로 하나씩 통과시키도록 유도.

## Cohesion Checklist (설계 통합성 검증)

1. **데이터 흐름**: PlayerInfo → LoginMessage → PacketBuilder → (TCP/UDP) → PacketBuilder → LoginMessage → PlayerInfo. 모든 클래스가 이 파이프라인에서 역할을 수행.
2. **Enum 완전 소비**: MessageType의 3개 값(Login, PositionUpdate, LootReport) 모두 Test 3, 5에서 검증. ProtocolType의 2개 값(TCP, UDP) 모두 Test 5에서 검증.
3. **반환값 활용**: Serialize()의 반환(바이트 배열)은 PacketBuilder의 입력이 됨. Deserialize()의 반환(optional)은 값 존재 여부 확인 후 필드 비교에 사용.
4. **시나리오 일관성**: 로그인 → 위치 업데이트 → 몬스터 처치 → 세션 관리의 흐름이 Test 1~8 순서와 일치.
5. **Cross-Module 연결**: 다형적 메시지(M7)를 직렬화(M4)하여 패킷(M6 RAII)으로 감싸고, 세션(M8 map + M11 mutex)을 통해 라우팅(M2 switch)하여 UDP(M12)로 전송. 모든 개념이 하나의 파이프라인에서 유기적으로 연결.
