# Raid Selector Server 설계 문서

이 문서는 Module 13에서 실제 소켓 기반 selector 서버를 구현하기 위한 설계안이다.
목표는 `select()` 기반 I/O 루프 위에 간단한 레이드 게임 서버를 올려서, 여러 클라이언트가 접속하고 패킷을 주고받으며 보스 레이드를 진행하는 것이다.

## 1. 목표

- `RaidServer`는 `select()` 기반 단일 I/O 스레드로 클라이언트 접속, 수신, 송신을 처리한다.
- 게임 로직은 서버 내부 상태 머신으로 관리한다.
- 클라이언트는 `WarriorClient`, `MageClient` 역할을 가진다.
- 서버는 보스 몬스터를 하나 가진다.
- 보스는 `GamePlaying` 상태에서 4초마다 살아있는 랜덤 클라이언트 한 명에게 20 데미지를 준다.
- 보스 HP가 0이 되면 레이드는 성공으로 종료된다.
- 모든 플레이어가 죽고 보스가 살아있으면 레이드는 실패로 종료된다.
- `GameOver` 이후 서버는 공격을 무시하고 상태를 알린 뒤 다시 `GameReady`로 돌아간다.

## 2. 전체 구성

구현 파일은 다음과 같이 나누는 것을 추천한다.

```text
module_13/
  RaidPacket.h
  RaidClient.h
  RaidClient.cpp
  RaidBoss.h
  RaidBoss.cpp
  RaidServer.h
  RaidServer.cpp
  raid_server_main.cpp
  warrior_client_main.cpp
  mage_client_main.cpp
```

기존 실습 코드와 섞이지 않도록, 새 레이드 서버 전용 클래스를 별도로 만든다.
기존 `Session`, `SessionManager`, `BossMonster`, `CommandHandler`는 참고용으로 두고, 이번 설계에서는 소켓 서버 흐름에 맞춘 새 타입을 사용한다.

## 3. 서버 상태

`RaidServer`는 다음 상태를 가진다.

```cpp
enum class RaidState
{
    GameReady,
    GamePlaying,
    GameOver
};
```

### GameReady

게임 시작 전 상태다.

- 서버를 처음 켜면 `GameReady`로 시작한다.
- 클라이언트 접속은 허용한다.
- `StartGame` 패킷을 받으면 `GamePlaying`으로 전환한다.
- 공격 패킷은 처리하지 않고 "아직 게임이 시작되지 않았습니다." 같은 응답을 보낸다.
- 보스 공격 타이머는 동작하지 않는다.

### GamePlaying

레이드 진행 상태다.

- `Attack` 패킷을 받아 보스 HP를 감소시킨다.
- 공격 결과를 모든 클라이언트에게 브로드캐스트한다.
- 4초마다 보스가 살아있는 랜덤 클라이언트 한 명을 공격한다.
- 클라이언트 HP가 0이 되면 사망 상태로 저장하고 모든 클라이언트에게 알린다.
- 죽은 클라이언트의 `Attack` 패킷은 보스에게 적용하지 않는다.
- 보스 HP가 0이 되면 마지막 공격자를 기록하고 `GameOver`로 전환한다.
- 모든 플레이어가 죽고 보스가 살아있으면 레이드 실패로 보고 `GameOver`로 전환한다.

### GameOver

레이드 종료 상태다.

- 공격 패킷은 무시한다.
- 클라이언트에게 현재 남은 보스 HP와 레이드 종료 상태를 알려준다.
- 승리로 끝났다면 보스 HP는 0이다.
- 패배로 끝났다면 보스 HP를 원래 값으로 복구한다.
- 종료 메시지를 브로드캐스트한 뒤 `GameReady`로 돌아간다.
- 다음 `StartGame` 패킷을 받을 준비를 한다.

## 4. 패킷 설계

초기 구현은 텍스트 기반 프로토콜을 추천한다.
TCP는 메시지 경계가 없으므로, 각 패킷은 `\n`으로 끝나는 한 줄로 보낸다.

```text
TYPE|field1|field2|...
```

### 클라이언트에서 서버로 보내는 패킷

```text
JOIN|Warrior|Alice
JOIN|Mage|Bob
START
ATTACK|35
MESSAGE|hello everyone
RAID_STATUS
SERVER_STATUS
MY_INFO
BOSS_INFO
```

### 서버에서 클라이언트로 보내는 패킷

