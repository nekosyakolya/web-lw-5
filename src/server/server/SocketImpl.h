#pragma once
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>//здесь лежит реализация сокетов
#include <ws2tcpip.h>//для работы с ip адресом
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

	WSADATA m_wsaData;   // служебная структура для хранение информации о реализации Windows Sockets
	struct addrinfo* m_addr; // структура, хранящая информацию об IP-адресе  слущающего сокета
	struct addrinfo m_hints; // Шаблон для инициализации структуры адреса
	int m_listenSocket;
};

