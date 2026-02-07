#include <iostream>
#include <list>
#include <algorithm>
#include <string>

// [실습 설명]
// C++ STL의 std::list 사용법을 익히는 실습입니다.
// std::list는 이중 연결 리스트(Doubly Linked List)로 구현되어 있습니다.
// 임의 접근(인덱스 [])은 불가능하지만, 
// 반복자(Iterator)를 통한 삽입/삭제는 O(1)로 매우 빠릅니다.

using namespace std;

void PrintList(const list<int>& l) {
    cout << "List: ";
    for (auto val : l) {
        cout << val << " ";
    }
    cout << "\n";
}

int main() {
    cout << "=== std::list 실습 ===\n";

    // 1. 리스트 생성 및 초기화
    // TODO: int형 데이터를 담는 리스트 'L'을 생성하고 1, 2, 3을 넣으세요.
    // 힌트: push_back 사용
    list<int> L;
    L.push_back(1);
    L.push_back(2);
    L.push_back(3);

    // (검증용 출력)
    PrintList(L);


    // 2. 앞/뒤 삽입
    cout << "\n[2] 앞/뒤 삽입 실습\n";
    // TODO: 리스트의 맨 앞에 0을 추가하세요. (push_front)
    L.push_front(0);
    // TODO: 리스트의 맨 뒤에 4를 추가하세요. (push_back)
    L.push_back(4);
    PrintList(L);


    // 3. 반복자(Iterator) 사용 (매우 중요!)
    cout << "\n[3] 반복자(Iterator) 이동 실습\n";
    // TODO: begin()을 사용하여 첫 번째 원소를 가리키는 반복자 'it'를 만드세요.
    // std::list<int>::iterator it = ... (c++11 auto 사용 권장)
    list<int>::iterator it = L.begin();
    // TODO: 반복자를 3칸 뒤로 이동시키세요. (it++, it++...)
    it++; it++; it++;

    // TODO: 현재 반복자가 가리키는 값(*it)을 출력해보세요. (예상: 3)
    cout << "(*it)=" << (*it) << endl;


    // 4. 중간 삽입 (Insert)
    cout << "\n[4] 중간 삽입 실습\n";
    // TODO: 현재 반복자 위치(3) 앞에 500을 삽입하세요. (L.insert)
    // list 의 iterator 는 가리키는곳 왼쪽에 삽입한다.
    // 단 가리키는곳은 변함없다. 
    L.insert(it, 500);

    PrintList(L);


    // 5. 중간 삭제 (Erase)
    cout << "\n[5] 중간 삭제 실습\n";
    // TODO: 현재 반복자가 가리키는 값(3)을 삭제하세요. (L.erase)
    // 주의: erase 후에는 반복자가 무효화되므로 반환값을 다시 받아야 안전합니다!
    // it = L.erase(it);
    // 해당 이터레이터 포인터가 가리키는 값을 삭제하고 새로은 값을 가리키기 때문에 다시 이터레이터 포인터를 받아야한다.
    it = L.erase(it);

     PrintList(L);


    // 6. [심화] 짝수만 삭제하기 (Remove If)
    cout << "\n[6] 조건 삭제 실습 (짝수 제거)\n";
    // 힌트: for문과 iterator를 사용하여 짝수면 erase 하세요.
    // !!주의!!: 반복문 안에서 erase를 할 때는 'it = L.erase(it)' 패턴을 꼭 지켜야 합니다.
    // 그렇지 않으면 삭제된 반복자를 it++ 하려다가 런타임 에러가 발생합니다.
    /*
    for (auto it = L.begin(); it != L.end(); ) {
        if (*it % 2 == 0) {
            // TODO: 짝수 지우기
        }
        else {
            it++;
        }
    }
    */
    for (list<int>::iterator it = L.begin(); it != L.end();) {
        if (*it % 2 == 0) {
            it = L.erase(it);
        }
        else {
            it++;
        }
    }

     PrintList(L);

    return 0;
}