```text
WELCOME|Alice|Warrior|100
INFO|Alice joined raid.
MESSAGE|Game is not playing.
GAME_STATE|GameReady
ATTACK_RESULT|Alice|35|9965
BOSS_ATTACK|Bob|20|80
PLAYER_DEAD|Bob
RAID_STATUS|GamePlaying|BossHp=9965|Players=Alice:100:Alive,Bob:80:Alive
SERVER_STATUS|GameReady
MY_INFO|Alice|Warrior|140|Alive
BOSS_INFO|10000|10000|Alive
GAME_OVER|Victory|LastHit=Alice|BossHp=0
GAME_OVER|Defeat|BossHp=10000
ERROR|You are dead.
ERROR|Game is not playing.
```

### 패킷 enum

```cpp
enum class RaidPacketType
{
    Join,
    StartGame,
    Attack,
    Message,
    RaidStatus,
    ServerStatus,
    MyInfo,
    BossInfo,
    Unknown
};
```

`RaidPacket.h`에 다음 구조체를 둔다.

```cpp
struct RaidPacket
{
    RaidPacketType type;
    std::vector<std::string> fields;
    std::string raw;
};
```

처음에는 복잡한 직렬화보다 `std::string` split 기반으로 구현해도 충분하다.
단, TCP 수신 버퍼에서는 `\n` 단위로 패킷을 분리해야 한다.

## 5. 클라이언트 모델

서버 안에서 관리하는 클라이언트 정보는 `RaidClient`로 표현한다.

```cpp
enum class ClientClass
{
    Warrior,
    Mage,
    Unknown
};

enum class ClientLifeState
{
    Alive,
    Dead
};
```

```cpp
class RaidClient
{
private:
    SOCKET _socket;
    std::string _name;
    ClientClass _classType;
    ClientLifeState _lifeState;
    int _hp;
    std::string _recvBuffer;
    std::queue<std::string> _sendQueue;

public:
    RaidClient(SOCKET socket, std::string name, ClientClass classType);

    SOCKET GetSocket() const;
    const std::string& GetName() const;
    ClientClass GetClassType() const;
    int GetHp() const;
    bool IsAlive() const;
    bool IsDead() const;

    void TakeDamage(int damage);
    void ResetForNewRaid();

    void AppendRecvData(std::string data);
    std::optional<std::string> PopPacketLine();

    void PushSend(std::string packet);
    std::optional<std::string> PopSend();
    bool HasPendingSend() const;
};
```

### 직업별 차이

처음 구현에서는 클라이언트 실행 파일만 `WarriorClient`, `MageClient`로 나누고, 서버 안에서는 직업 정보를 저장만 해도 된다.

나중에 확장할 때는 다음처럼 차이를 줄 수 있다.

- Warrior: 기본 공격력 30, HP 140
- Mage: 기본 공격력 45, HP 80

초기 버전에서는 클라이언트가 `ATTACK|damage`를 보내고 서버가 damage 값을 그대로 사용한다.

## 6. 보스 모델

`RaidBoss`는 보스 HP와 공격 타이머 판단에 필요한 정보를 가진다.

```cpp
class RaidBoss
{
private:
    int _maxHp;
    int _hp;
    int _attackDamage;
    std::chrono::steady_clock::time_point _lastAttackTime;

public:
    RaidBoss(int maxHp = 10000, int attackDamage = 20);

    int GetHp() const;
    int GetMaxHp() const;
    int GetAttackDamage() const;
    bool IsDead() const;

    void TakeDamage(int damage);
    void Reset();

    bool CanAttackNow(std::chrono::steady_clock::time_point now) const;
    void MarkAttackTime(std::chrono::steady_clock::time_point now);
};
```

보스의 랜덤 타겟 선택은 `RaidServer`에서 처리한다.
이유는 살아있는 클라이언트 목록을 `RaidServer`가 알고 있기 때문이다.

## 7. RaidServer 책임

`RaidServer`는 네트워크 I/O와 게임 상태를 함께 조율한다.

