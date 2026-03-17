#include <iostream>
#include<vector>
using namespace std;

int N;
int M;
const int MX = 5005;
vector<int> arr(M);
// 오른쪽으로 이동한단것은 오른쪽끝 원소도 하나 뽑는다는것
// deque 쓰는게 좋음

int deq[MX * 2 + 5];
int fId = MX;
int bId = MX;
int Size() {
    return bId - fId;
}
bool Empty() {
    return Size() == 0;
}
int Back() {
    return deq[bId - 1];
}
int Front() {
    return deq[fId];
}
int PopFront() {
    int rv = Front();
    fId++;
    return rv;
}
int PopBack() {
    int rv = Back();
    bId--;
    return rv;
}
void PushFront(int x) {
    fId--;
    deq[fId] = x;
}
void PushBack(int x) {
    deq[bId] = x;
    bId++;
}

void Debug() {
    for (int i = fId; i < bId; i++) {
        cout << deq[i] << ", ";
    }
    cout << "###############################" << endl;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N;
    cin >> M;
    for (int i = 0; i < M; i++) {
        int x;
        cin >> x;
        arr.push_back(x);
    }
    for (int i = 1; i <= N; i++) {
        PushBack(i);
    }
    //Debug();
    int result = 0;
    for (int i = 0; i < M; i++) {
        int mId = 999;
        for (int j = fId; j < bId; j++) {
            if (deq[j] == arr[i]) {
                int left = j - fId;
                int right = bId - j - 1;
                //cout << "left" << left << endl;
                //cout << "right" << right << endl;
                mId = min(left, right + 1);
                //cout << "mId: " << mId << endl;
                if (left < right) {
                    while (left-- > 0) {
                        PushBack(PopFront());
                    }
                }
                else {
                    while (right-- > 0) {
                        PushFront(PopBack());
                    }
                    PushFront(PopBack());
                }
                PopFront();
                result += mId;
                break;
            }
        }
        //Debug();
        
    }
    cout << result;
    return 0;
}
