#include <WS2tcpip.h>
#include "CStunIntercept.h"

#include "Minhook/MinHook.h"
#include "Helpers.h"
#include <functional>
#include <stun++/message.h>
#include <ws2ipdef.h>

#include <algorithm>

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
			if (i->to<type::xor_peer_address>().to_sockaddr(&address)) {
				inet_ntop(address.sa_family, get_in_addr((struct sockaddr*)&address), s, sizeof s);
				std::string str(s);


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

			

			if (msg.type() == stun::message::type::create_perm_request) {
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
	if (msg.type() == stun::message::create_perm_response) {
		m_WaitingForCreatePermission = false;
		auto itx = std::find(m_Origins.begin(), m_Origins.end(), m_CurrentPeerIp);
		if (itx == m_Origins.end() && m_CurrentPeerIp.size() > 0)
		{
			auto itx2 = std::find_if(_results.begin(), _results.end(), [this](CInterceptResult& result) {return result.Ip == m_CurrentPeerIp; });
			if (itx2 == _results.end())
				_results.push_back({ m_CurrentPeerIp, m_CurrentPeerPort });
		}
	}
	 else if (msg.type() == stun::message::create_perm_error_response) {
		m_WaitingForCreatePermission = false;
		m_CurrentPeerPort = "";
		 m_CurrentPeerIp = "";
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