```cpp
class RaidServer
{
private:
    SOCKET _listenSocket;
    fd_set _masterReadSet;
    fd_set _masterWriteSet;
    std::unordered_map<SOCKET, RaidClient> _clients;

    RaidBoss _boss;
    RaidState _state;
    std::mt19937 _rng;

public:
    RaidServer();
    ~RaidServer();

    bool Start(unsigned short port);
    void Run();

private:
    void AcceptClient();
    void ReceiveFromClient(SOCKET socket);
    void SendToClient(SOCKET socket);
    void DisconnectClient(SOCKET socket);

    void ProcessPacket(SOCKET socket, const RaidPacket& packet);

    void HandleJoin(SOCKET socket, const RaidPacket& packet);
    void HandleStartGame(SOCKET socket);
    void HandleAttack(SOCKET socket, const RaidPacket& packet);
    void HandleMessage(SOCKET socket, const RaidPacket& packet);
    void HandleRaidStatus(SOCKET socket);
    void HandleServerStatus(SOCKET socket);
    void HandleMyInfo(SOCKET socket);
    void HandleBossInfo(SOCKET socket);

    void TickBossAttack();
    RaidClient* PickRandomAliveClient();
    bool AreAllPlayersDead() const;

    void EnterGameReady();
    void EnterGamePlaying();
    void EnterGameOver(std::string reason, std::string lastHitter = "");

    void Broadcast(std::string packet);
    void QueueSend(SOCKET socket, std::string packet);
};
```

## 8. Selector 루프 설계

`select()` 루프는 다음 순서로 돈다.

```text
1. readSet, writeSet을 master set에서 복사한다.
2. select()를 짧은 timeout으로 호출한다.
3. listen socket이 readable이면 accept 한다.
4. client socket이 readable이면 recv 한다.
5. recvBuffer에서 '\n' 단위 패킷을 모두 꺼내 ProcessPacket 한다.
6. client socket이 writable이고 sendQueue가 있으면 send 한다.
7. GamePlaying 상태라면 TickBossAttack()을 호출한다.
```

권장 timeout:

```cpp
timeval timeout{};
timeout.tv_sec = 0;
timeout.tv_usec = 10000; // 10ms
```

보스 공격은 별도 스레드로 만들 수도 있지만, 초기 구현은 selector 루프 안에서 시간만 체크하는 방식이 단순하고 안전하다.

## 9. 핵심 게임 흐름

### 접속

1. 클라이언트가 서버에 TCP 연결한다.
2. 서버는 `AcceptClient()`에서 임시 `RaidClient`를 만든다.
3. 클라이언트가 `JOIN|Warrior|Alice` 또는 `JOIN|Mage|Bob`을 보낸다.
4. 서버는 이름과 직업을 저장하고 `WELCOME`을 응답한다.

### 게임 시작

1. 서버 상태는 `GameReady`다.
2. 아무 클라이언트가 `START`를 보낸다.
3. 서버는 모든 플레이어를 alive 상태와 full HP로 초기화한다.
4. 보스를 full HP로 초기화한다.
5. 상태를 `GamePlaying`으로 바꾼다.
6. 모든 클라이언트에게 `GAME_STATE|GamePlaying`을 보낸다.

### 플레이어 공격

1. 클라이언트가 `ATTACK|35`를 보낸다.
2. 서버 상태가 `GamePlaying`인지 확인한다.
3. 공격자가 살아있는지 확인한다.
4. 살아있다면 보스 HP를 감소시킨다.
5. 모든 클라이언트에게 `ATTACK_RESULT|name|damage|bossHp`를 보낸다.
6. 보스 HP가 0이면 `EnterGameOver("Victory", attackerName)`을 호출한다.

### 죽은 플레이어의 공격

1. 죽은 클라이언트가 `ATTACK|35`를 보낸다.
2. 서버는 보스 HP를 깎지 않는다.
3. 해당 클라이언트에게만 `ERROR|You are dead.`를 보낸다.

### 보스 공격

1. 서버가 `TickBossAttack()`에서 4초가 지났는지 확인한다.
2. 살아있는 클라이언트 중 랜덤으로 하나를 고른다.
3. 대상에게 20 데미지를 준다.
4. 모든 클라이언트에게 `BOSS_ATTACK|targetName|20|targetHp`를 보낸다.
5. 대상 HP가 0이면 `PLAYER_DEAD|targetName`을 브로드캐스트한다.
6. 모든 플레이어가 죽고 보스가 살아있으면 `EnterGameOver("Defeat")`을 호출한다.

### 레이드 종료

승리:

```text
GAME_OVER|Victory|LastHit=Alice|BossHp=0
```

패배:

```text
GAME_OVER|Defeat|BossHp=10000
```

패배 시에는 보스 HP를 원래 값으로 복구한다.
종료 메시지를 보낸 뒤 서버 상태는 다시 `GameReady`가 된다.

