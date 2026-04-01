#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>
#include <expected>
#include <cassert>

// =========================================================================
// [Cumulative Practice 9-3] 모던 타입 심화 종합 실습
// 포함 개념: 
// 1. Module 9 - Modern Types (optional, variant, expected)
// 2. Module 9 - Structured Binding & std::move
// 3. Module 1~8 - Map, Struct, Enum, Reference
// =========================================================================

// -------------------------------------------------------------------------
// [TODO 1] 구조체와 별명(using) 선언
// 1. Texture 구조체를 만드세요. 멤버 변수: int id;
// 2. Audio 구조체를 만드세요. 멤버 변수: float length;
// 3. std::variant<Texture, Audio> 를 `AssetVariant` 라는 이름으로 using 선언하세요.
// 4. enum class ErrorCode { FileNotFound, Corrupt, OutOfMemory }; 를 선언하세요.
// -> 여기에 작성



// -------------------------------------------------------------------------
// [TODO 2] 함수 구현: LoadAsset
// 매개변수: 에셋 경로 (const std::string& path)
// 반환값: 성공 시 AssetVariant, 실패 시 ErrorCode를 담는 std::expected 클래스
// 로직:
// - path가 "bg.png" 이면 Texture(100) 귀환
// - path가 "bgm.mp3" 이면 Audio(3.5f) 귀환
// - 그 외 경로는 std::unexpected(ErrorCode::FileNotFound) 반환
// -> 여기에 작성



// -------------------------------------------------------------------------
// [TODO 3] 에셋 관리자 클래스(AssetManager) 구현
class AssetManager {
private:
    // 키: 경로(string), 값: 에셋(AssetVariant)을 저장하는 map
    std::map<std::string, AssetVariant> cache;

public:
    // 1. Preload 함수 (반환형 void)
    // - const std::string& path 를 받아서 LoadAsset(path) 호출.
    // - 결과가 성공(has_value)이면, std::move() 를 써서 cache 맵에 insert 또는 [] 대입하세요.
    // - 실패하면 무시.
    // -> 여기에 작성
    

    // 2. PrintAll 함수 (반환형 void, 상수(const) 멤버 함수)
    // - C++17 구조 분해 할당(auto& [path, asset])을 사용해 cache 안의 모든 항목을 순회하세요.
    // - std::holds_alternative 와 std::get 을 활용하여,
    //   "텍스처: bg.png (ID: 100)" 또는 "오디오: bgm.mp3 (길이: 3.5)" 형태로 출력되게 하세요.
    // -> 여기에 작성
    
};


int main() {
    std::cout << "[ Cumulative Practice 9-3 시작 ]\n\n";

    AssetManager manager;

    // 1번: 검증된 에셋들 불러오기
    manager.Preload("bg.png");
    manager.Preload("bgm.mp3");
    
    // 2번: 없는 에셋을 불렀을 때 서버가 죽지 않고 무사히 넘어가야 함
    manager.Preload("fake_virus.exe");

    // 3번: 결과 출력 및 컴파일/런타임 검증
    std::cout << "--- 현재 캐시된 에셋 목록 ---\n";
    manager.PrintAll();
    // (정상 출력 예시)
    // 텍스처: bg.png (ID: 100)
    // 오디오: bgm.mp3 (길이: 3.5)

    std::cout << "\n🌟 모든 검증을 통과했습니다! 🌟\n";
    return 0;
}
