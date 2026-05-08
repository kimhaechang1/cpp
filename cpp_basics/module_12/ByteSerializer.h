// [코드 평가 피드백] (아키텍처 치명적 결함)
// (문제점) 헤더 가드(`#pragma once` 또는 `#ifndef`)가 없습니다. 이 헤더가 여러 곳에서 불리면 ODR(One Definition Rule) 위반으로 컴파일이 터질 수 있습니다!
#pragma once

#include "GameMessage.h"
// [코드 평가 피드백] (중복 인클루드)
// `GameMessage.h`에 이미 `<optional>`, `<vector>`, `<cstdint>`가 있으므로 지우는 것이 컴파일 최적화에 좋습니다.
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
        // [코드 평가 피드백] (메모리 통째 복사 이슈)
        // (문제점) 파싱을 위해 읽기 모드로 스트림을 열 때마다 원본 버퍼의 모든 데이터를 자신의 `_byteVec`으로 통째로 복사(`insert`)하고 있습니다.
        // (왜 발생하는가?) `std::vector` 간의 `insert`나 대입은 내부적으로 `new/malloc`을 통한 힙 메모리 할당과 `memcpy`를 수반합니다. 수천 명의 유저가 보내는 수백 바이트의 패킷을 파싱할 때마다 통째로 복사하면 CPU와 메모리에 엄청난 병목(Bottleneck)이 발생하여 서버 성능이 수직 하락합니다.
        // (수정 방법) 원본 벡터를 복사하지 말고, 읽기 전용 참조자나 메모리 주소(포인터)만 들고 있게 하는 Zero-Copy 패턴으로 리팩토링해야 합니다.
        // 수정 예시: `std::vector<uint8_t> _byteVec;` 대신 포인터와 크기만 저장하도록 멤버를 변경.
        // `const uint8_t* _readPtr = nullptr;` 와 같이 포인터만 들고 있게 수정한 뒤 `_readPtr = _other.data();` 로 주소만 복사.
        _byteVec.insert(_byteVec.end(), _other.begin(), _other.end());
    }
    // [코드 평가 피드백] (모던 C++ 패턴)
    // (문제점) `int`를 리턴해서 성공(0)/실패(1)를 나누는 것은 구형 C 언어 스타일입니다.
    // (개선 방향) `enum class ErrorCode`를 반환하거나 `bool`을 반환해야 의도가 명확해집니다.
    int BeginWrite();
    int EndWrite();
    int BeginRead(int &fromIdx);
    int EndRead();
    void Clear();
    void WriteInt(int v);
    // [코드 평가 피드백] (const 정확도 - Const Correctness)
    // (문제점) `char *cArray`처럼 `const`가 없는 포인터 매개변수는 "내가 원본 배열을 수정할 수도 있다"는 의미를 내포합니다.
    // (왜 발생하는가?) 직렬화 함수는 들어온 데이터를 읽어서 바이트 스트림에 밀어 넣기만 할 뿐 원본을 절대 수정하지 않습니다. 하지만 `const`를 붙이지 않으면, 나중에 `"Hero"` 같은 수정 불가능한 문자열 리터럴이나 `const char*` 데이터를 넘겨줄 때 컴파일 에러가 발생하여 함수의 활용도가 크게 떨어집니다.
    // (수정 방법) 수정 불가능한 참조를 받도록 `const`를 추가하세요. C++17에서는 메모리 할당 없이 포인터와 길이를 가볍게 묶어주는 `std::string_view`가 가장 우아한 선택입니다.
    // 수정 예시: `void WriteCharArray(std::string_view str);` 또는 최소한 `void WriteCharArray(const char *cArray, int arrayLen);`
    void WriteCharArray(char *cArray, int arrayLen);
    void WriteMsgType(MessageType type);
    void WriteFloat(float f);
    std::optional<int> ReadInt(int &lastIdx);
    void ReadCharArray(char *fromPtr, size_t &fromLen, int &lastIdx);
    std::optional<MessageType> ReadMsgType(int &lastIdx);
    std::optional<float> ReadFloat(int &lastIdx);
    std::vector<uint8_t> GetData();
};