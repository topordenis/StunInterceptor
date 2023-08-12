#pragma once
#include <iostream>
#include <mutex>
#include <vector>
#include <Windows.h>

class CInterceptResult {
public:
	std::string Ip;
	std::string Port;
};


class CRevTelegram
{
public:
	char* dataOfInterest; //0x0000
	char pad_0008[112]; //0x0008
}; //Size: 0x0078
class CStunIntercept
{
public:

	void Start();
	void Stop();
	void Clear();
	void Save();
	
	static CStunIntercept& Get() {
		return *_instance;
	}
public:
	std::vector<CInterceptResult> _results;
	std::recursive_mutex m_Mutex;
private:
	inline static CStunIntercept* _instance;
private:
	std::vector<std::string> m_Origins;
	std::vector<std::string> m_Cached;
	std::string m_CurrentPeerPort;
	std::string m_CurrentPeerIp;
	bool m_WaitingForCreatePermission{ false };
public:
	void TryReadStunBuffer(char* buf, int len);
};

