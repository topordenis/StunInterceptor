#include <iostream>
#include "CStunChecker.h"
#include <functional>
#include <thread>
#include "Stun/stun++/message.h"
#include <WS2tcpip.h>
#include <ws2ipdef.h>

void CStunChecker::Exectute() {
	m_Thread = new std::thread(&CStunChecker::Run, this);
	m_Thread->detach();
}
void CStunChecker::Run() {
	int socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		return;
	}
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		{
			std::lock_guard lock(m_Mtx);
			if (!m_Iplist.empty()) {
				auto ip = m_Iplist.back();

				std::cout << "Handling ip " << std::endl;

				uint8_t tsx_id[12] = {
	0xfd,0x95,0xe8,0x83,
	0x8a,0x05,0x28,0x45,
	0x6a,0x8e,0xf1,0xe2
				};

				stun::message message(stun::message::binding_request, tsx_id);
				
				size_t bytesSent = sendto(socketFd, (const char*)message.data(), message.size(), 0,
					(struct sockaddr*)(ip.get()), sizeof(sockaddr));
				if (bytesSent == -1) {
					std::cerr << "Failed to send STUN request." << std::endl;
					continue;
				}
				char responseBuffer[1024];
				int fromlen = 0;
				size_t bytesRead = recvfrom(socketFd, responseBuffer, sizeof(responseBuffer), 0,
					(struct sockaddr*)&ip, &fromlen);
				if (bytesRead == -1) {
					std::cerr << "Failed to receive STUN response." << std::endl;
					continue;
				}

				std::cout << "Received " << bytesRead << " bytes from STUN server." << std::endl;

				m_Iplist.pop_back();
			}
		}
		
	}
}
void CStunChecker::Enqueue(std::shared_ptr<sockaddr> ip) {
	std::lock_guard lock(m_Mtx);
	//std::cout << "CStunChecker::Enqueue " << ip << std::endl;
	m_Iplist.push_front(ip);
}