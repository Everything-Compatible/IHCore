#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <functional>

#define SHA1_RESULT_BIT 160
#define SHA1_RESULT_UINT8 (SHA1_RESULT_BIT / 8)         // 20
#define SHA1_RESULT_UINT32 (SHA1_RESULT_UINT8 / 4)      // 5

#define SHA1_BLOB_BIT 512
#define SHA1_BLOB_UINT8 (SHA1_BLOB_BIT / 8)             // 64
#define SHA1_BLOB_UINT32 (SHA1_BLOB_UINT8 / 4)          // 16

#define SHA1_REST_BIT 448
#define SHA1_REST_UINT8 (SHA1_REST_BIT / 8)             // 56
#define SHA1_REST_UINT32 (SHA1_REST_UINT8 / 4)          // 14

#define SHA1_OPERATION_CNT 80
#define SHA1_ROUND_CNT 4
#define SHA1_ROUND_LEN (SHA1_OPERATION_CNT / SHA1_ROUND_CNT) // 20

class SHA1Digest
{
public:
    SHA1Digest() : _byteCount(0)
    {
        Reset();
    }
    SHA1Digest(const void *input, size_t length) : SHA1Digest() {}

    void Update(const void *input, size_t length)
    {
        const unsigned char *buff = reinterpret_cast<const unsigned char *>(input);

        if (_countLow + (length << 3) < _countLow)
        {
            ++_countHight;
        }
        _countLow += length << 3;
        _countHight += length >> 29;

        while (length--)
        {
            reinterpret_cast<unsigned char *>(&_data[0])[_byteCount++] = *buff++;
            if (_byteCount == SHA1_BLOB_UINT8)
            {
                // TODO : 1. group 512-bits/64-bytes
                BytesReverse(_data, SHA1_BLOB_UINT32);
                Transform();
                _byteCount = 0;
            }
        }
    }

    const unsigned char *DigestText()
    {
        Final();
        return _digest;
    }
    std::string GetString(bool isUpcase = true)
    {
        const unsigned char *pstr = DigestText();
        size_t length = GetSize();

        // TODO : 10. get 40-chars string
        return BytesToHex(pstr, length, isUpcase);
    }
    size_t GetSize()
    {
        return SHA1_RESULT_UINT8;
    }
    void Reset()
    {
        InitH();
        _countLow = _countHight = 0;
        _byteCount = 0;
        std::memset(_data, 0, sizeof(_data));
        std::memset(_digest, 0, sizeof(_digest));
    }
private:
    SHA1Digest(const SHA1Digest &) = delete;
    SHA1Digest & operator = (const SHA1Digest &) = delete;


    void Transform()
    {
        uint32_t alpha[SHA1_RESULT_UINT32];
        uint32_t W[SHA1_OPERATION_CNT];

        // TODO : 2. fill W[] (0 ~ 15)
        for (int i = 0; i < SHA1_BLOB_UINT32; W[i] = _data[i], ++i) {}

        // TODO : 3. fill W[] (16 ~ 79)
        for (int i = SHA1_BLOB_UINT32; i < SHA1_OPERATION_CNT; W[i] = Expand(W, i), ++i) {}

        // TODO : 4. fill A, B, C, D, E
        for (int i = 0; i < SHA1_RESULT_UINT32; alpha[i] = _h[i], ++i) {}

        // TODO : 5. operator round 80
        for (int i = 0; i < SHA1_OPERATION_CNT; Round(alpha, W, i++)) {}

        // TODO : 6. update H[]
        for (int i = 0; i < SHA1_RESULT_UINT32; _h[i] += alpha[i], ++i) {}
    }
    void Final()
    {
        static unsigned int bitsOfByte = 8;

        // TODO : 7. cover bit (1)
        reinterpret_cast<unsigned char *>(&_data[0])[_byteCount++] = 0x80;

        // TODO : 7. cover bit (000...000)
        std::memset(reinterpret_cast<unsigned char *>(&_data[0]) + _byteCount, 0, SHA1_BLOB_UINT8 - _byteCount);
        if (_byteCount > SHA1_REST_UINT8)
        {
            BytesReverse(_data, SHA1_BLOB_UINT32);
            Transform();
            std::memset(_data, 0, sizeof(_data));
        }
        
        BytesReverse(_data, SHA1_BLOB_UINT32);

        // TODO : 8. add bits count
        _data[14] = _countHight;
        _data[15] = _countLow;

        Transform();

        // TODO : 9. get bytes array from words array
        for (int i = 0; i < SHA1_RESULT_UINT8; ++i)
        {
            _digest[i] = _h[i >> 2] >> (bitsOfByte * (3 - (i & 0x03))) & 0xFF;
        }
    }
    void InitH()
    {
        for (int i = 0; i < SHA1_RESULT_UINT32; _h[i] = H_INIT[i], ++i) {}
    }

private:
    uint32_t _h[SHA1_RESULT_UINT32];
    uint32_t _data[SHA1_BLOB_UINT32];
    uint32_t _countLow;
    uint32_t _countHight;
    uint32_t _byteCount;
    unsigned char _digest[SHA1_RESULT_UINT8];

private:
    static uint32_t H_INIT[SHA1_RESULT_UINT32];
    static uint32_t K[SHA1_ROUND_CNT];
    static std::function<uint32_t(uint32_t, uint32_t, uint32_t)> F[SHA1_ROUND_CNT];
    static uint32_t S(uint32_t x, int n);
    static uint32_t Expand(uint32_t W[], int i);
    static void Round(uint32_t alpha[], uint32_t W[], int i);

public:
    static std::string BytesToHex(const unsigned char *input, size_t length, bool isUpcase);
    static void BytesReverse(uint32_t data[], size_t length);
};

