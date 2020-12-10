#ifndef CRYPTO_UTIL_H
#define CRYPTO_UTIL_H

#include "stdint.h"
#include <string>

using namespace std;

#define CRYPTO_KEY_SIZE 8

namespace crypto
{
    static uint64_t _key = 0xa5fd316c45b93211;

    static string& xorEncrypt(string& str, uint64_t& key = crypto::_key)
    {
        char* key_array = reinterpret_cast<char*>(key);
        int len = str.length();
        for (size_t i = 0; i < CRYPTO_KEY_SIZE; ++i)
            str[i] ^= key_array[i % CRYPTO_KEY_SIZE];
        return str;
    } 

    static string& xorEncrypt(const char* _str, uint64_t& key = crypto::_key)
    {
        string str(_str);
        char* key_array = reinterpret_cast<char*>(key);
        int len = str.length();
        for (size_t i = 0; i < CRYPTO_KEY_SIZE; ++i)
            str[i] ^= key_array[i % CRYPTO_KEY_SIZE];
        return str;
    }

    static string& xorDecrypt(string& str, uint64_t& key = crypto::_key)
    {
        char* key_array = reinterpret_cast<char*>(key);
        int len = str.length();
        for (size_t i = 0; i < CRYPTO_KEY_SIZE; ++i)
            str[i] ^= key_array[i % CRYPTO_KEY_SIZE];
        return str;
    }

    static uint64_t stringSum(const string& str)
    {
        uint64_t sum = 0;
        for (char c : str)
            sum += (uint64_t)c;
        return sum;
    }

    static uint64_t stringSum(char* const str, size_t size)
    {
        uint64_t sum = 0;
        char* _str = str;
        for (int i = 0; i < size; ++i)
            sum += (uint64_t)*(_str++);
        return sum;
    }
}

#endif