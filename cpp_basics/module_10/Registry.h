#include "Equipment.h"
#include <algorithm>
#include <memory>
#include <vector>

class Registry {
  static inline int TotalRegisteredEquipment = 0;
  std::vector<std::unique_ptr<Equipment>> equipments;

public:
  void Register(std::unique_ptr<Equipment> equip) {
    TotalRegisteredEquipment++;
    equipments.push_back(std::move(equip));
  }

  void CalcTotalPower(int &result) const {
    for (int i = 0; i < this->equipments.size(); i++) {
      result += equipments[i]->GetPower();
    }
  }

  void SortByPower() {
    std::sort(
        this->equipments.begin(), this->equipments.end(),
        [](std::unique_ptr<Equipment> &e1, std::unique_ptr<Equipment> &e2) {
          return e1->GetPower() > e2->GetPower();
        });
  }

  void PrintAll() {
    for (const auto &equip : equipments) {
      equip->Describe();
    }
  }
  static size_t GetTotalRegistered() { return TotalRegisteredEquipment; }
  size_t GetSize() { return this->TotalRegisteredEquipment; }
  Equipment &GetItemAt(int index) {
    // Equipment *ptr = equipments[index].release();
    // 이것은 사실 raw pointer 로 소유권을 unique_ptr 이 해제해버리는것이다.
    // 이대로 되면 unique_ptr 의 장점인 알아서 delete 되는 효과도 없어져버리고
    // 원래 equipments 가 갖고있는 unique_ptr 에서는 nullptr 이 되어버린다.
    Equipment *ptr = equipments[index].get();
    // 그러니 get 으로 잠깐 옅보기만 하자
    return *ptr;
  }
};