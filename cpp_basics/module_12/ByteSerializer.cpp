#pragma once
#include "ByteSerializer.h"
#include "GameMessage.h"
// [코드 평가 피드백] (헤더 관리)
// 1. 헤더에 있는 `<optional>`, `<vector>` 등이 무더기로 중복되어 있습니다.
// 2. C++에서는 `<stdint.h>`(C언어용)와 `<cstdint>`(C++용)를 섞어 쓰지 말고 `<cstdint>`만 쓰는 것이 표준 패턴입니다.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <optional>
#include <stdint.h>
#include <vector>
#include <winsock.h>

int ByteStream::BeginWrite()
{
    // [코드 평가 피드백] (런타임 오버헤드 및 휴먼 에러)
    // (문제점) `BeginWrite`와 모든 `Write~` 함수 안에서 매번 `if (this->mode.has_value())`를 통해 락이 걸렸는지 런타임 검사(Runtime Check)를 수행하고 있습니다.
    // (왜 발생하는가?) 개발자가 락을 걸지 않고 함수를 호출하는 실수를 잡기 위한 방어 코드입니다. 하지만 데이터 1바이트를 쓸 때마다 if문 검사를 하면 파이프라인 분기 예측 지연으로 인해 CPU 사이클을 낭비합니다. 또한 개발자가 `EndWrite()`를 호출하지 않고 빠져나가는(메모리 릭과 비슷한) 휴먼 에러는 구조적으로 막을 수 없습니다.
    // (수정 방법) 생성자와 소멸자로 수명을 관리하는 RAII 패턴을 클래스로 분리해야 합니다.
    // 수정 예시: `ByteStream` 객체를 감싸는 가드 객체를 만듭니다.
    // class StreamWriterGuard {
    //     ByteStream& stream;
    // public:
    //     StreamWriterGuard(ByteStream& s) : stream(s) { stream.BeginWrite(); } // 생성 시 자동 락
    //     ~StreamWriterGuard() { stream.EndWrite(); } // 블록 벗어나면 자동 해제
    // };
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::ReadLock)
        {
            std::cout << "This Stream Already Openned With ReadStream"
                      << std::endl;
        }
        if (curMode == _LockMode::WriteLock)
        {
            std::cout << "This Stream Already Openned With WriteStream"
                      << std::endl;
        }
        return 1;
    }
    this->mode = _LockMode::WriteLock;
    return 0;
}
int ByteStream::EndWrite()
{
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::ReadLock)
        {
            std::cout << "This Stream Already Openned With ReadStream"
                      << std::endl;
            return 1;
        }
        this->mode = std::nullopt;
        return 0;
    }
    std::cout << "This Stream Doesn't Has Lock" << std::endl;
    return 1;
}
int ByteStream::BeginRead(int &fromIdx)
{
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::ReadLock)
        {
            std::cout << "This Stream Already Openned With ReadStream"
                      << std::endl;
        }
        if (curMode == _LockMode::WriteLock)
        {
            std::cout << "This Stream Already Openned With WriteStream"
                      << std::endl;
        }
        return 1;
    }
    this->mode = _LockMode::ReadLock;
    fromIdx = 0;
    return 0;
}
int ByteStream::EndRead()
{
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::WriteLock)
        {
            std::cout << "This Stream Already Openned With WriteStream"
                      << std::endl;
            return 1;
        }
        this->mode = std::nullopt;
        return 0;
    }
    std::cout << "This Stream Doesn't Has Lock" << std::endl;
    return 1;
}
void ByteStream::Clear()
{
    this->_byteVec.clear();
}
int ByteStream::_CheckCurrentWrite()
{
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::ReadLock)
        {
            std::cout << "This Stream Already Openned With ReadStream"
                      << std::endl;
            return 1;
        }
        return 0;
    }
    std::cout << "This Stream Doesn't Has Lock" << std::endl;
    return 1;
}
int ByteStream::_CheckCurrentRead()
{
    if (this->mode.has_value())
    {
        _LockMode curMode = this->mode.value();
        if (curMode == _LockMode::WriteLock)
        {
            std::cout << "This Stream Already Openned With WriteStream"
                      << std::endl;
            return 1;
        }
        return 0;
    }
    std::cout << "This Stream Doesn't Has Lock" << std::endl;
    return 1;
}

void ByteStream::WriteInt(int a)
{
    // byteVec.push_back((uint8_t)sizeof(int));
    // htonl을 위해 uint32_t 로 변환한다.
    // 강제로 값의 주소만을 넘겨서 읽는 방식을 1바이트씩 읽도록 변환한다.
    if (_CheckCurrentWrite() != 0)
    {
        return;
    }
    if (_isNetwork)
    {
        a = ntohl(a);
    }
    uint8_t *start = reinterpret_cast<uint8_t *>(&a);
    uint8_t *end = start + sizeof(int);
    this->_byteVec.insert(this->_byteVec.end(), start, end);
}

