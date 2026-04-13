#pragma once
#include "Monster.h"
#include <format>
#include <memory>
#include <mutex>
#include <span>
#include <stop_token>
#include <string>
#include <vector>

/**
 * [M1] FloorState 열거형
 * 상태(State)들을 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요.
 * Idle(대기), InProgress(공략중), Cleared(클리어) 상태가 필요합니다.
 */
// (enum class FloorState 정의)
enum class FloorState { Idle, InProgress, Cleared };

/**
 * [M6/M11] DungeonRaid 클래스
 * 던전 탐험대의 중앙 전투 기록부입니다. 여러 공격대(스레드)가 동시에
 * 접근하는 공유 자원을 안전하게 관리합니다.
 */
class DungeonRaid
{
  public:
    /**
     * [M6] 생성자
     * 생성자에서 초기화 시 대입(Assignment) 비용을 제거하는 엄격한 초기화
     * 방식을 사용하세요.
     */
    // (Floor 번호를 인자로 받는 생성자 선언)
    DungeonRaid(int floorNumber_)
        : floorNumber{floorNumber_}, state{FloorState::Idle}, killCount{0}
    {
    }

    /**
     * [M11] 처치 기록 (RecordKill)
     * 데이터가 오염될 수 있으므로, 내 작업이 끝날 때까지 아무도 진입하지
     * 못하도록 독점 보호막(자물쇠)을 쳐서 안전하게 보호하세요. 반드시 예외
     * 안전한 RAII 방식의 자물쇠 관리자를 사용하세요.
     */
    // (RecordKill 함수 선언)
    void RecordKill()
    {
        std::lock_guard<std::mutex> lock(this->m_lock);
        this->state = FloorState::InProgress;
        this->killCount++;
        this->totalGlobalKills++;
    }

    /**
     * [M1] 전투 보고서 (GetReport)
     * 문자열 템플릿 내부에 중괄호 {}를 열어 값을 주입하는 모던 C++ 문자열
     * 조립 기법을 쓰세요.
     */
    // (GetReport 함수 선언)
    // [Review] 객체를 수정하지 않으므로 const 멤버 함수로 선언
    std::string GetReport() const
    {
        return std::format("Floor {}, kill: {}", this->floorNumber,
                           this->killCount);
    }

    // (기타 필요한 Getter들: GetKillCount, GetState, GetTotalGlobalKills
    // 선언)
    // [Review] 값 반환(int, enum)에서 const 반환은 무의미 - 복사본이므로 수정 여부 무관
    int GetKillCount() const
    {
        return this->killCount;
    }
    FloorState GetState() const
    {
        return this->state;
    }
    static int GetTotalGlobalKills()
    {
        return totalGlobalKills;
    }

    /**
     * [M6] static 통계 초기화
     * 테스트 편의를 위해 전역 통계를 0으로 리셋하는 정적 함수가 필요합니다.
     */
    // (static ResetGlobalKills 함수 선언)
    static void ResetGlobalKills()
    {
        totalGlobalKills = 0;
    }

  private:
    int floorNumber;
    /**
     * [M6] killCount
     * 무결성을 위해 데이터 외부 접근을 금지하고 객체 자체적으로만 통제되는
     * 처치 카운터를 가집니다.
     */
    // (killCount 필드)
    int killCount;

    // (floorNumber 필드)
    // (state 필드)
    FloorState state;

    /**
     * [M11] mutex
     * 수백 명의 공격대(스레드)가 동시에 호출할 수 있습니다.
     * 데이터 보호를 위한 자물쇠를 선언하세요.
     */
    // (mutable std::mutex 필드)
    // [Review] mutable: const 멤버 함수에서도 mutex를 잠글 수 있게 함
    mutable std::mutex m_lock;

    /**
     * [M6] static 처치 통계
     * 수많은 탐험대 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만 존재하여
     * 전역적으로 공유되어야 합니다.
     */
    // (static totalGlobalKills 필드)
    static int totalGlobalKills;
};

/**
 * [M11] 탐험 공략 작업 (ExecuteRaid)
 *
 * 1. 단일 렌더링 흐름을 멈추지 않고 나란히 병렬 실행되는 백그라운드 작업자를
 *    소환하기 위한 전용 함수입니다.
 * 2. 이 함수가 외부의 '중단 깃발'을 볼 수 있도록 알맞은 타입의 매개변수를
 *    첫 번째 자리에 선언하세요.
 * 3. [M4] std::span을 사용하여 몬스터 목록을 안전하게 읽어와야 합니다.
 * 4. 반복 작업 도중 틈틈이 외부에서 깃발이 올라왔는지 확인하여,
 *    신호가 오면 현재 상태를 정리하고 안전하게 리턴하세요.
 */
// (ExecuteRaid 자유 함수 선언)
void ExecuteRaid(std::stop_token stoken,
                 std::span<const std::unique_ptr<Monster>> mPtrs,
                 DungeonRaid &raid);
