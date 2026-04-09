#include "CommitAnalyzer.h"

// ---------------------------------------------------------------------
// 여기에 CommitAnalyzer의 멤버 함수들을 구현하세요.
// ---------------------------------------------------------------------

void CommitAnalyzer::RecordCommitOnDay(int dayIndex, int count)
{
    // 구현하세요
    this->m_weeklyStats[dayIndex] = count;
}

int CommitAnalyzer::GetTotalCount() const
{
    // 구현하세요
    return this->m_commitMap.size();
}
