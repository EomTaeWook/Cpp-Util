#pragma once
#include <Windows.h>
#include <winhttp.h>
#include "WebEnum.h"
#include "Url.h"
#include <string>
#include <functional>
#pragma comment(lib ,"winhttp.lib")
NS_WEB_HTTP_BEGIN
class HttpClient
{
public:
	HttpClient();
	virtual ~HttpClient();
private:
	HINTERNET _session;
	HINTERNET _connect;
	HINTERNET _request;
	int _protocolType;
	char _buffer[2048];
	Url _url;
	std::wstring _header;
	std::function<void(int, std::string&)> _callback;
	std::string _response;
	int _responseCode;
public:
	void Init(std::string url);
	bool SendRequestAsync(std::string requestData = "", Method method = Method::Get, std::function<void(int, std::string&)> callback = nullptr);
	std::string SendRequest(std::string requestData = "", Method method = Method::Get);
	void HeaderAppend(std::string header);
private:
	void Invoke(unsigned long code, void* info, unsigned long length);
	void Close();
private:
	static void __stdcall Invoke(HINTERNET handle, DWORD_PTR context, DWORD status, void* info, DWORD infoLength);
};
inline HttpClient::HttpClient()
{
	_protocolType = 0;
}
inline HttpClient::~HttpClient()
{
	Close();
}
NS_WEB_HTTP_END