#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <format>
#include <span>
#include <cassert>

// ===========================================================================
// [Q] 오류 원천 차단을 위해, 상태(State)들을 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요.
// ===========================================================================
// TODO: 상태 타입 선언 (예: CommandStatus) - 항목: PENDING, EXECUTED
enum CommandStatus {
    PENDING, EXECUTED
};

// ===========================================================================
// [Q] 외부 노출은 철저히 막되, 오직 나를 상속받은 자식 클래스에게만 데이터 접근 권한을 물려주어야 합니다.
// ===========================================================================
class Command {
    // TODO: 멤버 변수 선언
    std::string name;// - 이름: name          // 문자열 형태, 커맨드 식별용
    CommandStatus status;// - 이름: status        // 위에서 만든 상태 타입 보관용

public:
    // TODO: 생성자 (매개변수: name) - name 초기화
    Command(std::string name) : name{ name }, status{CommandStatus::PENDING} {

    }
    Command(Command& other) : name{ other.name }, status{other.status} {
        
    }
    Command& operator=(Command& other) {
        if (this == &other) {
            return *this;
        }
        this->name = other.name;
        this->status = other.status;
        return *this;
    }
    virtual ~Command() {
        
    }

    // TODO: Getter 선언 (기능: 이름 반환) - 함수명: GetName
    std::string GetName() const {
        return this->name;
    }
    // TODO: Getter 선언 (기능: 상태 반환) - 함수명: GetStatus
    CommandStatus GetStatus() const {
        return this->status;
    }
    std::string GetStatusStr() const {
        switch (this->status) {
        case CommandStatus::EXECUTED:
            return "EXECUTED";
        default:
            return "PENDING";
        }
    }
    // TODO: Setter 선언 (기능: 이름 변경) - 함수명: SetName
    void SetName(const std::string name) {
        this->name = name;
    }
    // TODO: Setter 선언 (기능: 상태 변경) - 함수명: SetStatus
    void SetStatus(const CommandStatus status) {
        this->status = status;
    }

    // TODO: 추상 메서드 선언 (자식이 반드시 구현) - 함수명: Execute, 매개변수 없음
    // 추상클래스가 된다.
    virtual void Execute() = 0;
};

// ===========================================================================
// [Q] 이 데이터는 게임 내에서 반드시 동적 할당되어 수동으로 관리되어야 합니다.
// [Q] 단순 얕은 복사 시 자원의 이중 해제(Double Free) 폭탄이 터집니다.
// 모든 동적 자원을 안전하게 복사할 딥카피(Deep Copy) 전용 자체 공장을 건설하세요.
// ===========================================================================
class MoveCommand: public Command /* TODO: 상속 */ {
    // TODO: 멤버 변수 선언
    // - 이름: targetCoords   // 동적으로 할당되는 좌표 배열(예: x, y)
    std::pair<int, int>* targetCoords;

public:
    // TODO: 생성자 (매개변수: name) - 부모 Command 생성 및 targetCoords 할당
    MoveCommand(std::string name) : Command{ name }, targetCoords{ new std::pair<int, int>{1, 1} } {
        
    }

    // TODO: 복사 생성자 (딥카피 구현)
    MoveCommand(MoveCommand& other) : Command(other), targetCoords{ new std::pair<int, int>{other.targetCoords->first, other.targetCoords->second} } {
        
    }
    // TODO: 복사 대입 연산자 (딥카피 구현)
    MoveCommand& operator=(MoveCommand& other) {
        if (this == &other) {
            return *this;
        }
        if (this->targetCoords != nullptr) {
            delete this->targetCoords;
        }
        Command::operator=(other);
        // 동적할당 메모리기 때문에 이런식으로 넣게되면 같은 힙메모리 객체를 바라봄
        this->targetCoords = new std::pair<int, int>{ other.targetCoords->first, other.targetCoords->second };
        return *this;
    }
    // TODO: 소멸자 (동적 메모리 해제)
    virtual ~MoveCommand() {
        if (this->targetCoords != nullptr) {
            delete this->targetCoords;
        }
    }
    

    // [Q] 문자열 템플릿 내부에 중괄호 {}를 열어 값을 주입하는 모던 C++ 문자열 조립 기법을 쓰세요.
    // TODO: Execute 함수 오버라이딩 - 상태를 EXECUTED 로 바꾸고, std::format 으로 결과 출력
    void Execute() override {
        this->SetStatus(CommandStatus::EXECUTED);
        std::cout << std::format("[{}] STATUS={}", this->GetName(), this->GetStatusStr()) << std::endl;
    }
};

