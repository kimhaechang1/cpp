#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <span>
#include<format>

using namespace std;

// =========================================================================
// 1. 공격 속성(불, 물, 번개)을 나타내는 열거형 선언
// =========================================================================
enum Attribute {
    Fire, Water, Lightning
};



// =========================================================================
// 2. 기반이 되는 최상위 개체 설계 (추상적인 개념)
// - 이름과 기본 체력을 보호된 상태로 가짐 (외부 접근 불가, 자식만 접근 가능)
// - 소멸 시 메모리 처리를 위한 기본 설정 (Rule of 3/5/0 중 필요한 것 고민)
// =========================================================================
class Logger {
private:
    std::string* logBuffer;
    static inline std::string FATAL_HEAD = "FATAL";
    static inline std::string WARNING_HEAD = "WARNING";
    static inline std::string INFO_HEAD = "INFO";
    static inline std::string DEBUG_HEAD = "DEBUG";
    static inline int DEFAULT_LOG_BUFFER_SIZE = 100;
    int logBufferSize;
    int logBufferIdx;

    int CanBufferSave() {
        if (logBufferSize < logBufferIdx) {
            cout << "[FATAL] Cannot Save Log into Buffer!! please call GetBufferLogAndClear() or ClearBuffer()" << endl;
            return 0;
        }
        return 1;
    }

    std::string GetFormatForm() {
        std::string prefix = GetPrefix();
        return "[{}]" + GetPrefix() + ": {}";
    }

public:

    Logger(int _logBufferSize = DEFAULT_LOG_BUFFER_SIZE) : logBufferSize(_logBufferSize) {
        this->logBufferIdx = 0;
        this->logBuffer = new std::string[logBufferSize];
    }

    virtual void info(const std::string msg) {
        if (CanBufferSave() == 0) {
            return;
        }
        std::string log = std::vformat(GetFormatForm(), std::make_format_args(INFO_HEAD, msg));
        cout << log << endl;
        this->logBuffer[this->logBufferIdx++] = log;
    }

    virtual string GetPrefix() {
        return "";
    }
    void ClearLogBuffer() {
        delete[] this->logBuffer;
        this->logBuffer = new std::string[this->logBufferSize];
    }

    string* GetBufferLogAndClear() {
        string* sentBuffer = new std::string[this->logBufferSize];
        for (int i = 0; i < this->logBufferSize; i++) {
            sentBuffer[i] = this->logBuffer[i];
        }
        ClearLogBuffer();
        return sentBuffer;
    }

    ~Logger() {
        cout << "Logger 사망" << endl;
        if (this->logBuffer != nullptr) {
            delete[] this->logBuffer;
        }
    }

    Logger(const Logger& other) {
        this->logBufferSize = other.logBufferSize;
        this->logBufferIdx = other.logBufferIdx;
        this->logBuffer = new std::string[this->logBufferSize];
        for (int i = 0; i < this->logBufferIdx; i++) {
            this->logBuffer[i] = other.logBuffer[i];
        }    
    }
    Logger& operator=(const Logger& other) {
        // 까먹지 말것 자기자신인지 꼭 체크해야함
        // 여기서 other 원본을 받았기에, 주소로 비교해야함, 왜냐하면 this 도 주소기 때문
        if (this == &other) {
            return *this;
            // 근데 반대로 생각해서 other 가 원본을 가리킨다고 하면 *this = other 도 성립되어야 하는거 아닌가?
            /*
            * 아 이제 완벽하게 이해했다
            참조자가 타입앞에 쓰일때는 별칭이자 원천을 가져오겠단 의미고
            변수앞에 쓰일때는 해당 주소값을 가져오겠단 의미고
            그래서 this 와 비교하기 위해서는
            this 는 자기자신 객체를 가리키는 주소 포인터고
            other는 원천이지만 &other 를 하게되면 원천의 주소를 가져오는거라서 비교가 가능한거지
            주소끼리비교하는건 어떻게 보면 숫자값에 대한 연산이니까 정의된걸로 쓸거고

            반대로 생각해서 그럼 객체끼리 비교는?
            other 그자체랑 비교하고싶다면 역참조를 걸어야겠지 this에
            *this == other 
            근데 이거는 객체의 비교이기 때문에 클래스에 == operator 에 대한 정의가 있어야한다
            */
        }

        if (this->logBuffer != nullptr) {
            delete[] this->logBuffer;
        }
        this->logBufferSize = other.logBufferSize;
        this->logBufferIdx = other.logBufferIdx;
        this->logBuffer = new std::string[this->logBufferSize];
        for (int i = 0; i < this->logBufferIdx; i++) {
            this->logBuffer[i] = other.logBuffer[i];
        }
        return *this;
    }
};

class Object: virtual public Logger {
private:
    
    int objectId;
    static int inline id = 0;

    std::string GetPrefix() override {
        return std::format("ObjectId={}, name={}", this->objectId, this->name);
    }
    
    
protected:
    std::string name;
    int hp;
    
    
    Object(std::string n, int h) : name(n), hp(h) {
        id++;
        objectId = id;
    }

