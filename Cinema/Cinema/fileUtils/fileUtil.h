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
		if (connection_string == NULL)
			throw_exception("\nbase_entity(char* const src)\nsrc is NULL, terminating process", NULL, error_type::critical_error, NULL);
		connection_string = src;
		this->ReadObject();
	}

	virtual bool ReadObject() = 0;

	virtual bool WriteObject() = 0;

	virtual bool ReadObject(ofstream& in) = 0;

	virtual bool WriteObject(ofstream& out) = 0;

	friend ofstream& operator<<(ofstream& out, const base_entity& entity)
	{
		if (this->WriteObject(out) == true)
			return out;
		throw_exception("\n friend ifstream& operator>>(ifstream& in, const base_entity& entity)\n cause exception, terminating process", NULL, error_type::critical_error, NULL);;
		return out;
	}

	friend ifstream& operator>>(ifstream& in, const base_entity& entity)
	{
		if (this->ReadObject(in) == true)
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