#include "GameMessage.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <vector>
#include <winsock.h>

class ByteStream
{

  private:
    std::vector<uint8_t> _byteVec;
    bool _isNetwork;
    enum class _LockMode { ReadLock, WriteLock };
    std::optional<_LockMode> mode = std::nullopt;
    int _CheckCurrentWrite();
    int _CheckCurrentRead();

  public:
    ByteStream() : _isNetwork(false) {}
    ByteStream(bool _isNetwork) : _isNetwork(_isNetwork) {}
    ByteStream(std::vector<uint8_t> &_other, bool _isNetwork)
        : _isNetwork(_isNetwork)
    {
        _byteVec.insert(_byteVec.end(), _other.begin(), _other.end());
    }
    int BeginWrite();
    int EndWrite();
    int BeginRead(int &fromIdx);
    int EndRead();
    void Clear();
    void WriteInt(int v);
    void WriteCharArray(char *cArray, int arrayLen);
    void WriteMsgType(MessageType type);
    void WriteFloat(float f);
    std::optional<int> ReadInt(int &lastIdx);
    void ReadCharArray(char *fromPtr, size_t &fromLen, int &lastIdx);
    std::optional<MessageType> ReadMsgType(int &lastIdx);
    std::optional<float> ReadFloat(int &lastIdx);
    std::vector<uint8_t> GetData();
};