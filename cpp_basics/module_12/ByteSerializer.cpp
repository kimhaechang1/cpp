#pragma once
#include "ByteSerializer.h"
#include "GameMessage.h"
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
