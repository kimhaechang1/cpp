# 🔥 BEGINNER PROJECT: 텍스트 RPG 전투 시스템 (절차지향)

## 🎯 프로젝트 목표
지금까지 배운 **변수, 제어문(if/while), 함수**를 모두 활용하여 간단한 1:1 턴제 전투 시스템을 완성합니다.

## 📋 요구사항 (Requirements)
1. **Player**와 **Monster**의 상태(이름, HP, 공격력)를 구조체(`struct`)로 정의해야 합니다.
2. 게임은 플레이어와 몬스터 중 둘 중 하나가 쓰러질 때까지(`HP <= 0`) 계속됩니다 (`while` 반복문).
3. 각 턴마다 플레이어가 먼저 공격하고, 몬스터가 살아있다면 반격합니다.
4. 전투와 관련된 로직은 반드시 **함수**로 분리되어야 합니다.

## ⚙️ 구현해야 할 함수 목록
- `CreatePlayer()`: 플레이어 초기 상태 생성 및 반환
- `CreateMonster()`: 몬스터 초기 상태 생성 및 반환
- `Attack(attacker, defender)`: 공격 수행 및 데미지 차감 (핵심!)
- `IsDead(character)`: 캐릭터가 죽었는지 판별 (true/false)
- `PrintBattleInfo()`: 현재 양쪽의 HP 상황 출력

## 🚀 시작 방법
`project_rpg/main.cpp` 파일을 열면 스켈레톤 코드(함수 껍데기)가 준비되어 있습니다.
주석(`// TODO`)을 읽고 빈 부분을 직접 채워보세요!
