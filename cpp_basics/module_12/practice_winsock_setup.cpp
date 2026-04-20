#define WIN32_LEAN_AND_MEAN
#include <format> // C++20 standard
#include <iostream>
#include <memory>
#include <winsock2.h>

// TODO: NetworkManager 클래스를 구현하세요 (RAII)
// - 생성자: WSAStartup
// - 소멸자: WSACleanup
// - 메서드: PrintSystemInfo()
using namespace std;

class NetworkManager
{
  private:
    static inline shared_ptr<NetworkManager> manager = nullptr;
    WSAData wsaData;
    NetworkManager()
    {

        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        cout << "WSA Result: " << result << endl;
    }
    ~NetworkManager()
    {
        WSACleanup();
    }

  public:
    NetworkManager(const NetworkManager &other) = delete;
    NetworkManager &operator=(const NetworkManager &other) = delete;

    static shared_ptr<NetworkManager> GetInstance()
    {
        class _Inner : public NetworkManager
        {
          public:
            _Inner() : NetworkManager()
            {
            }
        };
        if (manager == nullptr) {
            manager = make_shared<_Inner>();
        }
        return manager;
    }
    static void Destory_Instance()
    {
        if (manager != nullptr) {
            manager = nullptr;
        }
    }

    void PrintSystemInfo()
    {
        int major = wsaData.wVersion & 0xFF;
        int minor = (wsaData.wVersion >> 8) & 0xFF;

        cout << "--- Network System Info ---" << endl;
        cout << "Version: " << major << "." << minor << endl; // 2.2 출력!
        cout << "Description: " << wsaData.szDescription << endl;
        cout << "Memory Size: " << sizeof(wsaData) << " bytes" << endl;
        cout << "---------------------------" << endl;
    }
};

int main()
{
    // TODO: NetworkManager를 인스턴스화하고 정보를 출력하세요.
    auto manager = NetworkManager::GetInstance();
    manager->PrintSystemInfo();

    NetworkManager::Destory_Instance();
    return 0;
}
