#include<iostream>
#include<string>
#include<sstream>

using namespace std;

static const int SIZE = 10000;
int deq[SIZE * 2 + 5];
int N;
int head = SIZE;
int tail = SIZE;
int Size() {
	return tail - head;
}
int Empty() {
	if (Size() == 0) {
		return 1;
	}
	return 0;
}
void Push_Front(int x) {
	head--;
	deq[head] = x;
}
void Push_Back(int x) {
	deq[tail] = x;
	tail++;
}

int Front() {
	if (Empty() == 1) {
		return -1;
	}
	return deq[head];
}
int Back() {
	if (Empty() == 1) {
		return -1;
	}

	return deq[tail - 1];
}
int Pop_Front() {
	if (Empty() == 1) {
		return  -1;
	}
	int rs = Front();
	head++;
	return rs;
}
int Pop_Back() {
	if (Empty() == 1) {
		return  -1;
	}
	int rs = Back();
	tail--;
	return rs;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cin >> N;
	stringstream stream;
	while (N-- > 0) {
		string op;
		cin >> op;
		int x;
		if (op == "push_back") {
			cin >> x;
			Push_Back(x);
		}
		else if (op == "push_front") {
			cin >> x;
			Push_Front(x);
		}
		else if (op == "front") {
			stream << Front() << endl;
		}
		else if (op == "back") {
			stream << Back() << endl;
		}
		else if (op == "pop_front") {
			stream << Pop_Front() << endl;
		}
		else if (op == "pop_back") {
			stream << Pop_Back() << endl;
		}
		else if (op == "size") {
			stream << Size() << endl;
		}
		else if (op == "empty") {
			stream << Empty() << endl;
		}
	}
	cout << stream.str();
	
}