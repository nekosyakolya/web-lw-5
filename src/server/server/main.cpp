#include "stdafx.h"
#include "Server.h"
#include <iostream>

int main()
{
	try
	{
		CServer server;
		server.ProcessRequests();
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what();
	}

	return EXIT_SUCCESS;
}

