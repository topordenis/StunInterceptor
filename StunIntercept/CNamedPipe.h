#pragma once
class CNamedPipe
{
public:
    CNamedPipe(std::string _pipeName) : pipeName_(_pipeName), pipeHandle_(nullptr) {};
    ~CNamedPipe();
    virtual bool Start() = 0; 
    virtual void SendMessageAsync(const std::string& message);

public:
    std::string pipeName_;
    OVERLAPPED overlapped_ = {};
    HANDLE pipeHandle_{ INVALID_HANDLE_VALUE };
};

