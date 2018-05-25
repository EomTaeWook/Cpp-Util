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
	HttpClient()
	{
		_protocolType = 0;
	}
	virtual ~HttpClient()
	{
		Close();
	}
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
	void Close();
private:
	void OnCallback(unsigned long code, void* info, unsigned long length);
private:
	static void __stdcall AsyncCallback(HINTERNET handle, DWORD_PTR context, DWORD status, void* info, DWORD infoLength);
};
NS_WEB_HTTP_END