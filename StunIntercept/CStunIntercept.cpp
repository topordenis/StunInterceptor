#include <WS2tcpip.h>
#include "CStunIntercept.h"
#include "CStunChecker.h"
#include "Minhook/MinHook.h"
#include "Helpers.h"
#include <functional>
#include <stun++/message.h>
#include <ws2ipdef.h>

#include <algorithm>
#include "CSharedComm.h"
#include "CNamedPipeClient.h"
#include "CNamedPipeServer.h"
#include "CNamedPipe.h"

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


bool isEqual(std::vector<int>& first, std::vector<int>& second)
{
	if (first.size() != second.size()) {
		return false;
	}

	std::sort(first.begin(), first.end());
	std::sort(second.begin(), second.end());

	return first == second;
}
class StunRemoteState {
public:
	std::string Ip;
	bool SendIndication = false;
	bool CreatePermissionRequest = false;
	bool CreatePermissionResponse = false;
	bool IsReady() { return SendIndication && CreatePermissionRequest && CreatePermissionResponse; };
};

USHORT get_in_port(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return (((struct sockaddr_in*)sa)->sin_port);

	return (((struct sockaddr_in6*)sa)->sin6_port);
}
void CStunIntercept::TryReadStunBuffer(char* buf, int len) {
	stun::message msg;



	static bool bIsWebrtc = Helpers::GetCurrentProcessName().find("Telegram") == std::string::npos;

	std::cout << "bIsWebrtc " << bIsWebrtc << std::endl;
	// Allocate a 2k memory block
	msg.resize(2 * 1024);

	// Receive network data directly into your STUN message bloc

	// Reduce the size to the packet size
	msg.resize(len);
	std::memcpy((void*)msg.data(), (void*)buf, len);

	// Check if this is a STUN message
	if (!msg.verify()) {
		//printf("Invalid STUN message! \n");
		return;
	}

	// Iterate over the message attributes
	using namespace stun::attribute;

	char s[INET6_ADDRSTRLEN];

	bool exists = false;
	for (stun::message::iterator i = msg.begin(), ie = msg.end(); i != ie; i++) {
		// First, check the attribute type
		//std::cout << "Found attribute " << i->type() << std::endl;


		switch (i->type()) {
		case type::software:
			//	std::cout << "got software " << i->to<type::software>().to_string() << std::endl;
			break;
		case type::username:
			//	std::cout << "got software " << i->to<type::username>().to_string() << std::endl;
			break;
		case type::xor_relayed_address:
		{
			sockaddr address;
			if (i->to<type::xor_relayed_address>().to_sockaddr(&address)) {
				inet_ntop(address.sa_family, get_in_addr((struct sockaddr*)&address), s, sizeof s);
				std::string str(s);

				m_RelayIp = str;

				std::cout << "got relayed ip " << str << std::endl;
				auto itx = std::find(m_Origins.begin(), m_Origins.end(), str);
				if (itx == m_Origins.end())
					m_Origins.push_back(str);
			}
		}
			break;
		case type::response_origin:
		{
			sockaddr address;
			if (i->to<type::xor_peer_address>().to_sockaddr(&address)) {
				inet_ntop(address.sa_family, get_in_addr((struct sockaddr*)&address), s, sizeof s);
				std::string str(s);

				auto itx = std::find(m_Origins.begin(), m_Origins.end(), str);
				if (itx == m_Origins.end())
					m_Origins.push_back(str);
			}
		}
			break;
		case type::xor_peer_address:
		{	sockaddr address;
		if (i->to<type::xor_peer_address>().to_sockaddr(&address)) {

			inet_ntop(address.sa_family, get_in_addr((struct sockaddr*)&address), s, sizeof s);
			std::string str(s);
			
		
			auto prt = get_in_port((struct sockaddr*)&address);
			
			{

			//	m_StunChecker->Enqueue(std::make_shared< sockaddr>(address));
			}
			if (bIsWebrtc && msg.type() == stun::message::type::channel_bind_request) {
				AddToResults(str, std::to_string(prt));
			}
			
			
			if (msg.type() == stun::message::type::create_perm_request) {
				std::cout << "m_RelayIp " << m_RelayIp << std::endl;
				std::cout << "xor ip " << str << std::endl;
			}

			if (!bIsWebrtc && msg.type() == stun::message::type::create_perm_request) {
				m_WaitingForCreatePermission = true;
				auto prt = get_in_port((struct sockaddr*)&address);
				m_CurrentPeerIp = str;
				m_CurrentPeerPort = std::to_string(prt);
			}
			
		}
		}
		break;
		// etc...
		}
	}
	if (!bIsWebrtc && msg.type() == stun::message::create_perm_response) {
		m_WaitingForCreatePermission = false;
		AddToResults(m_CurrentPeerIp, m_CurrentPeerPort);
	}
	 else if (msg.type() == stun::message::create_perm_error_response) {
		m_WaitingForCreatePermission = false;
		m_CurrentPeerPort = "";
		 m_CurrentPeerIp = "";
	 }
}

