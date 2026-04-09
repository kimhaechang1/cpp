#pragma once
#include "Commit.h"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <vector>

// 필요에 따라 다양한 헤더를 추가하세요.

class CommitAnalyzer
{
  public:
    // [M9] 커밋의 소유권을 통째로 강탈(이동)하여 저장소에 등록합니다.
    // 무거운 데이터를 복사하여 CPU를 낭비하지 마세요.
    // (RegisterCommit)
    void RegisterCommit(std::unique_ptr<Commit> commit)
    {
        this->m_commitMap.insert(
            std::make_pair(commit->GetHash(), std::move(commit)));
    }

    // [M9] 해시 문자열을 통해 커밋을 찾습니다.
    // 에러 방지를 위해 nullptr 대신 '값이 있을 수도 비어있을 수도 있는'
    // 안전상자로 포장해 읽기 전용 참조(또는 포인터)를 반환하세요.
    // (FindByHash)
    std::optional<Commit *> FindByHash(std::string &&hash)
    {
        for (const auto &[hashKey, commitPtr] : this->m_commitMap) {
            if (hashKey == hash) {
                return commitPtr.get();
            }
        }
        return std::nullopt;
    }

    // [M8] 함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용
    // 로직 덩어리(Predicate) 를 만들어 넘겨받으세요. 불필요한 복사 없이 읽기
    // 전용 참조(포인터) 목록을 반환하세요. (FilterByType)
    const std::vector<Commit *>
    FilterByType(const std::function<bool(Commit *)> fn)
    {
        return this->m_commitMap | std::views::values |
               std::views::transform([](std::unique_ptr<Commit> &commit) {
                   return commit.get();
               }) |
               std::views::filter(fn) |

               std::ranges::to<std::vector<Commit *>>();
    }

    // 특정 요일에 커밋 개수 기록
    void RecordCommitOnDay(int dayIndex, int count);

    // [M5] 주간 통계 고정 배열 전체를 읽을 수 있도록 반환하세요.
    // (GetWeeklyStats)
    const std::array<int, 7> &GetWeeklyStats() const
    {
        return this->m_weeklyStats;
    }

    // [M4] 원본 배열의 소유권은 갖지 않은 채, 연속된 데이터 중 평일(인덱스
    // 0~4)의 '시야(View)'만 대여받아 요소를 읽을 수 있도록 반환하세요.
    // (GetWeekdayStats)
    const std::span<int> GetWeekdayStats()
    {
        std::span<int> s{this->m_weeklyStats};
        return s.subspan(0, 5);
    }

    // 디버깅용: 저장소에 있는 요소 전체 개수
    int GetTotalCount() const;

  private:
    // [M8] 항상 정렬된 상태를 자동 유지하며 특정 Key(해시 문자열)로
    // Value(스택 수명 스코프를 벗어나는 즉시 스스로 메모리를 해제하는 래퍼)를
    // 찾는 컨테이너를 사용하세요.
    // (이름 예시: m_commitMap)
    std::map<std::string, std::unique_ptr<Commit>> m_commitMap;

    // [M5] 7일이라는 고정된 하드웨어 크기를 가지는 연속 메모리 블록을
    // 사용하세요. (이름 예시: m_weeklyStats)
    std::array<int, 7> m_weeklyStats{0};
};
