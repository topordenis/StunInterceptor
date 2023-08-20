#pragma once
#include "CNamedPipe.h"
#include <functional>
#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define INSTANCES 24 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096
typedef struct
{
    OVERLAPPED oOverlap;
    HANDLE hPipeInst;
    TCHAR chRequest[BUFSIZE];
    DWORD cbRead;
    TCHAR chReply[BUFSIZE];
    DWORD cbToWrite;
    DWORD dwState;
    BOOL fPendingIO;
} PIPEINST, * LPPIPEINST;
class CNamedPipeServer : CNamedPipe
{
public:

    CNamedPipeServer(const std::string& pipeName) : CNamedPipe(pipeName) {
        std::cout << "CNamedPipeServer " << pipeName << std::endl;
        this->pipeName_ = pipeName;
    };

    std::function<void(std::string data)> m_FuncReceive;
    void WaitForClient();
    void GetAnswerToRequest(LPPIPEINST pipe);
    bool Start();

    void Cristos();
};

