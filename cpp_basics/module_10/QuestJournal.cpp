#include "QuestJournal.h"
#include "Quest.h"

// [M10] 퀘스트 저널 클래스의 모든 기능을 여기에 구현하세요.
// 힌트: GTest 테스트 파일(test_quest_journal.cpp)을 읽고
// 필요한 인터페이스(함수 명칭, 파라미터 타입)를 추론하여 구현해야 합니다.

std::expected<Quest *, std::string>
// 진짜 들여다보기만 할거니까 raw pointer 타입을 사용하면 된다.
QuestJournal::FindQuest(const std::string &name) const {
  std::unique_ptr<Quest> found;
  for (auto &q : quests) {
    if (q->GetName() == name) {
      return q.get();
    }
  }

  return std::unexpected<std::string>(
      "동일한 이름의 퀘스트를 찾지 못했습니다.");
}

std::expected<std::string, std::string>
QuestJournal::AddQuest(std::unique_ptr<Quest> quest) {
  auto rs = FindQuest(quest->GetName());
  if (rs.has_value()) {
    return std::unexpected<std::string>(
        "중복된 이름이 존재하여 퀘스트 추가에 실패했습니다. Quest 이름: " +
        quest->GetName());
  }

  quests.push_back(std::move(quest));
  return "성공적으로 퀘스트를 추가했습니다.";
}

void QuestJournal::AdvanceState(const std::string &name) {
  auto founded = FindQuest(name);
  if (founded.has_value()) {
    founded.value()->AdvanceState();
  }
}

std::vector<const Quest *>
QuestJournal::FilterByState(std::function<bool(const Quest *)> fn) {
  std::vector<const Quest *> rs;
  for (const auto &quest : quests) {
    auto qPtr = quest.get();
    if (fn(qPtr)) {
      rs.push_back(qPtr);
    }
  }
  return rs;
}

int QuestJournal::GetQuestCount() const { return quests.size(); }