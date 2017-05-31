#pragma once
#include <string>
#include <memory>
#include "parser\request.h"
#include "parser\httprequestparser.h"

class CSocketImpl;
class CParserImpl;
class CFileListMakerImpl;


class CServer
{
public:
	CServer();
	void ProcessRequests();
	~CServer();
private:

	std::string ProcessGetRequest(const std::string&)const;

	void ReplaceSpaces(std::string&)const;

	bool IsExists(const std::string& path) const;

	std::string ProcessDeleteRequest(const std::string& path)const;

	std::string GetResponseForFileStructure(const std::string& path)const;
	std::string GetResponseForFile(const std::string& path) const;

	std::string GetContentTypeByExtension(const std::string& extension) const;

	bool IsIndexPage(const std::string& path) const;
	bool IsFilesStructure(const std::string& path) const;
	bool IsStylesheet(const std::string& path) const;
	bool IsScript(const std::string& path) const;
	bool IsFilesDirectory(const std::string& path) const;

	std::string FormResponse(size_t status, const std::string& contentType, const std::string& responseBody, const std::string charset = "utf-8")const;

	std::string GetResponseWhenIsNotExists(const std::string&path) const;
	std::string GetResponseForGetRequest(const std::string&path) const;
	std::string GetResponseForValidGetRequest(const httpparser::Request &m_request) const;

	std::unique_ptr<CSocketImpl> m_socket;

};