## 10. 상태 전이표

```text
GameReady
  START        -> GamePlaying
  ATTACK       -> GameReady, 공격 거부
  SERVER_STATUS -> GameReady, 서버 상태 응답
  MY_INFO      -> GameReady, 내 정보 응답
  BOSS_INFO    -> GameReady, 보스 정보 응답
  RAID_STATUS  -> GameReady, 상태 응답

GamePlaying
  ATTACK by alive player -> 보스 HP 감소
  ATTACK by dead player  -> 공격자에게 사망 응답
  SERVER_STATUS          -> 현재 서버 상태 응답
  MY_INFO                -> 내 정보 응답
  BOSS_INFO              -> 보스 정보 응답
  boss hp == 0           -> GameOver -> GameReady
  all players dead       -> GameOver -> GameReady
  RAID_STATUS            -> 현재 상태 응답

GameOver
  ATTACK       -> 공격 무시, 남은 보스 HP 응답
  SERVER_STATUS -> GameOver 또는 곧 GameReady 상태 응답
  MY_INFO      -> 내 정보 응답
  BOSS_INFO    -> 남은 보스 HP 응답
  after notify -> GameReady
```

## 11. 클라이언트 메뉴 설계

`WarriorClient`와 `MageClient`는 서버에 접속한 뒤 항상 입력 대기 상태로 머문다.
클라이언트는 서버 상태를 직접 판단하지 않고, 사용자의 선택을 패킷으로 바꿔 서버에 보낸다.
서버가 현재 상태에 따라 받아들일지 거절할지를 결정한다.

클라이언트 콘솔 메뉴는 다음처럼 고정한다.

```text
===== Raid Client =====
1. 서버상태 확인
2. 나의 정보
3. 보스 정보
4. 공격
5. 게임시작
6. 참가
q. 종료
> 
```

### 메뉴와 요청 패킷 매핑

```text
1 -> SERVER_STATUS
2 -> MY_INFO
3 -> BOSS_INFO
4 -> ATTACK|damage
5 -> START
6 -> JOIN|Warrior|name 또는 JOIN|Mage|name
```

`4. 공격`을 선택했을 때는 클라이언트가 추가로 데미지를 입력받는다.

```text
damage> 35
```

`6. 참가`를 선택했을 때는 클라이언트가 이름을 입력받는다.
직업은 실행 파일에 따라 자동으로 정한다.

```text
warrior_client.exe -> JOIN|Warrior|Alice
mage_client.exe    -> JOIN|Mage|Bob
```

### 중요한 설계 원칙

클라이언트는 1~6번 메뉴를 언제든 누를 수 있다.
하지만 서버가 모든 요청을 항상 성공 처리하지는 않는다.

예를 들어 서버가 `GameReady` 상태인데 클라이언트가 `4. 공격`을 누르면:

```text
client -> server
ATTACK|35

server -> client
MESSAGE|현재 게임중이 아닙니다.
```

클라이언트는 `MESSAGE` 패킷을 받으면 그 내용을 그대로 콘솔에 출력한다.

```text
[Server] 현재 게임중이 아닙니다.
```

즉 클라이언트는 메뉴 UI와 입력 담당이고, 게임 규칙 판단은 전부 서버가 맡는다.

## 12. 메뉴별 서버 응답 규칙

### 1. 서버상태 확인

요청:

```text
SERVER_STATUS
```

응답 예시:

```text
SERVER_STATUS|GameReady
SERVER_STATUS|GamePlaying
SERVER_STATUS|GameOver
```

이 요청은 모든 상태에서 허용한다.

### 2. 나의 정보

요청:

```text
MY_INFO
```

참가 전 응답:

```text
MESSAGE|아직 레이드에 참가하지 않았습니다.
```

참가 후 응답:

```text
MY_INFO|Alice|Warrior|140|Alive
MY_INFO|Bob|Mage|90|Dead
```

이 요청은 모든 상태에서 허용한다.

### 3. 보스 정보

요청:

```text
BOSS_INFO
```

응답:

```text
BOSS_INFO|10000|10000|Alive
BOSS_INFO|7500|10000|Alive
BOSS_INFO|0|10000|Dead
```

필드 의미:

```text
BOSS_INFO|현재HP|최대HP|상태
```

이 요청은 모든 상태에서 허용한다.

### 4. 공격

요청:

