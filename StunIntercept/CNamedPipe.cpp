#include <Windows.h>
#include <iostream>

#include "CNamedPipe.h"

CNamedPipe::~CNamedPipe() {
    {
        if (pipeHandle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(pipeHandle_);
        }
    }
}

void CNamedPipe::SendMessageAsync(const std::string& message)
{

}
