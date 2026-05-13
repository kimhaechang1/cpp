#pragma once
#include "ByteSerializer.h"
#include "GameMessage.h"
#include <cstdint>
#include <cstring>

void ByteWriter::WriteInt(int a)
{

    // 강제로 값의 주소만을 넘겨서 읽는 방식을 1바이트씩 읽도록 변환한다.
    if (_isNetwork)
    {
        a = htonl(a);
    }
    // 여기서 이제 reinterpret_cast 를 한 이유는 값이 아니라 바이트단위로 읽게 하기 위해서이다.
    // 원본의 손실없이 깔끔하게 가져오기 위해서이다.
    uint8_t *start = reinterpret_cast<uint8_t *>(&a);
    uint8_t *end = start + sizeof(int);
    this->_byteVec.insert(this->_byteVec.end(), start, end);
}

void ByteWriter::WriteCharArray(const char *cArray, int arrayLen)
{
    _byteVec.insert(_byteVec.end(), cArray, cArray + arrayLen);
}

void ByteWriter::WriteMsgType(MessageType type)
{
    // static cast 가 가능한이유는 우선 static cast 를 한다는것은 정적인 타입변환에 문제가 없다는것이다.
    // 즉, type 자체의 데이터를 읽는 방식에 달려있는데 MessageType Enum 은 uint8_t 타입으로 할당되어있다.
    // uint8_t 로 읽을 수 있다는것, 따라서 static_cast 를 진행한다.
    // 그리고 한바이트기 때문에 ntoh 나 hton등을 할 필요가 없다.
    this->_byteVec.push_back(static_cast<uint8_t>(type));
}

void ByteWriter::WriteFloat(float f)
{
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

std::optional<int> ByteReader::ReadInt()
{
    if (this->_cursor + sizeof(int) > this->_streamSize)
    {
        return std::nullopt;
    }

    uint32_t nValue;
    memcpy(&nValue, this->_rPtr + this->_cursor, sizeof(int));
    if (_isNetwork)
    {
        nValue = ntohl(nValue);
    }
    int rs = static_cast<int>(nValue);
    this->_cursor += sizeof(int);
    return rs;
}

void ByteReader::ReadCharArray(char *fromPtr, size_t &fromLen)
{
    int e = this->_cursor + fromLen;
    if (e > this->_streamSize)
    {
        return;
    }
    std::memcpy(fromPtr, this->_rPtr + _cursor, fromLen);
    _cursor = e;
}

std::optional<MessageType> ByteReader::ReadMsgType()
{
    if (_cursor + sizeof(MessageType) > this->_streamSize)
    {
        return std::nullopt;
    }
    uint8_t nValue;
    memcpy(&nValue, this->_rPtr + _cursor, sizeof(uint8_t));
    MessageType rs = static_cast<MessageType>(nValue);
    _cursor += sizeof(MessageType);
    return rs;
}

std::optional<float> ByteReader::ReadFloat()
{
    if (_cursor + sizeof(float) > this->_streamSize)
    {
        return std::nullopt;
    }
    // [추가 피드백] 여기도 `this->_rPtr + lastIdx` 로 오프셋을 더해야 합니다!
    uint32_t nValue;
    memcpy(&nValue, this->_rPtr + _cursor, sizeof(float));
    if (_isNetwork)
    {
        nValue = ntohl(nValue);
    }

    uint8_t *bytePtr = reinterpret_cast<uint8_t *>(&nValue);
    float rs;
    memcpy(&rs, bytePtr, 4);
    _cursor += sizeof(float);
    return rs;
}

std::vector<uint8_t> ByteWriter::GetData()
{
    return std::move(this->_byteVec);
}

int ByteReader::GetCurrentCursor()
{
    return this->_cursor;
}