void CStunIntercept::AddToResults(std::string ip, std::string port)
{
	auto itx = std::find(m_Origins.begin(), m_Origins.end(), m_CurrentPeerIp);
	if (itx == m_Origins.end() && ip.size() > 0)
	{

		if (m_instComm != nullptr && m_instComm->namedPipe != nullptr) {
			if (m_instComm->m_bServer) {
				auto itx = std::find_if(_results.begin(), _results.end(), [ip, port](CInterceptResult& res) { return res.Ip == ip && res.Port == port; });
				if (itx == _results.end())
					_results.push_back({ ip, port });
			}
			else {
				auto client = reinterpret_cast<CNamedPipeClient*>(m_instComm->namedPipe.get());
				if (client != nullptr) {
					client->SendMessageAsync(ip + ":" + port);
					std::cout << "Sent message " << ip << " to main host " << std::endl;
				}
			}
		}
	}
}


int (WINAPI* precv)(SOCKET socket, char* buffer, int length, int flags) = NULL;
int (WINAPI* psend)(SOCKET socket, const char* buffer, int length, int flags) = NULL;

int WINAPI MyRecv(SOCKET socket, char* buffer, int length, int flags)
{
	CStunIntercept::Get().TryReadStunBuffer(buffer, length);
	return precv(socket, buffer, length, flags);
}
int (WINAPI* recvfrom_o)(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen) = NULL;


int recvfrom_hk(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen) {
	CStunIntercept::Get().TryReadStunBuffer(buf, len);
	return recvfrom_o(s, buf, len, flags, from, fromlen);
}

int (WINAPI* send_o)(SOCKET s, const char* buf, int len, int flags) = NULL;

int send_hk(SOCKET s, const char* buf, int len, int flags) {
	CStunIntercept::Get().TryReadStunBuffer((char*)buf, len);
	return send_o(s, buf, len, flags);
}


int (WINAPI* sendto_o)(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen) = NULL;
int sendto_hk(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen) {
	CStunIntercept::Get().TryReadStunBuffer((char*)buf, len);
	return sendto_o(s, buf, len, flags, to, tolen);
}
CStunIntercept::CStunIntercept(std::shared_ptr<CStunChecker> stunChecker, std::shared_ptr<CSharedComm> sharedCom) : m_StunChecker(stunChecker), m_instComm(sharedCom)
{

	
}
void CStunIntercept::Start() {
	
	_instance = this;
	if (MH_Initialize() != MH_OK) {
		return;
	}

	if (MH_CreateHookApi(L"Ws2_32", "recv", MyRecv, (LPVOID*)&precv) != MH_OK)
		printf("recv hook failed");
	if (MH_CreateHookApi(L"Ws2_32", "recvfrom", recvfrom_hk, (LPVOID*)&recvfrom_o) != MH_OK)
		printf("recv hook failed");
	if (MH_CreateHookApi(L"Ws2_32", "send", send_hk, (LPVOID*)&send_o) != MH_OK)
		printf("send hook failed");
	if (MH_CreateHookApi(L"Ws2_32", "sendto", sendto_hk, (LPVOID*)&sendto_o) != MH_OK)
		printf("sendto hook failed");
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		std::cout << "Failed to intialize hooks." << std::endl;
		return;
	}

	std::cout << "hook initialized succefully!";



}


void CStunIntercept::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void CStunIntercept::Clear() {
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);
	_results.clear();
}
std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

void CStunIntercept::ReceiveMessage(std::string msg)
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);
	size_t pos = 0;

	auto items = split(msg, ":");
	if (items.size() != 2) {
		return;
	}

	auto itx = std::find_if(_results.begin(), _results.end(), [items](CInterceptResult& res) { return res.Ip == items.at(0) && res.Port == items.at(1); });
	if (itx == _results.end())
		_results.push_back({ items.at(0), items.at(1)});
	std::cout << "CStunIntercept::ReceiveMessage " << msg << std::endl;
}
