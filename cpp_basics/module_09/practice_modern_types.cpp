#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <expected>
#include <cassert>

enum class ParseError { InvalidFormat, OutOfRange, Unknown };

// =========================================================================
// [Practice] Modern Types (optional, variant, expected)
// 🚨 엄격한 백지 정책 적용 🚨
// ❗️주의: Module 3 훈련 지침에 따라 "함수의 원형"을 미리 제공하지 않습니다.
// =========================================================================

// -------------------------------------------------------------------------
// [TODO 1] std::optional
// [🚨 함수 원형 스스로 작성!]
// 이름: FindPlayerLevel
// 매개변수: 문자열(유저 닉네임) 하나를 받습니다. (const 참조 권장)
// 반환값: 유저의 레벨(int)을 반환하되, 유저가 없을 경우 "비어있는 상태"를 반환해 주세요.
// 로직: 닉네임이 "Faker"면 99, "Deft"면 90을 반환하고, 그 외에는 비어있는 상태를 반환하세요.
// -> 여기에 작성
std::optional<int> FindPlayerLevel(const std::string& name) {
    if (name == "Faker") {
        return 99;
    }
    else if (name == "Deft") {
        return 90;
    }
    else {
        return std::nullopt;
    }
}



// -------------------------------------------------------------------------
// [TODO 2] std::variant
// 게임 설정값은 정수(int), 실수(float), 불리언(bool) 세 가지 중 하나의 타입을 가질 수 있습니다.
// [🚨 함수 원형 스스로 작성!]
// 이름: GetConfigValue
// 매개변수: 설정 키(문자열)를 받습니다.
// 반환값: int, float, bool 셋 중 하나를 가질 수 있는 모던 유니온 타입(variant)을 반환하세요.
// (힌트: std::variant<T1, T2, T3> 를 반환 타입으로 씁니다)
// 로직: 키가 "Volume"이면 100, "Brightness"면 0.5f, "Fullscreen"이면 true를 반환하세요.
// -> 여기에 작성
std::variant<int, float, bool> GetConfigValue(const std::string& key) {
    if (key == "Volume") {
        return 100;
    }
    else if (key == "Brightness") {
        return 0.5f;
    }
    else {
        return true;
    }
}



// -------------------------------------------------------------------------
// [TODO 3] std::expected (C++23 전용 최신 문법)
// ※ 터미널에서 반드시 컴파일 시 -std=c++23 을 주어야 합니다!
// [🚨 함수 원형 스스로 작성!]
// 이름: ParseSecureString
// 매개변수: 파싱할 문자열(string) 하나를 받습니다.
// 반환값: 파싱에 성공하면 그 결과를 "정수(int)"로 반환하고, 실패하면 "ParseError 열거형"을 에러 객체로 반환하는 모던 에러 래퍼 타입을 사용하세요.
// (힌트: std::expected<성공타입, 에러타입> 을 반환 타입으로 씁니다)
// 로직: 
// 1. 문자열이 "Secret123" 이면 성공 값(123)을 반환하세요.
// 2. 문자열이 "Hack" 이면 에러 값(ParseError::InvalidFormat)을 unexpected() 로 감싸서 반환하세요.
// 3. 그 외의 문자열이면 에러 값(ParseError::Unknown)을 unexpected() 로 감싸서 반환하세요.
// -> 여기에 작성
std::expected<int, ParseError> ParseSecureString(const std::string& name) {
    if (name == "Secret123") {
        return 123;
    }
    else if (name == "Hack") {
        return std::unexpected(ParseError::InvalidFormat);
    }
    else {
        return std::unexpected(ParseError::Unknown);
    }
}   



int main() {
    std::cout << "[ Modern Types 훈련 시작 ]\n\n";

    // ---------------------------------------------------------
    // [검증 1] std::optional
    auto opt1 = FindPlayerLevel("Faker");
    auto opt2 = FindPlayerLevel("Unknown");

     assert(opt1.has_value() == true);
     assert(opt1.value() == 99);
     assert(opt2.has_value() == false);
     std::cout << "1. Optional Test OK\n";

    // ---------------------------------------------------------
    // [검증 2] std::variant
    auto var1 = GetConfigValue("Volume");
    auto var2 = GetConfigValue("Brightness");
    auto var3 = GetConfigValue("Fullscreen");

    // variant 안에 원하는 타입이 들어있는지 확인하는 std::holds_alternative 사용!
    assert(std::holds_alternative<int>(var1) == true);
    assert(std::get<int>(var1) == 100);
    // c++23 에 추가된것으로 holds_alternative 는 타입검사
    // std::get<T>() 는 특정 타입 값 꺼내오기
    // 
    // assert(std::holds_alternative<int>(var1) == true);
     assert(std::get<int>(var1) == 100);
     assert(std::holds_alternative<float>(var2) == true);
     assert(std::holds_alternative<bool>(var3) == true);
     assert(std::get<bool>(var3) == true);
    // std::cout << "2. Variant Test OK\n";

    // ---------------------------------------------------------
    // [검증 3] std::expected (※ 컴파일 시 -std=c++23 옵션 필수!)
    auto exp1 = ParseSecureString("Secret123");
    auto exp2 = ParseSecureString("Hack");

    // expected는 has_value()로 성공 여부를 체크하고, 
    // 성공 시 .value() 혹은 역참조(*)로 값을, 실패 시 .error()로 에러객체를 뽑습니다.
    // 약간 비슷하게 동작하네 optional 이랑
    // value() 가 존재할려면 has_value 가 true 여야하고
    // value() 가 존재하지 않으면서 error() 가 존재하려면 has_value 가 false 여야 한다.
    //std::cout << exp2.value() << std::endl;
     assert(exp1.has_value() == true);
     assert(exp1.value() == 123);
     assert(exp2.has_value() == false);
     assert(exp2.error() == ParseError::InvalidFormat);
    // std::cout << "3. Expected Test OK\n";

    std::cout << "\n🌟 모든 검증을 통과했습니다! 🌟\n";
    return 0;
}
