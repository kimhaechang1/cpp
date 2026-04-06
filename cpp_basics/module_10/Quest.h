#pragma once
#include <string>

/**
 * [M1] 오류 원천 차단을 위해, 퀘스트의 진행 상태(대기/진행/완료/실패)를
 * 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요.
 */
// (여기에 열거형(enum class) 정의)
enum class QuestState { Inactive, Active, Completed, Fail };

/**
 * [M7] 모든 퀘스트 종류의 공통 계약(Contract)을 정의하는 기반 설계도입니다.
 * 이 설계도 자체로는 실체화(인스턴스)가 불가능합니다.
 */
class Quest {
public:
  // [M6] 두 퀘스트가 '같다'는 것은 이름이 동일하다는 뜻입니다.
  // 비교 연산자(operator==)를 재정의하세요.
  bool operator==(const Quest &other) const { return this->name == other.name; }

  // [M7] 파생 타입마다 보상 배율이 다릅니다.
  // 기반 클래스에서는 계산 공식의 빈 껍데기만 약속하세요. (GetReward)
  virtual int GetReward() = 0;

  /**
   * [M3] 함수 호출이 끝난 후에도 원본 상태가 수정된 결과를 보존해야 합니다.
   * 대기->진행->완료 순서로 전이하며, 상태에 따라 분기하세요. (AdvanceState)
   */
  Quest(std::string name_, int reward_)
      : name{name_}, reward{reward_}, state{QuestState::Inactive} {}
  void AdvanceState() {
    switch (this->state) {
    case QuestState::Inactive:
      this->state = QuestState::Active;
      break;
    case QuestState::Active:
      this->state = QuestState::Completed;
      break;
    }
  }

  // [M4] 현재 이름을 외부에서 읽을 수 있어야 합니다. 불필요한 복사 없이.
  // (GetName)
  const std::string &GetName() const { return this->name; }

  // [M4] 현재 상태를 외부에서 읽을 수 있어야 합니다. 불필요한 복사 없이.
  // (GetState)
  const QuestState &GetState() const { return this->state; }

  // [Quest] 업캐스팅 구조를 고려하여 안전한 소멸자를 설계하세요.
  virtual ~Quest() {}

protected:
  // [M1] 퀘스트 이름(문자열)과 기본 보상(정수), 그리고 현재 상태를 보관합니다.
  std::string name;
  int reward;
  QuestState state;
};

/**
 * [M7] 메인 스토리 퀘스트입니다. 보상은 기본 보상의 2배입니다. (MainQuest)
 */
// (여기에 MainQuest 클래스 정의)
class MainQuest : public Quest {
public:
  MainQuest(std::string name_, int reward_) : Quest{name_, reward_} {}

  int GetReward() override { return this->reward * 2; }
};

/**
 * [M7] 부가 퀘스트입니다. 보상은 기본 보상 그대로(1배)입니다. (SideQuest)
 */
// (여기에 SideQuest 클래스 정의)
class SideQuest : public Quest {
public:
  SideQuest(std::string name_, int reward_) : Quest{name_, reward_} {}

  int GetReward() override { return this->reward; }
};
