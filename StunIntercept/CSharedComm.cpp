#include <Windows.h>
#include <iostream>

#include "CSharedComm.h"
#include "CNamedPipeClient.h"
#include "CNamedPipeServer.h"
#include "CNamedPipe.h"
#include <thread>

CSharedComm::CSharedComm() {

}
void CSharedComm::ThreadMain() {

	std::cout << "There are no servers active on this process which means we are the main server " << std::endl;

	auto server = std::make_shared<CNamedPipeServer>("\\\\.\\pipe\\detest");
	server->m_FuncReceive = std::move(m_ReceiveFunc);


	namedPipe = std::move(std::reinterpret_pointer_cast<CNamedPipe>(server));


	auto mata = server.get();
	mata->Cristos(); 



}
bool CSharedComm::TryAsClient() {

	{
		auto client = std::make_shared< CNamedPipeClient>("\\\\.\\pipe\\detest");
		if (client->Start()) {
			std::cout << "We connected succefully to the pipe. That means we are no the host" << std::endl;
			namedPipe = std::move(std::reinterpret_pointer_cast<CNamedPipe>(client));
			return true;
		}
		client.reset();
	}
	m_bServer = true;
	conThread = new std::thread(&CSharedComm::ThreadMain, this);
	conThread->detach();
	return false;
}
void CSharedComm::BindReceiveMessage(std::function<void(std::string msg)> func) {
	std::cout << "BindReceiveMessage called " << std::endl;
	std::cout << "m_bServer is " << m_bServer << std::endl;
	if (!m_bServer) return;

	this->m_ReceiveFunc = std::move(func);

}