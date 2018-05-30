#include "HttpClient.h"
NS_WEB_HTTP_BEGIN
void HttpClient::Init(std::string url)
{
	try
	{
		_url.Parse(url);
		if (_url.GetProtocol() == L"https")
			_protocolType = WINHTTP_FLAG_SECURE;
	}
	catch (...)
	{
		throw std::exception("InitException");
	}
}
std::string HttpClient::SendRequest(std::string requestData, Method method)
{
	try
	{
		_session = WinHttpOpen(L"0",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS,
			0);
				
		if (_session)
		{
			_connect = WinHttpConnect(_session,
				_url.GetHost().c_str(),
				_url.GetPort(),
				0);
		}
		std::wstring type = L"GET";
		std::wstring path = _url.GetPath().c_str();
		switch (method)
		{
		case Method::Get:
			path.append(requestData.begin(), requestData.end());
			requestData.clear();
			break;
		case Method::Post:
			type = L"POST";
			break;
		case Method::Put:
			type = L"PUT";
			break;
		case Method::Delete:
			type = L"Delete";
			break;
		}

		_request = WinHttpOpenRequest(_connect,
			type.c_str(),
			path.c_str(),
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			_protocolType);

		if (_header.size() == 0)
			_header = L"content-type:application/x-www-form-urlencoded";

		WinHttpAddRequestHeaders(_request, _header.c_str(), _header.size(), WINHTTP_ADDREQ_FLAG_ADD);

		if (WinHttpSendRequest(_request,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			(LPVOID)requestData.c_str(),
			requestData.size(),
			requestData.size(),
			(DWORD_PTR)this))
		{
			if (WinHttpReceiveResponse(_request, NULL))
			{
				unsigned long readSize = 0;
				do
				{
					WinHttpQueryDataAvailable(_request, &readSize);
					if (readSize <= 0) break;
					WinHttpReadData(_request, _buffer, sizeof(_buffer), &readSize);
					_response.append(_buffer, readSize);
					readSize = 0;
				} while (readSize > 0);
			}
		}
		else
		{
			throw std::exception();
		}
	}
	catch (...)
	{
		Close();
		throw std::exception("RequestException");
	}
	Close();
	return _response;
}
bool HttpClient::SendRequestAsync(std::string requestData, Method method, std::function<void(int, std::string&)> callback)
{
	try
	{
		_session = WinHttpOpen(L"0",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS,
			WINHTTP_FLAG_ASYNC);

		if (_session)
		{
			_connect = WinHttpConnect(_session,
				_url.GetHost().c_str(),
				_url.GetPort(),
				0);
		}

		std::wstring type = L"GET";
		std::wstring path = _url.GetPath().c_str();
		_callback = callback;
		switch (method)
		{
		case Method::Get:
			path.append(requestData.begin(), requestData.end());
			requestData.clear();
			break;
		case Method::Post:
			type = L"POST";
			break;
		case Method::Put:
			type = L"PUT";
			break;
		case Method::Delete:
			type = L"Delete";
			break;
		}

		_request = WinHttpOpenRequest(_connect,
			type.c_str(),
			path.c_str(),
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			_protocolType);

		if (_header.size() == 0)
			_header = L"content-type:application/x-www-form-urlencoded";

		WinHttpAddRequestHeaders(_request, _header.c_str(), _header.size(), WINHTTP_ADDREQ_FLAG_ADD);

		WinHttpSetStatusCallback(_request,
			HttpClient::AsyncCallback,
			WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS,
			0);

		auto result = WinHttpSendRequest(_request,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			(LPVOID)requestData.c_str(),
			requestData.size(),
			requestData.size(),
			(DWORD_PTR)this);
		return result;
	}
	catch (...)
	{
		throw std::exception("RequestException");
	}
}
void HttpClient::HeaderAppend(std::string header)
{
	if (header.size() == 0) return;
	auto data = std::wstring().assign(header.begin(), header.end());
	_header.append(data + L"\n");
}
void HttpClient::Close()
{
	if (_request) WinHttpCloseHandle(_request);
	if (_connect) WinHttpCloseHandle(_connect);
	if (_session) WinHttpCloseHandle(_session);
}

void HttpClient::OnCallback(unsigned long code, void* info, unsigned long length)
{
	switch (code)
	{
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
		WinHttpReceiveResponse(_request, NULL);
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
	{
		unsigned long status = 0;
		unsigned long statusSize = sizeof(unsigned long);
		std::string response;
		WinHttpQueryHeaders(_request,
			WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
			WINHTTP_HEADER_NAME_BY_INDEX,
			&status,
			&statusSize,
			WINHTTP_NO_HEADER_INDEX);
		_responseCode = status;
		WinHttpQueryDataAvailable(_request, NULL);
		break;
	}
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
	{
		WinHttpReadData(_request, &_buffer, sizeof(_buffer), NULL);
		break;
	}
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
	{
		if (length != 0)
		{
			_response.append(_buffer, length);
			WinHttpQueryDataAvailable(_request, NULL);
		}
		else
		{
			if (_callback != nullptr)
			{
				_callback(_responseCode, _response);
				this->Close();
			}
		}
		break;
	}
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		if (_callback != nullptr)
		{
			_callback(code, _response.append("WINHTTP_CALLBACK_STATUS_REQUEST_ERROR"));
			this->Close();
		}
		break;
	}
}
void __stdcall HttpClient::AsyncCallback(HINTERNET handle, DWORD_PTR context, DWORD status, void* info, DWORD infoLength)
{
	auto client = reinterpret_cast<HttpClient*>(context);
	if (client != NULL)
		client->OnCallback(status, info, infoLength);
}
NS_WEB_HTTP_END