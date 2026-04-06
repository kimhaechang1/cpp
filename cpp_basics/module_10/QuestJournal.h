#pragma once
#include "Quest.h"
#include <expected>
#include <functional>
#include <memory>
#include <string>
#include <vector>

/**
 * [M10] 퀘스트들의 원본을 독점 소유하며 관리하는 저널(Journal)입니다.
 * 스택 수명 스코프를 벗어나는 즉시 자동으로 정리되는 똑똑한 독점 래퍼로
 * 각 퀘스트를 감싸서 보관하세요.
 */
class QuestJournal {

public:
  /**
   * [M9] 새 퀘스트를 저널에 등록합니다. 이미 같은 이름이 존재하면 에러를
   * 반환하세요. 에러 방지를 위해 성공/실패를 안전상자로 포장해 반환하세요.
   * (std::expected) 퀘스트의 소유권은 저널로 완전히 이전됩니다(복사 금지,
   * 강탈만 허용, move).
   */
  // (AddQuest 함수)
  std::expected<std::string, std::string>
  AddQuest(std::unique_ptr<Quest> quest);

  /**
   * [M9] 이름으로 퀘스트를 검색합니다.
   * 찾으면 원본의 읽기 전용 참조를, 못 찾으면 에러를 안전상자에 담아
   * 반환하세요. (std::expected)
   */
  // (FindQuest 함수)
  std::expected<Quest *, std::string>
  // 진짜 들여다보기만 할거니까 raw pointer 타입을 사용하면 된다.
  FindQuest(const std::string &name) const;

  /**
   * [M3] 이름으로 퀘스트를 찾아 상태를 한 단계 전진시킵니다. (AdvanceState)
   */
  // (AdvanceState 함수)
  void AdvanceState(const std::string &name);

  /**
   * [M8] 현장에서 이름 없는 1회용 로직 덩어리(Lambda)를 받아서,
   * 조건에 맞는 퀘스트들의 읽기 전용 참조 목록을 반환하세요.
   */
  // (FilterByState 함수)
  std::vector<const Quest *>
  FilterByState(std::function<bool(const Quest *)> fn);
  /**
   * [M10] 현재 등록된 퀘스트의 총 개수를 반환합니다. (GetQuestCount)
   */
  // (GetQuestCount 함수)
  int GetQuestCount() const;

private:
  /**
   * [M8] 퀘스트 래퍼들을 1차원으로 순차적, 연속적으로 보관하는
   * 가변 길이 컨테이너를 사용하세요. (std::vector)
   */
  // (저장소 데이터 멤버)
  std::vector<std::unique_ptr<Quest>> quests;
};