    ~Object() {
        cout << "Object 사망" << endl;
    }

    Object(const Object& other): Logger(other){
        this->name = other.name;
        this->hp = other.hp;
    }

    Object& operator=(const Object& other) {
        if (this == &other) return *this;
        Logger::operator=(other);
        this->name = other.name;
        this->hp = other.hp;
        return *this;
    }
public:
    void DropHp(int value) {
        if (this->hp >= value) {
            this->hp -= value;
        }
        else {
            this->hp = 0;
        }
    }
    int GetObjectId() {
        return objectId;
    }
    std::string GetName() {
        return name;
    }
};







// =========================================================================
// 4. 모험가 설계 (최상위 개체를 상속)
// - 다이아몬드 상속 문제가 발생하지 않도록 상속 방식을 결정
// - 최대 마나를 추가 속성으로 가짐
// =========================================================================
class Adventurer : virtual public Object {
private:
    int mana;

    bool CheckMana(int value) {
        return this->mana >= value;
    }

    std::string GetPrefix() override {
        return std::format("ObjectId={}, Name={}, Job={}", this->GetObjectId(), this->name, GetJobName());
    }

    virtual std::string GetJobName() {
        return "";
    }
protected:
    virtual void LogForNotEnoughMana() {
        info("마나가 부족합니다.");
    }
public:
    Adventurer(std::string n, int hp, int mana) : Object{ n, hp }, mana(mana) {

    }
    int DropMana(int dropValue) {
        if (!CheckMana(dropValue)) {
            LogForNotEnoughMana();
            return 0;
        }
        this->mana -= dropValue;
        return 1;
    }
    int GetMana() {
        return mana;
    }
    Adventurer(const Adventurer& other): Object(other){
        this->mana = other.mana;
    }
    ~Adventurer() {

    }

    Adventurer operator=(const Adventurer& other) {
        if (this == &other) return *this;
        Object::operator=(other);
        this->mana = other.mana;
        return *this;
    }
};


// =========================================================================
// 3. '공격 가능함'을 나타내는 인터페이스 설계
// - 다형성을 활용하여 자식 클래스에서 반드시 구현해야 하는 공격 함수 선언 (타입 무관)
// - 공격 대상(단일 개체 또는 다중 개체)을 인자로 받을 수 있도록 설계 고민
// =========================================================================
class IAttacker {
public:
    virtual void Attack(span<Adventurer> targets, Attribute attribute) {

    }
};

// =========================================================================
// 5. 마법사 클래스 설계 (모험가이자 동시에 공격 가능한 개체 - 다중 상속)
// - 부모들의 속성을 올바르게 초기화하는 생성자 작성
// - 공격 인터페이스 구현: 
//   a) 공격 시 전달받은 대상들(범위/배열 전체 뷰)에 대해 순회
//   b) 대상의 속성에 따라 데미지를 다르게 적용하는 분기문 작성
//   c) 데미지 적용은 별도의 내부 유틸리티 함수(참조 전달 사용)를 호출하여 처리
// - 모든 공격 기록을 힙 영역에 동적으로 저장하여 누적 데미지를 추적하되, 
//   댕글링 포인터가 남지 않도록 해제 및 초기화 철저
// - 동일한 마법사끼리 공격 기록을 병합할 수 있는 연산자 오버로딩 구현
// =========================================================================
class Mage: virtual public IAttacker, virtual public Adventurer {
    // IAttacker 를 위한 vbptr, 
    // Advanturer 를 위한 vbptr

    // 그리고 본인자신이 오버라이드한 Attack 함수로 인한 vptr -> Mage 의 vtable
    int* accDamage;
    int ManaAttack(Adventurer& target, int damageValue, int usedMana) {
        int rv = DropMana(usedMana);
        if (rv == 0) {
            return 0;
        }
        target.DropHp(damageValue);
        *accDamage += damageValue;
        return 1;
    }

    std::string GetJobName() override {
        return "Mage";
    }


    void LogForAttack(Adventurer& target, std::string attributeName, int usedMana, int damageValue) {
        info(std::format("target[ObjectId={}, name={}] 을 attribute={} 로 {} 마나를 사용하여 {} 데미지를 입혔다!", target.GetObjectId(), target.GetName(), attributeName, usedMana, damageValue));
    }

    
public:
    Mage(std::string n, int hp, int mana) : Adventurer{ n, hp, mana }, Object{n, hp} {
        accDamage = new int{ 0 };
    }

