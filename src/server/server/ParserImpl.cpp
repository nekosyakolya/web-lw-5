#include "stdafx.h"
#include "ParserImpl.h"


CParserImpl::CParserImpl()
{
}

bool CParserImpl::IsValid(const char buf[])
{
	return httpparser::HttpRequestParser::ParsingCompleted == m_parser.parse(m_request, buf, buf + strlen(buf));
}

std::string CParserImpl::GetMethod() const
{
	return m_request.method;
}

std::string CParserImpl::GetPath() const
{
	return m_request.uri;
}



CParserImpl::~CParserImpl()
{
}
