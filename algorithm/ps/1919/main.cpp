#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main() {
	string s1, s2;
	cin >> s1;
	cin >> s2;
	int cnt[26] = { 0, };
	for (char c : s1) cnt[c - 'a']++;
	for (char c : s2) cnt[c - 'a']--;
	int result = 0;
	for (int c : cnt) {
		if (c < 0) {
			result += (c * -1);
			continue;
		}
		result += c;
	}
	cout << result;
}