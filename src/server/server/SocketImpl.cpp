#include "stdafx.h"
#include "SocketImpl.h"
#include <iostream>

CSocketImpl::CSocketImpl():
	m_addr(nullptr)
{
	InitSocketsLibrary();
	InitHints();
	InitAddr();
	CreateSocket();
	BindSocket();
	InitSocket();
	std::cout << "socket ready\n";
}

int CSocketImpl::GetListenSocket() const
{
	return m_listenSocket;
}


CSocketImpl::~CSocketImpl()
{
	closesocket(m_listenSocket);
	freeaddrinfo(m_addr);
	WSACleanup();
	std::cout << "socket delete\n";
}

void CSocketImpl::InitSocketsLibrary()
{
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
	{
		throw std::runtime_error("WSAStartup failed");
	}
}

void CSocketImpl::InitHints()
{
	ZeroMemory(&m_hints, sizeof(m_hints));

	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;
}

void CSocketImpl::InitAddr()
{
	if (0 != getaddrinfo("0.0.0.0", "80", &m_hints, &m_addr))
	{
		//WSACleanup();
		throw std::runtime_error("getaddrinfo failed");
	}
}

void CSocketImpl::CreateSocket()
{
	m_listenSocket = socket(m_addr->ai_family, m_addr->ai_socktype, m_addr->ai_protocol);
	if (INVALID_SOCKET == m_listenSocket)
	{
		//freeaddrinfo(m_addr);
		//WSACleanup();
		throw std::runtime_error("Creating of socket failed:" + WSAGetLastError());
	}
}

void CSocketImpl::BindSocket()
{
	if (SOCKET_ERROR == bind(m_listenSocket, m_addr->ai_addr, (int)m_addr->ai_addrlen))
	{
		//freeaddrinfo(m_addr);
		//closesocket(m_listenSocket);
		//WSACleanup();
		throw std::runtime_error("bind failed with error: " + WSAGetLastError());
	}
}

void CSocketImpl::InitSocket()
{
	if (SOCKET_ERROR == listen(m_listenSocket, SOMAXCONN))
	{
		//closesocket(m_listenSocket);
		//WSACleanup();
		throw std::runtime_error("listen failed with error: " + WSAGetLastError());
	}
}
