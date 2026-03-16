// Module 07 - Practice: Virtual Destructor
// Goal: Prove that without virtual destructor, child destructor is silently skipped.
//       Then fix it and prove the correct cleanup order.
//
// Instructions:
//   1. Implement [PART A] BuggyWeapon and BuggyStaff classes (NO virtual destructor on parent).
//   2. Implement [PART B] SafeWeapon and SafeStaff classes  (WITH virtual destructor on parent).
//   3. Run and check the test output. [PART A] test should PASS with destructor NOT called.
//                                     [PART B] test should PASS with destructor called.
// DO NOT modify main() or the global tracking variables.

#include <iostream>
#include <string>

// ============================================================
// Destructor call tracking flags (DO NOT MODIFY)
// ============================================================

// Set this to true inside BuggyStaff's destructor
bool buggyStaffDestructorCalled = false;

// Set this to true inside SafeStaff's destructor
bool safeStaffDestructorCalled = false;

// ============================================================
// [PART A] Buggy version: parent destructor is NOT virtual
// ============================================================
//
// class BuggyWeapon
//   - Purpose: base weapon class. Does NOT have a virtual destructor.
//   - Member variable named: name  (stores weapon name)
//   - Member variable named: damage (stores damage value)
//   - Constructor: takes name and damage, prints "[BuggyWeapon] Created: <name>"
//   - Destructor (non-virtual): prints "[BuggyWeapon] Destroyed: <name>"
//   - Member function named: Use  (virtual)
//       purpose: prints "[BuggyWeapon] Generic use: <name>"
//
// class BuggyStaff : public BuggyWeapon
//   - Purpose: child weapon that owns a dynamically allocated resource.
//   - Member variable named: mana  (dynamically allocated, purpose: store mana value)
//   - Constructor: takes name, damage, manaValue. Calls parent constructor.
//       Allocates mana on the heap. Prints "[BuggyStaff] Created, mana=<value>"
//   - Destructor: deletes mana. Sets buggyStaffDestructorCalled = true.
//       Prints "[BuggyStaff] Destroyed, mana freed"
//   - Override named: Use
//       purpose: prints "[BuggyStaff] Cast spell! mana=<*mana>"

// --- Write BuggyWeapon here ---
class BuggyWeapon {

    std::string name;
    int damage;
public:
    BuggyWeapon(std::string _name, int _damage): name(_name), damage(_damage) {
        std::cout << "[BuggyWeapon] Created: " << this->name << std::endl;
    }
    ~BuggyWeapon() {
        std::cout << "[BuggyWeapon] Destroyed: " << this->name << std::endl;
    }

    virtual void Use() {
        std::cout << "[BuggyWeapon] Generic use: " << this->name << std::endl;
    }
};


// --- Write BuggyStaff here ---
class BuggyStaff : public BuggyWeapon {
    int* mana;
public:
    BuggyStaff(std::string _name, int _damage, int mValue) : BuggyWeapon{_name, _damage} {
        this->mana = new int{ mValue };
        std::cout << "[BuggyStaff] Created, mana=" << *(this->mana) << std::endl;
    }

    ~BuggyStaff() {
        if (this->mana != nullptr) delete this->mana;
        buggyStaffDestructorCalled = true;
        std::cout << "[BuggyStaff] Destroyed, mana freed" << std::endl;
    }
    void Use() override {
        std::cout << "[BuggyStaff] Cast spell! mana=" << *(this->mana) << std::endl;
    }
};


// ============================================================
// [PART B] Safe version: parent destructor IS virtual
// ============================================================
//
// class SafeWeapon
//   - Identical structure to BuggyWeapon, EXCEPT:
//     the destructor must be declared VIRTUAL.
//   - Constructor: prints "[SafeWeapon] Created: <name>"
//   - Destructor (virtual): prints "[SafeWeapon] Destroyed: <name>"
//   - Member function named: Use (virtual)
//       purpose: prints "[SafeWeapon] Generic use: <name>"
//
// class SafeStaff : public SafeWeapon
//   - Identical structure to BuggyStaff, EXCEPT:
//     sets safeStaffDestructorCalled = true (not buggyStaffDestructorCalled).
//   - Constructor: prints "[SafeStaff] Created, mana=<value>"
//   - Destructor: deletes mana. Sets safeStaffDestructorCalled = true.
//       Prints "[SafeStaff] Destroyed, mana freed"
//   - Override named: Use
//       purpose: prints "[SafeStaff] Cast spell! mana=<*mana>"

