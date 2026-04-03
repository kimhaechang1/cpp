#include <fmt/core.h>
#include <string>

enum EquipCategory { Sword, Shield, Staff };

std::string GetCategoryLabel(EquipCategory category);

class Equipment {

public:
  virtual ~Equipment() {}
  virtual const std::string GetName() const = 0;
  virtual const int GetPower() const = 0;
  virtual void Describe() = 0;
};

class Weapon : public Equipment {
private:
  std::string name;
  int power;
  EquipCategory category;

public:
  Weapon(std::string name_, int power_, EquipCategory category_)
      : name(name_), power(power_), category(category_) {}
  virtual ~Weapon() {}

  const std::string GetName() const override { return this->name; }

  const int GetPower() const override { return this->power; }

  void Describe() override {
    fmt::print("Category: {}, Name: {}, Power: {}\n",
               GetCategoryLabel(this->category), GetName(), GetPower());
  }
};

class Armor : public Equipment {
private:
  std::string name;
  int power;

public:
  Armor(std::string name_, int power_) : name(name_), power(power_) {}
  const std::string GetName() const override { return this->name; }
  const int GetPower() const override { return this->power; }
  void Describe() override {
    fmt::print("Name: {}, Defense: {}\n", GetName(), GetPower());
  }
};