#pragma once
#pragma once
#include "NS.h"
#include <string>
#include <algorithm>
NS_WEB_BEGIN
class Url
{
public:
	Url() {}
	Url(std::string url)
	{
		Parse(std::wstring().assign(url.begin(), url.end()));
	}
	Url(std::wstring url)
	{
		Parse(url);
	}
	virtual ~Url() {}
private:
	std::wstring _path;
	std::wstring _protocol;
	std::wstring _host;
	int _port;
public:
	void Parse(std::string url);
	void Parse(std::wstring url);
	std::wstring GetPath();
	std::wstring GetProtocol();
	std::wstring GetHost();
	int GetPort();
};
inline void Url::Parse(std::string url)
{
	Parse(std::wstring().assign(url.begin(), url.end()));
}
inline void Url::Parse(std::wstring url)
{
	if (url.size() == 0) return;
	auto beginIdx = url.find(L"://");
	if (beginIdx != std::wstring::npos)
	{
		_protocol = url.substr(0, beginIdx).c_str();
		std::transform(_protocol.begin(), _protocol.end(), _protocol.begin(), ::tolower);
		beginIdx += 3;
	}
	else
	{
		_protocol = L"http";
		beginIdx = 0;
	}
	auto endIdx = url.find(L"/", beginIdx);
	if (endIdx != std::wstring::npos)
	{
		_host = url.substr(beginIdx, endIdx - beginIdx);
		std::transform(_host.begin(), _host.end(), _host.begin(), ::tolower);
		beginIdx = endIdx;
	}
	else
		_host = url.substr(beginIdx, url.size() - beginIdx);

	endIdx = _host.rfind(L":");
	if (endIdx != std::wstring::npos)
	{
		_port = std::stoi(_host.substr(endIdx + 1, _host.size() - endIdx));
		_host.erase(endIdx, _host.size() - endIdx);
	}
	else
		_port = 80;
	if (beginIdx != 0)
	{
		_path = url.substr(beginIdx, url.size() - beginIdx);
		std::transform(_path.begin(), _path.end(), _path.begin(), ::tolower);
	}
}
inline std::wstring Url::GetPath()
{
	return _path;
}
inline std::wstring Url::GetProtocol()
{
	return _protocol;
}
inline std::wstring Url::GetHost()
{
	return _host;
}
inline int Url::GetPort()
{
	return _port;
}
NS_WEB_END