#include "stdafx.h"
#include "Server.h"
#include "SocketImpl.h"

#include <iostream>
#include <fstream>


//filesystem
#include <filesystem>
#include <vector>
#include <algorithm>
#include <map>


//rapidjson
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson\document.h"
#include "rapidjson\writer.h"

namespace fs = std::tr2::sys;
const std::map<size_t, std::string> STATUS_CODES =
{
	{ 200, "OK" },
	{ 400, "Bad request" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" }
};



const std::string BASE_TYPE = "Application/octet-stream";
const std::map<std::string, std::string> CONTENT_TYPES =
{
	{ ".html", "Text/html" },
	{ ".txt", "Text/plain" },
	{ ".css", "Text/css" },
	{ ".js", "Text/javascript" },
	{ ".json", "Application/json" },
	{ ".png", "Image/png" },
	{ ".jpg", "Image/jpeg" },
	{ ".jpeg", "Image/jpeg" },
	{ ".gif", "Image/gif" },
	{ ".mpeg", "Video/mpeg" },
	{ ".mp4", "Video/mp4" },
	{ ".avi", "Video/avi" },
	{ ".wav", "Audio/x-wav" },
	{ ".mp3", "Audio/mpeg3" }
};



void GetAllFiles(std::vector<fs::path>&files, const fs::path & directoryPath)
{
	std::vector<fs::path> subFiles((fs::directory_iterator(directoryPath)), fs::directory_iterator());
	for (auto file : subFiles)
	{
		files.push_back(file);
	}
}

std::string GetJsonString(const rapidjson::Document& document)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	return buffer.GetString();
}

std::string DirectoryToJson(const std::string & path)
{
	rapidjson::Document document;
	document.SetObject();

	auto& allocator = document.GetAllocator();

	rapidjson::Value key("elements", allocator);
	rapidjson::Value elements(rapidjson::kArrayType);

	std::vector<fs::path> files;
	GetAllFiles(files, path);

	for (auto file : files)
	{
		std::string tmpStr = file.string();
		while (tmpStr.find("\\") != std::string::npos)
		{
			tmpStr.replace(tmpStr.find("\\"), 1, "/");
		}
		rapidjson::Value tmp(tmpStr, allocator);
		elements.PushBack(tmp, allocator);
	}
	document.AddMember(key, elements, allocator);

	return GetJsonString(document);
}



CServer::CServer():
	m_socket(new CSocketImpl)
{
}

void CServer::ProcessRequests()
{
	const int MAX_CLIENT_BUF_SIZE = 1024;
	char buf[MAX_CLIENT_BUF_SIZE];
	
	for (;;)
	{
		int clientSocket = accept(m_socket->GetListenSocket(), NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			throw std::runtime_error("accept failed: " + WSAGetLastError());
		}

		int result = recv(clientSocket, buf, MAX_CLIENT_BUF_SIZE - 1, 0);

		std::string response;
		if (result == SOCKET_ERROR)
		{
			throw std::runtime_error("Receive failed: " + result);
		}
		else if (result > 0)
		{
			if (result > MAX_CLIENT_BUF_SIZE)
			{
				throw std::logic_error("max size is" + MAX_CLIENT_BUF_SIZE);
			}

			buf[result] = '\0';
			httpparser::Request m_request;
			httpparser::HttpRequestParser m_parser;
			response = (httpparser::HttpRequestParser::ParsingCompleted != m_parser.parse(m_request, buf, buf + strlen(buf))) ? FormResponse(400, "Text / plain", "bad request") : GetResponseForValidGetRequest(m_request);

			if (send(clientSocket, response.c_str(), response.length(), 0) == SOCKET_ERROR)
			{
				throw std::runtime_error("send failed:" + WSAGetLastError());
			}
			closesocket(clientSocket);
		}
	}
}

CServer::~CServer()
{
}

std::string CServer::ProcessGetRequest(const std::string & path) const
{
	std::string filePath = (path != "/") ? path.substr(1, path.length()) : path;
	ReplaceSpaces(filePath);
	return (!IsExists(filePath)) ? GetResponseWhenIsNotExists(path) : GetResponseForGetRequest(filePath);
	
}

void CServer::ReplaceSpaces(std::string &path) const
{
	while (path.find("%20") != std::string::npos)
	{
		path.replace(path.find("%20"), 3, " ");
	}
}

bool CServer::IsExists(const std::string & path) const
{
	return fs::exists(fs::path(path));
}

