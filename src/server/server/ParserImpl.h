#pragma once
#include "parser\request.h"
#include "parser\httprequestparser.h"

class CParserImpl
{
public:
	CParserImpl();
	bool IsValid(const char buf[]);
	std::string GetMethod()const;

	std::string GetPath()const;

	~CParserImpl();
private:
	httpparser::Request m_request;
	httpparser::HttpRequestParser m_parser;
};

