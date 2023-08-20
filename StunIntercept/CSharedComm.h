#pragma once
class CNamedPipe;
#include <thread>
#include <functional>
class CSharedComm
{
public:
	CSharedComm();
	std::shared_ptr< CNamedPipe> namedPipe;
	bool TryAsClient();

	std::thread* conThread;
	void ThreadMain();
	bool m_bServer{ false };
	std::function<void(std::string msg)> m_ReceiveFunc;

	void BindReceiveMessage(std::function<void(std::string msg)> func);
};

