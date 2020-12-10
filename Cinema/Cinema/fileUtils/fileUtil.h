#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include "..\Exceptions\exception.h"

using namespace std;

class base_entity
{
public:
	base_entity(char* const src)
	{
		this->connection_string = (char*)malloc(256);
		memcpy(this->connection_string, src, strlen(src));
		if (connection_string == NULL)
			throw_exception("\nbase_entity(char* const src)\nsrc is NULL, terminating process", NULL, error_type::critical_error, NULL);
		connection_string = src;
		this->ReadObject();
	}

	virtual bool ReadObject()
	{
		return false;
	}

	virtual bool WriteObject()
	{
		return false;
	}

	virtual bool ReadObject(ifstream& in)
	{
		return false;
	}

	virtual bool WriteObject(ofstream& out)
	{
		return false;
	}

	friend ofstream& operator<<(ofstream& out, base_entity& entity)
	{
		if (entity.WriteObject(out) == true)
			return out;
		throw_exception("\n friend ifstream& operator>>(ifstream& in, const base_entity& entity)\n cause exception, terminating process", NULL, error_type::critical_error, NULL);;
		return out;
	}

	friend ifstream& operator>>(ifstream& in, base_entity& entity)
	{
		if (entity.ReadObject(in) == true)
			return in;
		throw_exception("\n friend ifstream& operator>>(ifstream& in, const base_entity& entity)\n cause exception, terminating process", NULL, error_type::critical_error, NULL);
		return in;	
	}

    ~base_entity()
    {
		if ((void*)connection_string == NULL)
        	free((void*)connection_string);
    }
protected:
	char* connection_string;
};

#endif