std::string CServer::ProcessDeleteRequest(const std::string & path) const
{
	std::string response;
	std::string filePath = path.substr(1, path.length());
	ReplaceSpaces(filePath);

	if (IsFilesDirectory(filePath))
	{
		
		if (IsExists(filePath))
		{
			if (fs::is_regular_file(fs::path(filePath)))
			{
				fs::remove(fs::path(filePath));
				response = FormResponse(200, "Text/plain", filePath + " was deleted");
			}
			else
			{
				response = FormResponse(400, "Text/plain", filePath + " is not a file");
			}
		}
		else
		{
			response = GetResponseWhenIsNotExists(filePath);
		}
	}
	else
	{
		response = FormResponse(403, "Text/plain", "Access denied");
	}

	return response;
}

std::string CServer::GetResponseForFileStructure(const std::string & path) const
{
	return FormResponse(200, "Application/json", DirectoryToJson(path), "windows-1251");
	
}

std::string CServer::GetResponseForFile(const std::string & path)const
{

	std::string filePath = (path != "/") ? path : "index.html";

	std::string responseBody;
	if (fs::file_size(fs::path(filePath)) > 52428800)
	{
		
		return FormResponse(403, "Text/plain", "file size too large");
	}

	std::ifstream file(filePath, std::ios::binary | std::ios::in);

	char buf[512];
	while (file.read(buf, sizeof(buf)).gcount() > 0)
	{
		responseBody.append(buf, file.gcount());
	}
	
	
	std::string extension = fs::path(filePath).extension().string();
	std::string type = GetContentTypeByExtension(extension);
	std::string charset = "";
	if (".txt" == extension || ".html" == extension || ".css" == extension || ".js" == extension)
	{
		charset = "windows-1251";
	}

	for (size_t pos = 0; (pos = responseBody.find("\r\n", pos)) != std::string::npos && type == "Text/plain"; ++pos)
	{
		responseBody.replace(pos, 2, "<br />");
	}


	return FormResponse(200, type, responseBody, charset);
}

std::string CServer::GetContentTypeByExtension(const std::string & extension) const
{
	
	std::string type = BASE_TYPE;
	if (CONTENT_TYPES.find(extension) != CONTENT_TYPES.end())
	{
		type = CONTENT_TYPES.find(extension)->second;
	}
	return type;

}

bool CServer::IsIndexPage(const std::string & path) const
{
	return path == "index.html" || path == "/" ;
}

bool CServer::IsFilesStructure(const std::string & path) const
{
	return path =="files" || path == "files/";
}

bool CServer::IsStylesheet(const std::string & path) const
{
	return path.substr(0, 4) == "css/";
}

bool CServer::IsScript(const std::string & path) const
{
	return path.substr(0, 3) == "js/";
}

bool CServer::IsFilesDirectory(const std::string & path) const
{
	return path.substr(0, 6) == "files/";
}

std::string CServer::FormResponse(size_t status, const std::string & contentType, const std::string & responseBody, const std::string charset)const
{
	std::string response = "HTTP/1.1 " + std::to_string(status) + " " + STATUS_CODES.find(status)->second + "\r\n"
		+ "Version: HTTP/1.1\r\n"
		+ "Content-Type: " + contentType;
	if (!charset.empty())
	{
		response += "; charset=" + charset;
	}
	response += "\r\n";
	response += "Content-Length: " + std::to_string(responseBody.length());
	if (status == 400)
	{
		response += "\r\nConnection: close";
	}
	response += "\r\n\r\n" + responseBody;
	return response;
}

std::string CServer::GetResponseWhenIsNotExists(const std::string & path) const
{
	return FormResponse(404, "Text/plain", (path + " not found"));
}

std::string CServer::GetResponseForGetRequest(const std::string & path) const
{
	std::string response;

	if (IsIndexPage(path))
	{
		response = GetResponseForFile(path);
	}
	else if (IsFilesStructure(path))
	{
		response = GetResponseForFileStructure(path);
	}
	else if ((IsStylesheet(path) || IsScript(path) || IsFilesDirectory(path)) && fs::is_directory(fs::path(path)))
	{
		
			response = GetResponseForFileStructure(path);
		
	}
	else if ((IsStylesheet(path) || IsScript(path) || IsFilesDirectory(path)))
	{
			response = GetResponseForFile(path);
	
	}
	else
	{
		response = FormResponse(403, "Text/plain", "Access denied");
	}
	return response;
}

std::string CServer::GetResponseForValidGetRequest(const httpparser::Request &m_request) const
{
	std::string response;
	if (m_request.method == "GET")
	{
		response = ProcessGetRequest(m_request.uri);
	}
	else if (m_request.method == "DELETE")
	{
		response = ProcessDeleteRequest(m_request.uri);
	}
	else
	{
		response = FormResponse(400, "Text/plain", "bad request");
	}
	return response;
}
