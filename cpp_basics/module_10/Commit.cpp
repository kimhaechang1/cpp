#include "Commit.h"

// ---------------------------------------------------------------------
// 여기에 Commit 및 파생 클래스들의 멤버 함수, 정적 변수 초기화 등을 구현하세요.
// ---------------------------------------------------------------------

// [M6] 정적 카운터 변수 초기화 (예: int Commit::s_commitCount{0};)

void Commit::ResetCounter()
{
    // 여기에 카운터를 0으로 초기화하는 코드를 작성하세요 (테스트용)
    Commit::s_commitCount = 0;
}

int Commit::GetCounter()
{
    // 여기에 카운터 반환 코드를 작성하세요
    return Commit::s_commitCount;
}
std::ostringstream &operator<<(std::ostringstream &oss, Commit &commit)
{
    oss << "[" << commit.GetTypeLabel() << "] " << commit.GetHash() << ": "
        << commit.GetMessage();

    return oss;
}