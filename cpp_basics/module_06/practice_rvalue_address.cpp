#include <iostream>

enum MagicType { Fire, Ice };

void AddressTest(const MagicType& refType, const MagicType* ptrType) {
    // 1. 역참조(원본 값) 확인
    std::cout << "값 확인 -> 참조자 값: " << refType << " / 포인터가 가리키는 값: " << *ptrType << "\n";
    
    // 2. [가장 중요한 부분] '&' 연산자를 붙였을 때 나오는 주소의 의미
    // - 참조자(&)에 '&'를 붙이면: 참조자 본인의 주소가 아니라, "자신이 가리키고 있는 원본 방의 주소"가 나옵니다.
    // - 포인터(*)에 '&'를 붙이면: "포인터 변수 자기 자신의 주소"가 나옵니다.
    std::cout << "가리키는 대상의 주소 -> 참조자가 가리키는 주소: " << &refType << " / 포인터가 가리키는 주소: " << ptrType << "\n";
    std::cout << "자기 자신의 주소   -> 참조자 자기 자신의 주소: " << &refType << " (아까와 똑같음!) / 포인터 자기 자신의 주소: " << &ptrType << "\n\n";
}

int main() {
    MagicType myMagic = MagicType::Fire;
    
    std::cout << "--- [진짜 방] 원본 myMagic의 주소: " << &myMagic << " ---\n\n";
    
    // myMagic 변수를 참조자와 포인터 둘 다 넘겨봅니다.
    AddressTest(myMagic, &myMagic);

    return 0;
}
