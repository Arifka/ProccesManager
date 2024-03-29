#ifndef SMARTHANDLE_H
#define SMARTHANDLE_H
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

class SmartHandle
{
public:
    SmartHandle(HANDLE handle);
    ~SmartHandle();
    operator bool();
    operator HANDLE();
    HANDLE handle();
private:
    HANDLE _handle = NULL;
};

struct ProcessInfo
{
    PROCESSENTRY32 pe;
    std::vector<THREADENTRY32> threads;
};

#endif // SMARTHANDLE_H
