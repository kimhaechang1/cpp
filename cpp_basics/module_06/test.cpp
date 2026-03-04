#include <iostream>
#include <cassert>

class SlimeSpawner;
class Slime {
    static int totalCount;
    friend class SlimeSpawner;
    Slime() { totalCount++; }
public:
    ~Slime() { totalCount--; }
    static int GetTotalCount() { return totalCount; }
};
int Slime::totalCount = 0;

class SlimeSpawner {
public:
    Slime CreateSlime() {
        Slime slime;
        return slime;
    }
};

int main() {
    SlimeSpawner spawner;
    Slime s1 = spawner.CreateSlime();
    std::cout << "Count: " << Slime::GetTotalCount() << "\n";
    return 0;
}