```text
ATTACK|35
```

`GameReady` 상태 응답:

```text
MESSAGE|현재 게임중이 아닙니다.
```

`GamePlaying` 상태이지만 참가하지 않은 클라이언트:

```text
MESSAGE|레이드에 먼저 참가해야 합니다.
```

`GamePlaying` 상태이지만 죽은 클라이언트:

```text
MESSAGE|당신은 현재 죽었습니다.
```

`GamePlaying` 상태의 살아있는 클라이언트:

```text
ATTACK_RESULT|Alice|35|9965
```

공격 결과는 모든 클라이언트에게 브로드캐스트한다.

`GameOver` 상태 응답:

```text
MESSAGE|레이드가 종료되었습니다. 남은 보스 체력: 0
```

### 5. 게임시작

요청:

```text
START
```

`GameReady` 상태에서는 허용한다.

```text
GAME_STATE|GamePlaying
MESSAGE|레이드가 시작되었습니다.
```

`GamePlaying` 상태에서는 거절한다.

```text
MESSAGE|이미 레이드가 진행중입니다.
```

`GameOver` 상태에서는 서버가 정리 중이면 거절하거나, 이미 `GameReady`로 돌아간 뒤 새 시작 요청으로 처리한다.
초기 구현에서는 `GameOver` 메시지 브로드캐스트 직후 바로 `GameReady`로 돌리는 방식을 추천한다.

### 6. 참가

요청:

```text
JOIN|Warrior|Alice
JOIN|Mage|Bob
```

참가 성공:

```text
WELCOME|Alice|Warrior|140
MESSAGE|Alice 님이 레이드에 참가했습니다.
```

이미 참가한 클라이언트:

```text
MESSAGE|이미 레이드에 참가했습니다.
```

`GamePlaying` 중 참가를 허용할지는 정책으로 정해야 한다.
초기 구현에서는 단순하게 `GameReady`에서만 참가 가능하게 한다.

```text
MESSAGE|게임 진행중에는 참가할 수 없습니다.
```

## 13. 구현 순서

1. `RaidPacket.h` 작성
   - 패킷 타입 enum
   - `ParseRaidPacket(std::string line)`
   - `MakePacket(...)` 보조 함수

2. `RaidClient.h/.cpp` 작성
   - 소켓, 이름, 직업, HP, 생존 상태
   - 수신 버퍼와 송신 큐
   - `\n` 기준 패킷 추출

3. `RaidBoss.h/.cpp` 작성
   - HP 감소, 리셋, 4초 공격 타이머

4. `RaidServer.h/.cpp` 작성
   - Winsock 초기화
   - listen socket 생성
   - non-blocking 설정
   - select loop
   - accept, recv, send

5. 게임 상태 처리 추가
   - `HandleStartGame`
   - `HandleAttack`
   - `HandleRaidStatus`
   - `TickBossAttack`
   - `EnterGameOver`

6. `warrior_client_main.cpp`, `mage_client_main.cpp` 작성
   - 접속 후 자동으로 `JOIN` 전송
   - 콘솔 입력을 패킷으로 변환
   - 서버 응답 출력

7. `CMakeLists.txt`에 실행 파일 추가
   - `raid_server`
   - `warrior_client`
   - `mage_client`

## 14. 수동 테스트 시나리오

### 시나리오 A: 접속과 시작

1. `raid_server.exe` 실행
2. `warrior_client.exe Alice` 실행
3. `mage_client.exe Bob` 실행
4. Alice가 메뉴 `6. 참가` 선택
5. Bob이 메뉴 `6. 참가` 선택
6. Alice가 메뉴 `5. 게임시작` 선택
5. 모든 클라이언트가 `GAME_STATE|GamePlaying`을 받는지 확인

### 시나리오 B: 공격

1. Alice가 메뉴 `4. 공격` 선택 후 `30` 입력
2. Bob이 메뉴 `4. 공격` 선택 후 `45` 입력
3. 모든 클라이언트가 공격자, 데미지, 남은 보스 HP를 받는지 확인

### 시나리오 B-1: 게임 시작 전 공격 거부

1. 서버를 켠다.
2. 클라이언트를 접속한다.
3. 아직 게임을 시작하지 않는다.
4. 클라이언트가 메뉴 `4. 공격`을 선택한다.
5. 서버가 `MESSAGE|현재 게임중이 아닙니다.`를 응답하는지 확인한다.

