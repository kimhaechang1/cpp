#include <iostream>
#include <string>

using namespace std;

int main() {
	int A, B, C;
	cin >> A;
	cin >> B;
	cin >> C;
	int result = A * B * C;
	string S = to_string(result);
	int cnt[10] = { 0, };
	for (char c : S) {
		cnt[c - '0']++;
	}
	for (int c : cnt) {
		cout << c << endl;
	}
}