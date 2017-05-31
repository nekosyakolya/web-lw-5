#pragma once
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>//����� ����� ���������� �������
#include <ws2tcpip.h>//��� ������ � ip �������
#include <stdio.h>

class CSocketImpl
{
public:
	CSocketImpl();
	int GetListenSocket() const;
	~CSocketImpl();
private:
	void InitSocketsLibrary();
	void InitHints();
	void InitAddr();
	void CreateSocket();
	void BindSocket();
	void InitSocket();

	WSADATA m_wsaData;   // ��������� ��������� ��� �������� ���������� � ���������� Windows Sockets
	struct addrinfo* m_addr; // ���������, �������� ���������� �� IP-������  ���������� ������
	struct addrinfo m_hints; // ������ ��� ������������� ��������� ������
	int m_listenSocket;
};