// --- Write SafeWeapon here ---
class SafeWeapon {

    std::string name;
    int damage;
public:

    SafeWeapon(std::string _name, int _damage) : name(_name), damage(_damage) {
        std::cout << "[SafeWeapon] Created: " << this->name << std::endl;
    }
    virtual ~SafeWeapon() {
        std::cout << "[SafeWeapon] Destroyed: " << this->name << std::endl;
    }

    virtual void Use() {
        std::cout << "[SafeWeapon] Generic use: " << this->name << std::endl;
    }
};


// --- Write SafeStaff here ---
class SafeStaff: public SafeWeapon {
    int* mana;
public:

    SafeStaff(std::string _name, int _damage, int mValue) : SafeWeapon{ _name, _damage } {
        this->mana = new int{ mValue };
        std::cout << "[SafeStaff] Created, mana=" << *(this->mana) << std::endl;
    }

    ~SafeStaff() {
        if (this->mana != nullptr) delete this->mana;
        safeStaffDestructorCalled = true;
        std::cout << "[SafeStaff] Destroyed, mana freed" << std::endl;
    }
    void Use() override {
        std::cout << "[SafeStaff] Cast spell! mana=" << *(this->mana) << std::endl;
    }
};

// ============================================================
// Main: Test Cases (DO NOT MODIFY)
// ============================================================
int main() {

    // --- Test 1: Buggy delete (no virtual destructor) ---
    std::cout << "=== Test 1: BuggyWeapon delete via base pointer ===\n";
    {
        BuggyWeapon* p = new BuggyStaff("Shadow Staff", 40, 100);
        p->Use();
        delete p;
    }
    std::cout << "\n";

    if (!buggyStaffDestructorCalled) {
        std::cout << "[PASS] BuggyStaff destructor was NOT called. Memory leaked (as expected).\n";
    }
    else {
        std::cout << "[FAIL] BuggyStaff destructor should NOT have been called without virtual dtor.\n";
    }

    std::cout << "\n";

    // --- Test 2: Safe delete (with virtual destructor) ---
    std::cout << "=== Test 2: SafeWeapon delete via base pointer ===\n";
    {
        SafeWeapon* p = new SafeStaff("Holy Wand", 70, 250);
        // 결국에는 케스팅된 형태에서 밀려난 부모영역의 데이터 시작주소를 가리키고 있게 되고,
        // 처음에 OS 분명히 SaftStaff 의 시작주소를 줬겠지만, CPP 에서 이 업케스팅의 형태를 올바르게 작동시키기 위해서 주소값을 실제 부모 시작주소로 옮겨버렸다.
        // 그래서 p-> 를 하게되면 당연히 컴파일러의 의도대로 부모클래스 영역의 필드만 탐색이 가능하다.
        // 하지만 p->Use(); 의 경우에서는 vptr 은 부모껄 받아서 자기자신의 vtable(SafeStaff 의) 을 바라보도록 하고, vtable 역시 컴파일때 미리 부모것을 기반으로 덮어씌웠으므로
        // p->Use(); 에서 vptr 을 따라 SafeStaff 의 vtable 에 도착하고 그곳에서 Use() 함수의 주소를 찾아서 콜 하게된다.
        p->Use();
        delete p;
    }
    std::cout << "\n";

    if (safeStaffDestructorCalled) {
        std::cout << "[PASS] SafeStaff destructor WAS called. No memory leak.\n";
    }
    else {
        std::cout << "[FAIL] SafeStaff destructor should have been called with virtual dtor.\n";
    }

    std::cout << "\n";

    // --- Test 3: Polymorphic array cleanup ---
    std::cout << "=== Test 3: Polymorphic array, all SafeWeapons ===\n";
    SafeWeapon* party[3];
    party[0] = new SafeWeapon("Iron Sword", 20);
    party[1] = new SafeStaff("Wind Staff", 50, 80);
    party[2] = new SafeStaff("Fire Wand", 65, 120);

    for (int i = 0; i < 3; i++) {
        party[i]->Use();
    }
    std::cout << "\n--- Cleanup ---\n";
    for (int i = 0; i < 3; i++) {
        delete party[i]; // All should call the correct destructor chain
    }

    std::cout << "\n[Test 3 complete] Check above output: SafeStaff entries must print 'mana freed'.\n";

    return 0;
}