    void Attack(span<Adventurer> targets, Attribute attribute) override {
        for (Adventurer& adv : targets) {
            /*
            span 에 있는걸 꺼내는건데 왜 복사가 안일어나게 조심해야하는지?
            꼭 span 이라서 생기는 문제가 아니다
            span 자체는 메모리 view 의 역할을 수행한다.
            span<int> views() 에서의 괄호 ()
            네, 맞습니다. 변수를 선언하면서 뒤에 ()를 붙이는 것은 해당 타입의 **생성자(Constructor)**를 호출하는 행위입니다.

            span<int> s(container); 처럼 호출하면, span은 실제 데이터를 새로 복사해서 저장하는 것이 아니라, 넘겨받은 컨테이너의 **"시작 주소"**와 **"길이"**만 딱 복사해서 보관합니다.

            2. span 생성자가 허용하는 인자들
            span은 "연속된 메모리"라면 무엇이든 다 받아줄 준비가 되어 있습니다. 주로 다음과 같은 것들을 인자로 받습니다.

            C 스타일 배열: int arr[] = {1, 2, 3}; span<int> s(arr);

            표준 컨테이너: std::vector<int> v; span<int> s(v); 또는 std::array<int, 3> a;

            포인터와 크기: span<int> s(ptr, count); (가장 원초적인 방식)

            시작과 끝 반복자: span<int> s(v.begin(), v.end());

            핵심: span은 데이터를 소유하지 않습니다. 그냥 "어디서부터 어디까지가 데이터야"라는 이정표만 들고 있는 것입니다.
            
            그리고 ranged-for 의 경우 내부적으로 auto 를 통한 변수에 저장을 하게된다.
            이 과정에서 일반 대입마냥 발생되서 복사가 되는것
            */
            std::string attributeName = "";
            int usedMana = 0;
            int damageValue = 0;
            if (attribute == Attribute::Fire) {
                attributeName = "Fire";
                usedMana = 10;
                damageValue = 10;
            }
            else if (attribute == Attribute::Water) {
                attributeName = "Water";
                usedMana = 5;
                damageValue = 5;
            }
            else if (attribute == Attribute::Lightning) {
                attributeName = "Lightning";
                usedMana = 15;
                damageValue = 15;
            }
            int rv = ManaAttack(adv, 10, 10);
            if (rv == 1) {
                LogForAttack(adv, attributeName, usedMana, damageValue);
            }
        }   
    }
    int GetTotalDamage() {
        int accCopy = (*accDamage);
        return accCopy;
    }

    ~Mage() {
        if (accDamage != nullptr) {
            delete accDamage;
        }
    }
    Mage(Mage& other) : Adventurer(other), Object(other) {
        /*
        * 왜 나는 Adventurer 에 Object 에 대한 생성자를 분명히 호출했는데 왜 Mage 에서 또 호출해야하는걸까?
        * 
        가상 상속(virtual public)을 하는 이유는 "이 클래스(Object, Logger)는 상속 계층 어디에 있든 메모리에 딱 하나만 존재해야 한다"고 선언하는 것입니다.

        이때 문제가 발생합니다. 만약 Mage가 Adventurer를 통해 Object를 초기화하도록 내버려 두면, 나중에 다른 경로(예: IAttacker가 Object를 상속받는 경우)가 생겼을 때 누가 Object를 초기화할지 결정할 수 없게 됩니다.

        그래서 C++은 다음과 같은 규칙을 정했습니다:

        가상 기저 클래스(Virtual Base Class)의 생성자는 가장 하위 자식(Most Derived Class)이 직접 호출한다. 중간 부모들이 호출하는 코드(예: Adventurer가 호출하는 Object{n, hp})는 가상 상속 상황에서 완전히 무시됩니다.
        */
        this->accDamage = new int{ *other.accDamage };
    }

    Mage& operator=(Mage& other) {
        if (this == &other) return *this;
        if (this->accDamage != nullptr) {
            delete this->accDamage;
        }
        Adventurer::operator=(other);
        this->accDamage = new int{ *other.accDamage };
        return *this;
    }
};


// =========================================================================
// 테스트 코드 (수정하지 마세요)
// =========================================================================
int main() {
    // 💡 테스트 코드 (사용자는 이 main 함수가 오류 없이 컴파일되고 통과하도록 위쪽 구조를 설계해야 합니다)

    // 1. 타겟 개체들 생성 (임시로 모험가 개체를 타겟으로 사용)
    Adventurer target1("Goblin", 50, 0);
    Adventurer target2("Orc", 100, 0);

    vector<Adventurer> targetList = { target1, target2 };

    // 2. 다중 상속을 받은 마법사 객체 생성 (이름, 체력, 최대 마나)
    Mage mage1("Gandalf", 100, 500);

    // 3. '공격 가능함' 인터페이스 포인터로 업캐스팅하여 다형성 동작 검증
    IAttacker* attacker = &mage1;

    //span<Target> viewList(targetList);
    // 4. 여러 대상을 나타내는 뷰(span)와 열거형 속성을 전달하여 공격
    attacker->Attack(targetList, Attribute::Fire);

    // 5. 동적 할당된 힙 영역에 누적 데미지가 정상적으로 기록되었는지 검증
    assert(mage1.GetTotalDamage() > 0);

    // 6. 대입 연산자 오버로딩을 통한 깊은 복사 검증
    Mage mage2("Saruman", 120, 600);
    mage2 = mage1; // 깊은 복사 로직 실행

    assert(mage2.GetTotalDamage() == mage1.GetTotalDamage());

    cout << "✅ [SUCCESS] 다중 상속과 다형성을 포함한 누적 실습을 완벽하게 통과했습니다!" << endl;
    return 0;
}
