#ifndef USER_SESSION_H
#define USER_SESSION_H

#include <fstream>
#include "User.h"
#include "..\fileUtils\fileUtil.h"
#include "..\FilmManager\filmManager.h"
#include "..\Exceptions\exception.h"

#define READING_EXCEPTION 0x04
#define WRITING_EXCEPTION 0x08
#define USER_MANAGMENT_FAULT 0x10

class UserManager : base_entity
{
public:
    User* currentUser;

    FilmManager filmManager;

    list<User> userList;
public:
    UserManager(string str, FilmManager& filmManager) : base_entity((char*)str.c_str())
    {
        this->filmManager = filmManager;
    }

    bool Authorise(string& name, string& password)
    {
        for (auto user : this->userList)
        {
            if (user.GetName() == name)
                if (user.IsPasswordValid(password))
                {
                    this->currentUser = &user;
                    return true;
                }
        }
        return false;
    }

    inline void Logout()
    {
        this->currentUser = nullptr;
        return;
    }

    bool AddUser(const User& user)
    {
        auto name = user.GetName();
        bool isUnique = true;
        for (auto _user : this->userList)
            if (_user.GetName() == name)
                isUnique = false;
        if (isUnique)
        {
            this->userList.push_back(user);
            return true;
        }
        return false;
    }

    inline void SetAttributes(User& user, const uint8_t& attributes)
    {
        if (currentUser->hasAttributes(UserAttributes::Admin))
            user.attributes = attributes;
    }

    inline void SetName(User& user, const string& name)
    {
        user.name = name;
    }

    bool DeleteUser(string& name)
    {
        for (auto user : this->userList)
        {
            if (user.GetName() == name)
            {
                this->userList.remove(user);
                return true;
            }
        }
        return false;
    }

    bool ReadObject()
    {
        uint64_t sum = 0;
        uint64_t fileSum = 0;
        ifstream stream;
        stream.open(this->connection_string, ios::binary | ios::in);
        if (stream.is_open())
        {
            int size;
            stream >> size;
            for (int i = 0; i < size; ++i)
            {
                User* user = new User();
                stream >> user->name;
                sum += crypto::stringSum(user->name);
                stream >> user->cryptedPassword;
                sum += crypto::stringSum(user->cryptedPassword);
                stream >> user->attributes;
                sum += user->attributes;
                int filmSize;
                stream >> filmSize;
                sum += filmSize;
                for (int j = 0; j < filmSize; ++j)
                {
                    int filmID;
                    stream >> filmID;
                    sum += filmID;
                    user->filmList.push_back(filmID);
                }
                this->userList.push_back(*user);
                delete user;
            }
            stream >> fileSum;
            stream.close();
            if (fileSum == sum)
                return true;
            throw_exception("Invalid input file sum, object created from scratch", UserManager::UserManagerHandler, READING_EXCEPTION, (void*)this);
            return false;
        }
        stream.close();
        throw_exception("file open exception, terminating process", UserManager::UserManagerHandler, READING_EXCEPTION | error_type::critical_error, (void*)this);
        return false;
    }

    bool WriteObject()
    {
        uint64_t sum = 0;
        ofstream stream;
        stream.open(this->connection_string, ios::binary | ios::trunc | ios::out);
        if (stream.is_open())
        {
            stream << this->userList.size() << endl;
            for (auto user : this->userList)
            {
                sum += crypto::stringSum(user.name);
                stream << user.name << endl;
                sum += crypto::stringSum(user.cryptedPassword);
                stream << user.cryptedPassword << endl;
                sum += user.attributes;
                stream << user.attributes << endl;
                sum += user.filmList.size();
                stream << user.filmList.size() << endl;
                for (auto film : user.filmList)
                {
                    sum += film;
                    stream << film << endl;
                }
            }
            stream << sum <<endl;
            stream.close();
            return true;
        }
        stream.close();
        throw_exception("file open exception, terminating process", UserManager::UserManagerHandler, WRITING_EXCEPTION | error_type::critical_error, (void*)this);
        return false;
    }

    bool ReadObject(ofstream& in)
    {
        uint64_t sum = 0;
        uint64_t fileSum = 0;
        if (in.is_open())
        {
            int size;
            in << size;
            for (int i = 0; i < size; ++i)
            {
                User* user = new User();
                in << user->name;
                sum += crypto::stringSum(user->name);
                in << user->cryptedPassword;
                sum += crypto::stringSum(user->cryptedPassword);
                in << user->attributes;
                sum += user->attributes;
                int filmSize;
                in << filmSize;
                sum += filmSize;
                for (int j = 0; j < filmSize; ++j)
                {
                    int filmID;
                    in << filmID;
                    sum += filmID;
                    user->filmList.push_back(filmID);
                }
                this->userList.push_back(*user);
                delete user;
            }
            in << fileSum;
            if (fileSum == sum)
                return true;
            throw_exception("Invalid input file sum, object created from scratch", UserManager::UserManagerHandler, READING_EXCEPTION, (void*)this);
            return false;
        }
        throw_exception("file open exception, terminating process", UserManager::UserManagerHandler, READING_EXCEPTION | error_type::critical_error, (void*)this);
        return false;
    }

    bool WriteObject(ofstream& out)
    {
        uint64_t sum = 0;
        if (out.is_open())
        {
            out << this->userList.size() << endl;
            for (auto user : this->userList)
            {
                sum += crypto::stringSum(user.name);
                out << user.name << endl;
                sum += crypto::stringSum(user.cryptedPassword);
                out << user.cryptedPassword << endl;
                sum += user.attributes;
                out << user.attributes << endl;
                sum += user.filmList.size();
                out << user.filmList.size() << endl;
                for (auto film : user.filmList)
                {
                    sum += film;
                    out << film << endl;
                }
            }
            out << sum <<endl;
            return true;
        }
        throw_exception("file open exception, terminating process", UserManager::UserManagerHandler, WRITING_EXCEPTION | error_type::critical_error, (void*)this);
        return false;
    }

    static void UserManagerHandler(void* _this)
    {
        UserManager* error_manager = (UserManager * )_this;
        if (last_exception.ex_flags & error_type::critical_error)
            return;
        if (last_exception.ex_flags & READING_EXCEPTION)
        {
            string name = "Admin Admin";
            list<int> list;
            User Admin(name, crypto::xorEncrypt("Password"), UserAttributes::Admin, list);
            error_manager->userList.clear();
            error_manager->userList.push_front(Admin);
            error_manager->currentUser = &error_manager->userList.front(); 
        }
        else if (last_exception.ex_flags & USER_MANAGMENT_FAULT) 
        {
            error_manager->WriteObject();
        }
    }
};

#endif