void ByteStream::WriteCharArray(char *cArray, int arrayLen)
{
    if (_CheckCurrentWrite() != 0)
    {
        return;
    }
    // [코드 평가 피드백] (알고리즘 오버헤드 - 점진적 메모리 할당)
    // (문제점) for 루프 안에서 1바이트씩 `push_back`을 호출하고 있습니다.
    // (왜 발생하는가?) `std::vector`는 여유 공간(Capacity)이 꽉 찰 때마다 새로운 큰 힙 메모리를 통째로 다시 할당받고 기존 데이터를 전체 복사(Reallocation)하는 무거운 방식으로 동작합니다. 1024바이트를 1바이트씩 넣으면 메모리 이사를 수없이 반복하게 되어 극심한 퍼포먼스 저하가 발생합니다.
    // (수정 방법) 필요한 공간을 한 번에 예약하거나, 데이터를 벌크(Bulk)로 한 방에 밀어 넣어야 합니다.
    // 수정 예시: 루프를 완전히 지우고 다음 한 줄로 대체하세요.
    // _byteVec.insert(_byteVec.end(), cArray, cArray + arrayLen);
    for (int i = 0; i < arrayLen; i++)
    {
        _byteVec.push_back((uint8_t)cArray[i]);
    }
}

void ByteStream::WriteMsgType(MessageType type)
{
    if (_CheckCurrentWrite() != 0)
    {
        return;
    }

    this->_byteVec.push_back(static_cast<uint8_t>(type));
}

void ByteStream::WriteFloat(float f)
{
    if (_CheckCurrentWrite() != 0)
    {
        return;
    }
    // 같은 바이트 크기의 포인터로 주소상의 값을 읽도록 캐스팅
    // 이유는 network 직렬화의 경우 htonl 을 하기 위해서
    uint32_t f32 = *reinterpret_cast<uint32_t *>(&f);
    if (_isNetwork)
    {
        f32 = htonl(f32);
    }
    // 그리고 uint8_t 로의 한바이트씩 이동하기 위한 캐스팅을 강제로 진행한다.
    uint8_t *start = reinterpret_cast<uint8_t *>(&f32);
    uint8_t *end = start + sizeof(float);
    _byteVec.insert(_byteVec.end(), start, end);
}

std::optional<int> ByteStream::ReadInt(int &lastIdx)
{
    if (_CheckCurrentRead() != 0)
    {
        std::cout << "what is this fuxking compiler" << std::endl;
        return std::nullopt;
    }
    if (lastIdx + sizeof(int) > _byteVec.size())
    {
        return std::nullopt;
    }

    uint32_t nValue;
    // [코드 평가 피드백]
    // (비효율적인 부분) 4바이트 정수를 읽을 때마다 범용 복사 함수인 `memcpy`를 호출하여 함수 호출 오버헤드가 발생합니다.
    // (개선 방안) 직접 포인터 캐스팅 `nValue = *reinterpret_cast<uint32_t*>(&_byteVec[lastIdx]);` 을 사용하면 어셈블리 단에서 한 번의 메모리 읽기 명령어(MOV)로 최적화되어 훨씬 빠릅니다. (단, Strict Aliasing 규칙 고려 필요)
    memcpy(&nValue, &_byteVec[lastIdx], sizeof(int));
    if (_isNetwork)
    {
        nValue = ntohl(nValue);
    }
    int rs = static_cast<int>(nValue);
    lastIdx += sizeof(int);
    return rs;
}

void ByteStream::ReadCharArray(char *fromPtr, size_t &fromLen, int &lastIdx)
{
    int e = lastIdx + fromLen;
    if (e > _byteVec.size())
    {
        return;
    }
    // [코드 평가 피드백] (메모리 복사 최적화 - 루프 전개)
    // (문제점) 1바이트 단위로 `for`문을 돌면서 대입 연산(`=`)을 수행하고 있습니다.
    // (왜 발생하는가?) CPU는 메모리를 1바이트씩 옮길 때마다 캐시 라인을 접근하며 심각한 병목을 겪습니다. 최신 CPU들은 16바이트, 32바이트씩 한 번에 메모리를 옮기는 SIMD 명령어(AVX 등)를 지원하는데, 사람이 짠 단순 for문은 컴파일러가 이 병렬 가속 명령어로 최적화해주지 못할 가능성이 매우 높습니다.
    // (수정 방법) 운영체제와 컴파일러가 하드웨어 단의 병렬 최적화를 가장 잘 걸어주는 메모리 복사 전용 함수 `std::memcpy`를 사용해야 압도적으로 빠릅니다.
    // 수정 예시: for 루프를 완전히 지우고 다음 한 줄로 대체하세요.
    // std::memcpy(fromPtr, &_byteVec[lastIdx], fromLen);
    for (int i = lastIdx; i < e; i++)
    {
        fromPtr[i - lastIdx] = _byteVec[i];
    }
    lastIdx = e;
}

std::optional<MessageType> ByteStream::ReadMsgType(int &lastIdx)
{
    if (lastIdx + sizeof(MessageType) > _byteVec.size())
    {
        return std::nullopt;
    }
    MessageType rs = static_cast<MessageType>(_byteVec[lastIdx]);
    lastIdx += sizeof(MessageType);
    return rs;
}

std::optional<float> ByteStream::ReadFloat(int &lastIdx)
{
    if (lastIdx + sizeof(float) > _byteVec.size())
    {
        return std::nullopt;
    }
    uint32_t nValue;
    memcpy(&nValue, &_byteVec[lastIdx], sizeof(float));
    if (_isNetwork)
    {
        nValue = ntohl(nValue);
    }

    uint8_t *bytePtr = reinterpret_cast<uint8_t *>(&nValue);
    float rs;
    memcpy(&rs, bytePtr, 4);
    lastIdx += sizeof(float);
    return rs;
}

std::vector<uint8_t> ByteStream::GetData()
{
    return this->_byteVec;
}