### 시나리오 B-2: 메뉴 상태 조회

1. 어느 상태에서든 메뉴 `1. 서버상태 확인`을 누른다.
2. 서버가 `SERVER_STATUS|GameReady` 같은 응답을 주는지 확인한다.
3. 메뉴 `2. 나의 정보`, `3. 보스 정보`도 모든 상태에서 응답하는지 확인한다.

### 시나리오 C: 보스 공격

1. GamePlaying 상태에서 4초 이상 기다린다.
2. 랜덤 플레이어가 20 데미지를 받는지 확인
3. 모든 클라이언트에게 `BOSS_ATTACK`이 전달되는지 확인

### 시나리오 D: 플레이어 사망

1. 보스 공격으로 특정 플레이어 HP를 0까지 떨어뜨린다.
2. `PLAYER_DEAD|name`이 브로드캐스트되는지 확인
3. 죽은 플레이어가 `attack 30`을 입력한다.
4. 죽은 플레이어에게만 `ERROR|You are dead.`가 오는지 확인

### 시나리오 E: 레이드 승리

1. 공격 데미지를 크게 입력해서 보스 HP를 0으로 만든다.
2. `GAME_OVER|Victory|LastHit=name|BossHp=0`이 브로드캐스트되는지 확인
3. 서버가 다시 `GameReady` 상태가 되는지 확인

### 시나리오 F: 레이드 실패

1. 플레이어들이 공격하지 않고 보스 공격만 맞는다.
2. 모든 플레이어가 죽으면 `GAME_OVER|Defeat|BossHp=10000`이 오는지 확인
3. 보스 HP가 원상 복구되는지 확인
4. 서버가 다시 `GameReady` 상태가 되는지 확인

## 15. 구현 시 주의점

- TCP는 한 번의 `recv()`가 한 패킷이라는 보장이 없다.
  - 반드시 클라이언트별 `_recvBuffer`에 누적한다.
  - `\n`을 찾을 때마다 한 줄 패킷으로 꺼낸다.

- `send()`도 한 번에 모든 데이터를 보낸다는 보장이 없다.
  - 초기 구현에서는 짧은 문자열만 보내므로 단순화할 수 있다.
  - 확장하려면 송신 버퍼와 보낸 바이트 수를 따로 관리한다.

- `select()`에서 write set은 송신 큐가 비어있지 않은 소켓만 넣는다.
  - 항상 모든 소켓을 write set에 넣으면 대부분 즉시 writable이라 루프가 바쁘게 돈다.

- 게임 상태 변경은 `RaidServer` 한 곳에서만 한다.
  - `RaidClient`와 `RaidBoss`는 자기 데이터만 관리한다.

- 보스 공격 타이머는 `GamePlaying` 상태에서만 체크한다.

- 죽은 클라이언트도 접속은 유지한다.
  - 메시지와 상태 조회는 가능하다.
  - 공격만 거부한다.

- 클라이언트 메뉴는 항상 같은 1~6번을 보여준다.
  - 현재 상태에서 가능한 행동인지 판단하는 책임은 서버에 둔다.
  - 서버는 거절 사유를 `MESSAGE` 패킷으로 보내고, 클라이언트는 그대로 출력한다.

- `GameOver`가 되었다고 클라이언트 소켓을 끊지 않는다.
  - 같은 접속으로 다음 레이드를 시작할 수 있게 한다.

## 16. 최소 완성 기준

다음이 가능하면 1차 구현 완료로 본다.

- 서버 실행 후 여러 클라이언트 접속 가능
- `JOIN`, `START`, `ATTACK`, `MESSAGE`, `RAID_STATUS` 처리 가능
- `GameReady -> GamePlaying -> GameOver -> GameReady` 순환 가능
- 보스가 4초마다 랜덤 생존자 공격
- 죽은 플레이어 공격 거부
- 보스 사망 시 마지막 공격자 포함 승리 메시지 브로드캐스트
- 모든 플레이어 사망 시 패배 메시지 브로드캐스트 및 보스 HP 복구

## 17. 추천 확장

1차 구현 이후에는 다음을 추가해볼 수 있다.

- 직업별 기본 공격력과 최대 HP 차이
- `HEAL` 패킷 추가
- `READY` 시스템 추가
- 방 여러 개 관리
- length-prefix 기반 바이너리 패킷
- worker thread pool로 무거운 게임 로직 분리
- 서버 로그 파일 저장
