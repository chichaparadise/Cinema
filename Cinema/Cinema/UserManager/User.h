#ifndef USER_H
#define USER_H

#include <string>
#include <list>
#include "stdint.h"
#include "..\fileUtils\fileUtil.h"
#include "..\Crypto\cryptoUtil.h"

using namespace std;

typedef enum user_attributes_t
{
    Admin = 0x01,
    // Authorised = 0x02,
    Blocked = 0x04
}UserAttributes;

class User
{
public:
    User()
    {
        this->attributes = 0;
    }

    User(const string& name, const string& crypted_password, const uint8_t& attributes, const list<int>& filmList)
    {
        this->attributes = attributes;
        this->cryptedPassword = crypted_password;
        this->name = name;
        this->filmList = filmList;
    }
    
    inline bool SetAttributes(const uint8_t attributes, string& password)
    {
        if (this->IsPasswordValid(password))
            this->attributes = attributes;
    }

    inline bool SetName(const string& name, string& password)
    {
        if (this->IsPasswordValid(password))
            this->name = name;
    }

    inline bool hasAttributes(const uint8_t& attributes)
    {
        return (this->attributes & attributes) == 0 ? false : true;
    }

    inline list<int>& GetFilms()
    {
        return this->filmList;
    }

    inline void AddFilm(const int& filmId)
    {
        //if (this->hasAttributes(UserAttributes::Authorised))
            filmList.push_front(filmId);
    }

    inline string GetName() const
    {
        return name;
    }

    inline string GetName()
    {
        return name;
    }

    inline bool IsPasswordValid(string password)
    {
        crypto::xorEncrypt(password);
        bool res = this->cryptedPassword == password;
        crypto::xorDecrypt(password);
        return res;
    } 
private:
    friend class UserManager;
    
    uint8_t attributes;

    string name;

    string cryptedPassword;

    list<int> filmList;
};

#endif USER_H