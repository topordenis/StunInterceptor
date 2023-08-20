#pragma once
#include <deque>
#include <mutex>
#include <thread>

struct sockaddr;

class CStunChecker
{
public:
	void Exectute();
	void Enqueue(std::shared_ptr<sockaddr> ip);

	void Run();

	std::deque<std::shared_ptr<sockaddr>> m_Iplist;
	std::thread* m_Thread;
	std::mutex m_Mtx;
};

