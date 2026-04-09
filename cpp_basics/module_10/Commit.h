#pragma once
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

// [M1] 오류 원천 차단을 위해, 커밋의 종류(신규 기능 추가, 버그 수정)를
// 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요.
// (여기에 CommitType 등 이름으로 정의, Feature, Bugfix 포함)
enum class CommitType { Feature, Bugfix };

// [M7] 인스턴스화가 불가능한 인터페이스 설계법을 사용하세요.
class Commit
{
  public:
    // [Safety] 이 클래스의 사용 패턴을 고려하여, 필요하다면 소멸자를
    // 설계하세요.
    virtual ~Commit()
    {
    }
    Commit(const Commit &other)
        : m_hash{other.m_hash}, m_message{other.m_message},
          m_author{other.m_author}
    {
        Commit::s_commitCount++;
    }

    Commit &operator=(const Commit &other)
    {
        if (this == &other) {
            return *this;
        }
        this->m_hash = other.m_hash;
        this->m_message = other.m_message;
        this->m_author = other.m_author;
        Commit::s_commitCount++;
        return *this;
    }
    Commit(Commit &&other)
        : m_hash{std::move(other.m_hash)},
          m_message{std::move(other.m_message)},
          m_author{std::move(other.m_author)}
    {
    }
    Commit &operator=(Commit &&other)
    {
        if (this == &other) {
            return *this;
        }
        this->m_hash = other.m_hash;
        this->m_message = other.m_message;
        this->m_author = other.m_author;
        other.m_hash = "";
        other.m_message = "";
        other.m_author = "";
        return *this;
    }
    // [M7] 커밋의 종류를 문자열로 반환하는 행위를 자식 클래스가 반드시
    // 구현하도록 강제하세요. (GetTypeLabel)
    virtual std::string GetTypeLabel() const = 0;

    // [M7] 커밋의 종류를 기호 집합(예: CommitType)으로 반환하는 행위를 자식
    // 클래스가 반드시 구현하도록 강제하세요. (GetType)
    virtual CommitType GetType() const = 0;

    // [M2] 커밋의 종류에 따라 서로 다른 우선순위 점수를 반환해야 합니다.
    // 종류별로 갈라지는 다중 경로 분기를 사용하세요. (GetPriority)
    // - Feature 타입일 경우 1 반환
    // - Bugfix 타입일 경우 3 반환
    virtual int GetPriority() const = 0;

    // [M4] 해시, 메시지, 작성자를 밖에서 읽을 수 있어야 합니다. 불필요한 복사
    // 없이. (GetHash, GetMessage, GetAuthor)
    const std::string &GetHash()
    {
        return this->m_hash;
    }
    const std::string &GetMessage()
    {
        return this->m_message;
    }
    const std::string &GetAuthor()
    {
        return this->m_author;
    }

    // 상태 확인용 정적 카운터 (구현부에서 초기화)
    static void ResetCounter();
    static int GetCounter();

    // [M6] 외부(main, 테스트)에서 private 멤버에 접근하여 커밋의 타입, 해시,
    // 메시지를 예쁘게 스트림에 출력할 수 있도록 예외적 허용을 사용하세요.
    // (operator<< 구현 시 "[타입(문자열)] 해시: 메시지" 포맷으로 출력하세요)
    friend std::ostringstream &operator<<(std::ostringstream &oss,
                                          Commit &commit);

  protected:
    // [M1] 초기화 시 데이터 손실 발생을 막기 위해 컴파일 에러를 강제하는 엄격한
    // 초기화 문법을 사용하세요.
    std::string m_hash{""};
    std::string m_message{""};
    std::string m_author{""};
    Commit(std::string m_hash_, std::string m_message_, std::string m_author_)
        : m_hash{m_hash_}, m_message{m_message_}, m_author{m_author_}
    {
        s_commitCount++;
    }

    // [M6] 수많은 커밋 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만
    // 존재하여 전역적으로 공유되어야 하는 카운터입니다. (이름 예시:
    // s_commitCount)
    static inline int s_commitCount = 0;
};

// [M7] 조상 클래스의 인터페이스를 구체적으로 실현하는 파생 클래스입니다.
// (FeatureCommit)
class FeatureCommit : public Commit
{
  private:
    CommitType commitType;

  public:
    virtual ~FeatureCommit()
    {
    }
    FeatureCommit(std::string m_hash_, std::string m_message_,
                  std::string m_author_)
        : Commit{m_hash_, m_message_, m_author_},
          commitType{CommitType::Feature}
    {
    }

    FeatureCommit(const FeatureCommit &other)
        : Commit{other}, commitType{other.GetType()}
    {
    }

    FeatureCommit &operator=(const FeatureCommit &other)
    {
        if (this == &other) {
            return *this;
        }
        Commit::operator=(other);
        this->commitType = other.commitType;
        return *this;
    }
    FeatureCommit(FeatureCommit &&other)
        : Commit{other}, commitType{other.commitType}
    {
    }
    FeatureCommit &operator=(FeatureCommit &&other)
    {
        if (this == &other) {
            return *this;
        }
        Commit::operator=(other);
        this->commitType = std::move(other.commitType);
        return *this;
    }

    CommitType GetType() const override
    {
        return this->commitType;
    }

    std::string GetTypeLabel() const override
    {
        return "Feature";
    }

    int GetPriority() const override
    {
        return 1;
    }
};

// [M7] 조상 클래스의 인터페이스를 구체적으로 실현하는 파생 클래스입니다.
// (BugfixCommit)
class BugfixCommit : public Commit
{
  private:
    CommitType commitType;

  public:
    virtual ~BugfixCommit()
    {
    }
    BugfixCommit(std::string m_hash_, std::string m_message_,
                 std::string m_author_)
        : Commit{m_hash_, m_message_, m_author_}, commitType{CommitType::Bugfix}
    {
    }

    BugfixCommit(const BugfixCommit &other)
        : Commit{other}, commitType{other.GetType()}
    {
    }

    BugfixCommit &operator=(const BugfixCommit &other)
    {
        if (this == &other) {
            return *this;
        }
        Commit::operator=(other);
        this->commitType = other.commitType;
        return *this;
    }
    BugfixCommit(BugfixCommit &&other)
        : Commit{other}, commitType{std::move(other.commitType)}
    {
    }
    BugfixCommit &operator=(BugfixCommit &&other)
    {
        if (this == &other) {
            return *this;
        }
        Commit::operator=(other);
        this->commitType = std::move(other.commitType);
        return *this;
    }

    CommitType GetType() const override
    {
        return this->commitType;
    }

    std::string GetTypeLabel() const override
    {
        return "Bugfix";
    }
    int GetPriority() const override
    {
        return 3;
    }
};