uint32_t SHA1Digest::H_INIT[SHA1_RESULT_UINT32] =
{
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};

uint32_t SHA1Digest::K[SHA1_ROUND_CNT] =
{
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
};

std::function<uint32_t(uint32_t, uint32_t, uint32_t)> SHA1Digest::F[SHA1_ROUND_CNT] =
{
    [] (uint32_t x, uint32_t y, uint32_t z) -> uint32_t
    {
        return (x & y) | (~x & z);
    },
    [] (uint32_t x, uint32_t y, uint32_t z) -> uint32_t
    {
        return x ^ y ^ z;
    },
    [] (uint32_t x, uint32_t y, uint32_t z) -> uint32_t
    {
        return (x & y) | (x & z) | (y & z);
    },
    [] (uint32_t x, uint32_t y, uint32_t z) -> uint32_t
    {
        return x ^ y ^ z;
    },
};

uint32_t SHA1Digest::S(uint32_t x, int n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t SHA1Digest::Expand(uint32_t W[], int i)
{
    return S((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1);
}

void SHA1Digest::Round(uint32_t alpha[], uint32_t W[], int i)
{
    uint32_t & A = alpha[0];
    uint32_t & B = alpha[1];
    uint32_t & C = alpha[2];
    uint32_t & D = alpha[3];
    uint32_t & E = alpha[4];

    uint32_t tmp = S(A, 5) + F[i / SHA1_ROUND_LEN](B, C, D) + E + W[i] + K[i / SHA1_ROUND_LEN];
    E = D;
    D = C;
    C = S(B, 30);
    B = A;
    A = tmp;
}

std::string SHA1Digest::BytesToHex(const unsigned char *input, size_t length, bool isUpcase)
{
    static const char *digitUpper = "0123456789ABCDEF";
    static const char *digitLower = "0123456789abcdef";

    const char *pstr = isUpcase ? digitUpper : digitLower;

    std::string ret;
    ret.reserve(length << 1);
    for (unsigned int i = 0; i < length; ++i)
    {
        ret.push_back(pstr[(input[i] & 0xF0) >> 4]);
        ret.push_back(pstr[input[i] & 0x0F]);
    }
    return ret;
}

void SHA1Digest::BytesReverse(uint32_t data[], size_t length)
{
    static unsigned int bitsOfByte= 8;

    for (unsigned int i = 0; i < length; ++i)
    {
        data[i] = ((data[i] >> (0 * bitsOfByte) & 0xFF) << (3 * bitsOfByte)) |
                    ((data[i] >> (1 * bitsOfByte) & 0xFF) << (2 * bitsOfByte)) |
                    ((data[i] >> (2 * bitsOfByte) & 0xFF) << (1 * bitsOfByte)) |
                    ((data[i] >> (3 * bitsOfByte) & 0xFF) << (0 * bitsOfByte));
    }
}

std::string GetSHA1(const void* Buf, size_t Size)
{
    SHA1Digest Dg;
    Dg.Update(Buf, Size);
    return Dg.GetString(false);
}