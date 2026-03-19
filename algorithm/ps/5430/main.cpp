#include <iostream>
#include<sstream>
#include<vector>
using namespace std;
int T;
string fn;
int n;
string in;
const int MX = 300000;
int deq[MX * 2 + 5];
int fi = MX / 2;
int bi = MX / 2;
void PushBack(int x) {
    deq[bi] = x;
    bi++;
}
void PushFront(int x) {
    fi--;
    deq[fi] = x;
}
int Size() {
    return bi - fi;
}
bool Empty() {
    return Size() == 0;
}
int Front() {
    if (Empty()) {
        return -1;
    }
    return deq[fi];
}
int Back() {
    if (Empty()) {
        return -1;
    }
    return deq[bi - 1];
}
int PopBack() {
    if (Empty()) {
        return -1;
    }
    int rs = Back();
    bi--;
    return rs;
}
int PopFront() {
    if (Empty()) {
        return -1;
    }
    int rs = Front();
    fi++;
    return rs;
}
void Debug();

void Input() {
    cin >> fn;
    cin >> n;
    cin >> in;
    fi = MX / 2;
    bi = MX / 2;
    for (int i = 0; i < in.length(); i++) {
        int cur = 0;
        if (in[i] - '0' < 0 || in[i] - '0' > 9) {
            continue;
        }
        for (int j = i; j < in.length(); j++) {
            if (in[j] - '0' < 0 || in[j] - '0' > 9) {
                i = j;
                break;
            }
            cur = (cur * 10 + (in[j] - '0'));
            
           
        }
        PushBack(cur);
    }
}

void DebugDeq() {
    for (int i = fi; i < bi; i++) {
        cout << deq[i] << " ";
    }
    cout << endl;
}

string Print(bool flag) {
    stringstream stream;
    stream << "[";
    if (flag) {
        for (int i = fi; i < bi; i++) {
            stream << deq[i];
            if (i < bi - 1) {
                stream << ",";
            }
            
        }
    }
    else {
        for (int i = bi - 1; i >= fi; i--) {
            stream << deq[i];
            if (i > fi) {
                stream << ",";
            }
        }
    }
    

    stream << "]";

    return stream.str();
}


string Test() {
    // 현재가 정방인지 반대인지 알면된다.
    bool isLeftToRight = true;
    for (int i = 0; i < fn.length(); i++) {
        if (fn[i] == 'R') {
            isLeftToRight = !isLeftToRight;
        }
        else {
            if (Empty()) {
                return "error";
            }
            if (isLeftToRight) {
                PopFront();
            }
            else {
                PopBack();
            }
        }
    }   
    return Print(isLeftToRight);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> T;
    stringstream stream;
    while (T-- > 0) {
        Input();
        stream << Test() << endl;
    }
    cout << stream.str();
    return 0;
}
