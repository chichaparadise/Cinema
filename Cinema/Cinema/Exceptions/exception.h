#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "corecrt_terminate.h"

char* s = setlocale(LC_ALL, "Rus");

#define TERMINATE 1
#define CALL_HANDLING 2

//ONLY C LANG IMPLEMENTATION

typedef struct exception_entity_t
{
    void (*ex_handler)(void*);
    void* this_arg;
    uint16_t ex_flags;
    char msg_buff[256];
}Exception_info;

typedef enum error_type_t
{
    unknown_error = 0x01,
    critical_error = 0x02
}error_type;

static uint8_t program_condition = 0;

static Exception_info last_exception;

static void throw_exception(const char* _msg, void (*_ex_handler)(void*), uint16_t _ex_flags, void* _this)
{
    program_condition |= CALL_HANDLING;
    memcpy(last_exception.msg_buff, _msg, strlen(_msg) + 1);
    last_exception.ex_handler = _ex_handler;
    last_exception.this_arg = _this;
    last_exception.ex_flags = _ex_flags;
    if (_ex_flags & error_type::critical_error)
    {
        printf("\n\n%s", last_exception.msg_buff);
        if (_ex_handler != NULL)
            _ex_handler(_this);
        terminate();
    }
    else if (_ex_flags & error_type::unknown_error)
    {
        fprintf(stdout, "\n\n\n\n\n\n\t\tUNKNOWN ERROR\nTRY TO SOLVE IT OR TREMINATE THE PROCESS\n\n\n\n\n\n\n\n");
        fprintf(stdout, "%s", last_exception.msg_buff);
        terminate();
    }
    else
    {
        _ex_handler(_this);
    }
}

#endif /*EXCEPTION_H*/