// ===========================================================================
// [Q] 데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이를 가져야 합니다.
// ===========================================================================
class CommandQueue {
    // TODO: 컬렉션 선언 (다형성을 유지하는 Command* 들을 저장)
    std::vector<Command*> commands;

public:
    // TODO: 함수 선언 - AddCommand (매개변수: Command* cmd) - 컬렉션에 추가
    void AddCommand(Command* cmd) {
        this->commands.push_back(cmd);
    }

    // [Q] 컬렉션의 크기에 구애받지 않고 처음부터 끝까지 모든 원소를 안전하게 훑는 순회 방식을 쓰세요.
    // TODO: 함수 선언 - ExecuteAll (조건: 상태가 PENDING 인 경우에만 Execute 호출)
    void ExecuteAll() {
        std::span<Command*> views(this->commands);
        for (const auto cmd: views) {
            if (cmd->GetStatus() == CommandStatus::PENDING) {
                cmd->Execute();
            }
        }
    }

    // [Q] 자료구조의 겉모습이 배열인지 트리인지 무관하게, 내부 요소를 가리키고 전진할 수 있는 보편적 순회용 추상화 포인터를 사용하세요.
    // TODO: 함수 선언 - CountPending (반환: 현재 PENDING 상태인 객체 수)
    int CountPending() {
        int count = 0;
        for (auto it = this->commands.begin(); it != this->commands.end(); ++it) {
            if ((*it)->GetStatus() == CommandStatus::PENDING) {
                count++;
            }
        }
        return count;
    }

    // [Q] 함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용 로직 덩어리를 만들어 알고리즘에 던져 넣으세요.
    // TODO: 함수 선언 - MarkAllNamesDone (모든 객체의 name 앞에 "[DONE] " 를 붙임)
    void MarkAllNamesDone() {
        auto func = [&](std::vector<Command*> cmd) {
            for (Command* c : cmd) {
                c->SetName(std::format("[DONE] {}", c->GetName()));
            }
            };
        func(this->commands);
    }

    // std::span 테스트용 Getter
    // TODO: 함수 선언 - GetSpan (반환: 컬렉션을 std::span 로 노출)
    std::span<Command*> GetSpan() {
        return this->commands;
    }
};

// ===========================================================================
// [Q] 원본 배열의 소유권은 갖지 않은 채, 연속된 데이터의 '시야(View)'만 대여받아 요소를 읽거나 수정해야 합니다.
// [Q] 함수 호출이 끝난 후에도 외부에 있는 원본 데이터가 수정된 결과를 온전히 보존해야 합니다.
// ===========================================================================
// TODO: 함수 선언 - ProcessPartialCommands
// - 매개변수 1: 일부 Command 객체들을 std::span 로 전달받음
// - 매개변수 2: 결과를 저장할 정수 변수 (리턴값 사용 금지, 직접 변조)
void ProcessPartialCommands(std::span<Command*> views, int& resultCount) {
    for (Command* cmd : views) {
        if (cmd->GetStatus() == CommandStatus::EXECUTED) {
            resultCount++;
        }
    }
}

int main() {
    // ===========================================================================
    // [TEST] TDD 테스트 구역 (수정 금지)
    // ===========================================================================
    
    CommandQueue queue;

    // 객체 생성 (다형성 지원 및 동적 자원 소유 관리 확인)
    MoveCommand* mc1 = new MoveCommand("Player_Move1");
    MoveCommand* mc2 = new MoveCommand("Player_Move2");
    MoveCommand* mc3 = new MoveCommand("Player_Move3");

    queue.AddCommand(mc1);
    queue.AddCommand(mc2);
    queue.AddCommand(mc3);

    // 보편적 순회 방식 검증
    int pendingCount = queue.CountPending();
    assert(pendingCount == 3);

    // Deep Copy(메모리 릭 및 이중 해제 방지) 검증
    MoveCommand cloneTarget(*mc1);
    cloneTarget.Execute();
    assert(cloneTarget.GetStatus() == CommandStatus::EXECUTED);
    // [중요] 원본(mc1)은 오염되지 않아야 함!
    assert(mc1->GetStatus() == CommandStatus::PENDING);

    // 집단 실행 로직 검증
    queue.ExecuteAll();
    assert(queue.CountPending() == 0);
    assert(mc2->GetStatus() == CommandStatus::EXECUTED);

    // 1회용 로직 덩어리 주입(이름 변경) 검증
    queue.MarkAllNamesDone();
    assert(mc3->GetName() == "[DONE] Player_Move3");

    // 시야 대여 및 외부 데이터 직접 변조 검증
     std::vector<Command*> sampleView = { mc1, mc2 };
     int resultCount = 0;
     ProcessPartialCommands(sampleView, resultCount);
     assert(resultCount == 2);

    // 메모리 누수 방지 직접 클린업
    delete mc1;
    delete mc2;
    delete mc3;

    std::cout << "\n[SUCCESS] Cumulative Practice Passed!\n";
    
    return 0;
}
