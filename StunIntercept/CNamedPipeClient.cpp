#include <Windows.h>
#include <iostream>

#include "CNamedPipeClient.h"
#include <thread>
#include <sstream>

bool CNamedPipeClient::Start() {
    std::cout << "Creating named pipe client " << std::endl;
    {
        pipeHandle_ = CreateFile(
            pipeName_.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );

        if (pipeHandle_ == INVALID_HANDLE_VALUE) {

            std::stringstream s;
            s << "Failed to connect to named pipe(" << pipeName_ << "). Error code : " << GetLastError() << std::endl;

            OutputDebugString(s.str().c_str());
            return false;
        }

        std::stringstream s;
        s << "Client connect pipeHandle " << pipeHandle_ << std::endl;

        OutputDebugString(s.str().c_str());

       // std::cout << "Connected to the server." << std::endl;
       //std::thread* test = new std::thread([this]() {
       //     while (true) {
       //         Sleep(1000);
       //                 this->SendMessageAsync("Connected");
       //         
       //                // Sleep(10000);
       //     }
       //    
       //     });
       // test->detach();
        return true;
    }

}
void CNamedPipeClient::SendMessageAsync(const std::string& message){
    {
        OVERLAPPED overlapped = {};
        DWORD bytesWritten;

            WriteFile(pipeHandle_, message.c_str(), static_cast<DWORD>(message.length()), nullptr, NULL);
           // GetOverlappedResult(pipeHandle_, &overlapped, &bytesWritten, TRUE);
        
    }
}
