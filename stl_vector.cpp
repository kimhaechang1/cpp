#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void func1(vector<int> v) {
    v[10] = 7;
    cout << "call by value: " << v.data() << endl;
}

void func2(vector<int> &v) {
    v[10] = 7;
    cout << "call by reference: " << v.data() << endl;
}

int main() {
    vector<int> v(100);
    cout << "original: " << v.data() << endl;
    func1(v);
    func2(v);
    return 0;
}