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

class CStunChecker;
class CSharedComm;
class CStunIntercept
{
private:
	std::shared_ptr<CStunChecker> m_StunChecker;
	std::shared_ptr< CSharedComm> m_instComm;
public:
	CStunIntercept() {};
	CStunIntercept(std::shared_ptr<CStunChecker> stunChecker, std::shared_ptr<CSharedComm> sharedCom);
	void Start();
	void Stop();
	void Clear();
	void Save();
	
	static CStunIntercept& Get() {
		return *_instance;
	}
public:
	void ReceiveMessage(std::string msg);
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
	bool m_bIsRelayed{ false };
	std::string m_RelayIp{};

	bool m_WaitingForCreatePermission{ false };
public:
	void TryReadStunBuffer(char* buf, int len);
private:
	void AddToResults(std::string ip, std::string port);
};

