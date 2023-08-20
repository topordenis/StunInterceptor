#pragma once
#include "CNamedPipe.h"
class CNamedPipeClient : CNamedPipe
{
public:
    CNamedPipeClient(const std::string& pipeName) : CNamedPipe(pipeName) {

    };
    bool Start();
    void SendMessageAsync(const std::string& message);
};

