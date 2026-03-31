    #include <iostream>
    #include <vector>
    #include <memory>
    #include <array>
    #include <span>
    #include <cassert>
    #include <algorithm>
    #include <string>
    #include<ranges>

    // =========================================================================
    // [Cumulative Practice 9-1: Smart Pointers & Move Semantics]
    // 
    // 🚨 엄격한 백지(Blank Slate) 정책 적용 🚨
    // 1. 함수의 반환 타입, 매개변수(&, * 등)는 스스로 결정해야 합니다.
    // 2. 가상 소멸자, 깊은 복사/이동 등에 대한 명시적 힌트가 없습니다. 
    // 3. 아래 주석의 "지시문(Prompt)"을 읽고 올바른 모던 C++ 문법을 추론하세요.
    // =========================================================================

    // [M1] 오류 원천 차단을 위해, 자원 상태(READY, ACTIVE, DEPLETED)들을 
    // 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요. (State)
    // -> 여기에 작성
    enum State {
        READY, ACTIVE, DEPLETED
    };

    // [M7] 부모 클래스: base_resource
    // [M7] 외부 노출은 철저히 막되, 오직 나를 상속받은 자식 클래스에게만 데이터(이름, 상태) 접근 권한을 물려주어야 합니다.
    class BaseResource {
        // -> 여기에 작성
    protected:
        std::string name;
        State state;
    public:
        // [M7] 다형성 보장용 소멸자
        // -> 여기에 작성
        BaseResource(std::string name) : name{ name }, state{State::READY} {
        
        }
        virtual ~BaseResource() {

        }

        // [M7] 파생 클래스에서 반드시 구현하도록 순수 가상 함수를 만드세요. (Use())
        // -> 여기에 작성
        virtual void Use() = 0;
    

        // [M9] Move Semantics & Rule of 5:
        // 얕은 복사 방지 및 복사 금지. 자원의 소유권을 독점해야 하므로 
        // "복사 생성자"와 "복사 대입 연산자"는 명시적으로 지워버리세요. (delete)
        BaseResource(const BaseResource& other) = delete;
        BaseResource& operator=(const BaseResource& other) = delete;
        // 반면, "이동 생성자"와 "이동 대입 연산자"는 default로 활성화하여 소유권 강탈을 허용하세요.
        // -> 여기에 작성
        BaseResource(const BaseResource&& other) noexcept {
            this->name = other.name;
            this->state = other.state;
        }
        BaseResource& operator=(const BaseResource&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            this->name = name;
            this->state = state;
        }
        State GetState() {
            return this->state;
        }
    };

    // [M7] 파생 클래스: EnergyCell
    // BaseResource를 상속받고, 생성자에서 이름을 받으세요.
    class EnergyCell /* 상속 */: public BaseResource {
    public:
        EnergyCell(std::string name) : BaseResource{name} {

        }
        virtual ~EnergyCell() {
        
        }
        // Use() 함수를 명시적으로 오버라이드하여, 호출 시 상태를 ACTIVE로 변경하게 하세요.
        // -> 여기에 작성
        void Use() override {
            this->state = State::ACTIVE;
        }
    };

    // ----------------------------------------------------------------------------------

    class ResourceVault {
    private:
        // [M8] 데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이를 가져야 합니다.
        // [M9] 스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는 똑똑한 독점 메모리 래퍼를 사용하세요. 
        // 즉, "독점 메모리 래퍼"를 담는 "가변 길이 배열"을 소유해야 합니다.
        // -> 여기에 보관소 데이터 선언
        std::vector<std::unique_ptr<EnergyCell>> resources;

    public:
        // 1. StoreResource
        // 외부에서 동적 할당된(단독 소유권) 자원을 매개변수로 받아 "보관소" 소유로 강탈하세요.
        // [M9] 무거운 데이터를 복사하지 말고, 내부 자원의 껍데기만 빠르고 난폭하게 '강탈'해 통째로 옮기세요.
        // -> 여기에 작성
        void StoreResource(std::unique_ptr<EnergyCell> resource) {
            // 여기서는 move 를 통해 복사생성자가 호출되는것이 아니라
            // 참조만 그대로 넘겨받는다.

            this->resources.push_back(std::move(resource));
            // 하지만 push_back 을 하게되면 일반적으로는 복사생성자가 또 호출될수 있다.
            // 따라서 이동시멘틱을 통해 그대로 옮겨준다.
        }

        // 2. TransferTo
        // 현재 보관소에 남아있는 자원을 '다른 ResourceVault'로 모조리 이전(강탈)시킵니다.
        // 이전 후, 현재 보관소는 텅 비어있어야 합니다.
        // [M9] std::move를 활용하세요.
        // -> 여기에 작성
        void TransferTo(ResourceVault& other) {
            for (int i = 0; i < this->resources.size(); i++) {
                other.StoreResource(std::move(this->resources[i]));
            }
            this->resources.clear();
        }

        // 3. CountActiveResources
        // 보관소 내부에 있는 자원 중, 특정 상태(예: ACTIVE)인 자원의 개수만 세어 반환합니다.
        // [M8] 함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용 로직 덩어리(Lambda)를 만들어 던져 넣으세요. (std::count_if)
        // -> 여기에 작성
        int CountActiveResources() {
            //int count = 0;
            /*auto fn = [&](const std::vector<std::unique_ptr<EnergyCell>>& res) {
                for (int i = 0; i < res.size(); i++) {
                    if (res[i]->GetState() == State::ACTIVE) {
                        count++;
                    }
                }
                };
            fn(this->resources);*/
            /*for (const std::unique_ptr<EnergyCell> cell : this->resources) {
                이거 안되는이유,
                원래라면 ranged-for 가 사용이 되야하지만 원소를 보아라
                임시 변수 cell 에 복사되어 전달 될 것이다.
                그말은 즉, unique_ptr<EnergyCell> 이 유일해야하는데, 두개 이상이 생겨버리는것이다.
            }*/
            int count = std::count_if(
                this->resources.begin(),
                this->resources.end(),
                [](const std::unique_ptr<EnergyCell>& cell) {
                    return cell->GetState() == State::ACTIVE;
                }
            );
            return count;
        }

        int GetSize() const {
            return this->resources.size();
        }
        const std::vector<std::unique_ptr<EnergyCell>>& GetResources() {
            // 앞에 오는 const 는 함수 바디내에 this-> 를 모두 읽기전용으로 가져온다.
            // 뒤에 오는 const 는 함수자체가 const 반환을 한다.
            // 여기서 중요한것은 EnergyCell 포인터변수가 수정이 불가능한거지 EnergyCell 포인터가 가리키는 힙은 수정이 가능하다.
            // 즉 const vector<unique_ptr<EnergyCell>> 이란것은 곧 vector<const unique_ptr<EnergyCell>> 와 같은 의미가된다.
            return this->resources;
        }
    };

    // ----------------------------------------------------------------------------------

    // [M4, M2] ViewSystem Function
    // [M4] 원본 배열의 소유권은 갖지 않은 채, 연속된 데이터의 '시야(View)'만 대여받아 요소를 읽거나 수정해야 합니다. (매개변수로 받기)
    // [M2] 컬렉션의 크기에 구애받지 않고 처음부터 끝까지 모든 원소를 안전하게 훑는 순회 방식을 사용하여, 각 자원의 Use()를 모두 호출하세요.
    // 함수의 이름은 'ActivateAll' 입니다.
    // -> 여기에 함수 구현부 작성
    void ActivateAll(std::span<const std::unique_ptr<EnergyCell>> views) {
        // 함수에서 파라미터로 넘어온것이 const 이기 때문에 const 를 맞춰주어야만 한다.
        // 그 이유는 span 의 경우 값을 관찰하는 목적이기 때문에 대상 값의 타입이 const 라면 맞춰줘야 하기 때문이다.
        // const span 은 말이안되는게, 관찰자이면서 관찰 대상을 교체할 수가 없다.
        // 그래서 span<const std::unique_ptr<EnergyCell>> 로 해야지 const 반환을 하는 자료구조와 맞출 수 있다.
        // 이 해석은 다음과 같다.
        // 관찰대상은 교체가능하나 관찰대상의 값을 수정할 순 없다.
        for (int i = 0; i < views.size(); i++) {
            views[i]->Use();
        }
    }


    int main() {
        std::cout << "[ Cumulative Practice 9-1: TDD Start ]\n\n";

        // 1. Vault 1, 2 생성
        ResourceVault vault1;
        ResourceVault vault2;

        // 2. 독점 소유권 객체 생성 (std::make_unique 사용)
        // -> vault1.StoreResource() 로 에너지셀 3개를 주입하되, "소유권 강탈 규칙(move)"을 철저히 지킬 것!
        /*
            auto e1 = std::make_unique<EnergyCell>("Cell-01");
            // vault1.StoreResource(... e1 ...);
        */
        auto e1 = std::make_unique<EnergyCell>("Cell-01");
        auto e2 = std::make_unique<EnergyCell>("Cell-02");
        auto e3 = std::make_unique<EnergyCell>("Cell-03");
        vault1.StoreResource(std::move(e1));
        vault1.StoreResource(std::move(e2));
        vault1.StoreResource(std::move(e3));


        // 3. Vault 상태 검증 1
        assert(vault1.GetSize() == 3);
        std::cout << "Vault1 StoreResource OK!\n";

        // 4. 소유권 전체 강탈 이전
         vault1.TransferTo(vault2);

        // 5. Vault 상태 검증 2
         assert(vault1.GetSize() == 0);
         assert(vault2.GetSize() == 3);
         std::cout << "Vault1 -> Vault2 Transfer OK!\n";

        // 6. View 시스템(std::span)으로 참조만 빌려서 작동시키기
        // vault2에서 데이터 뷰를 뽑아 ActivateAll()을 호출해야 합니다. (벡터를 직접 던질 수 있는지 고민해보세요)
        ActivateAll(vault2.GetResources());

        // 7. Lambda 캡처 확인 (상태 필터링 로직)
        assert(vault2.CountActiveResources() == 3);
        std::cout << "Lambda & CountActiveResources OK!\n";

        std::cout << "\n🌟 모든 TDD 검증을 통과했습니다! 🌟\n";
        return 0;
    }
