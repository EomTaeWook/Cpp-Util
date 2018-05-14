#include "MSMQ.h"

NS_MESSAGE_MSMQ_BEGIN

void MSMQ::Init(std::wstring pathName, MessageQueueMode mode)
{
	if (pathName.size() == 0)
		throw "MQ_ERROR_INVALID_PARAMETER";

	_mode = mode;
	DWORD formatNameBufferLength = 256;
	WCHAR formatNameBuffer[256];
	auto hr = MQPathNameToFormatName(pathName.c_str(),
		formatNameBuffer,
		&formatNameBufferLength);

	if (FAILED(hr))
	{
		// Set queue properties.
		const int NUMBEROFPROPERTIES = 2;
		MQQUEUEPROPS queueProps;
		QUEUEPROPID queuePropId[NUMBEROFPROPERTIES];
		MQPROPVARIANT queuePropVar[NUMBEROFPROPERTIES];
		DWORD propId = 0;
		queuePropId[propId] = PROPID_Q_PATHNAME;
		queuePropVar[propId].vt = VT_LPWSTR;
		queuePropVar[propId].pwszVal = const_cast<wchar_t*>(pathName.c_str());
		propId++;
		MQCreateQueue(NULL, &queueProps, formatNameBuffer, &formatNameBufferLength);
	}
	if (_mode == MessageQueueMode::READ)
		MQOpenQueue(formatNameBuffer, (int)MessageQueueAccess::RECEIVE_ACCESS, (int)MessageQueueShare::DENY_RECEIVE_SHARE, &_hQueue);
	else
		MQOpenQueue(formatNameBuffer, (int)MessageQueueAccess::SEND_ACCESS, (int)MessageQueueShare::DENY_NONE, &_hQueue);
}
HRESULT MSMQ::ReadingQueue(Message& message, int timeOutMillis)
{
	if (_hQueue == NULL)
		throw "Invalid Handle";

	// Define an MQMSGPROPS structure.
	const int NUMBEROFPROPERTIES = 5;
	DWORD propId = 0;

	MQMSGPROPS msgProps;
	MSGPROPID msgPropId[NUMBEROFPROPERTIES];
	MQPROPVARIANT msgPropVar[NUMBEROFPROPERTIES];
	HRESULT msgStatus[NUMBEROFPROPERTIES];

	// Specify the message properties to be retrieved.  
	msgPropId[propId] = PROPID_M_LABEL_LEN;           // Property ID  
	msgPropVar[propId].vt = VT_UI4;                   // Type indicator  
	msgPropVar[propId].ulVal = MQ_MAX_MSG_LABEL_LEN;  // Length of label  
	propId++;

	WCHAR labelBuffer[MQ_MAX_MSG_LABEL_LEN];        // Label buffer  
	msgPropId[propId] = PROPID_M_LABEL;				// Property ID  
	msgPropVar[propId].vt = VT_LPWSTR;				// Type indicator  
	msgPropVar[propId].pwszVal = labelBuffer;       // Label buffer  
	propId++;

	msgPropId[propId] = PROPID_M_BODY_SIZE;           // Property ID  
	msgPropVar[propId].vt = VT_NULL;                  // Type indicator  
	propId++;

	msgPropId[propId] = PROPID_M_BODY;							// Property ID  
	msgPropVar[propId].vt = VT_VECTOR | VT_UI1;					// Type indicator
	msgPropVar[propId].caub.pElems = message.GetBuffer().get();	// Body buffer
	msgPropVar[propId].caub.cElems = message.GetBufferSize();	// Buffer size
	propId++;

	msgPropId[propId] = PROPID_M_BODY_TYPE;						// Property ID
	msgPropVar[propId].vt = VT_NULL;							// Type indicator
	propId++;

	// Initialize the MQMSGPROPS structure.  
	msgProps.cProp = propId;									// Number of message properties  
	msgProps.aPropID = msgPropId;								// IDs of the message properties  
	msgProps.aPropVar = msgPropVar;								// Values of the message properties  
	msgProps.aStatus = msgStatus;								// Error reports  

	msgPropVar[0].ulVal = MQ_MAX_MSG_LABEL_LEN;
	auto hr = MQReceiveMessage(_hQueue,
		timeOutMillis,
		MQ_ACTION_RECEIVE,
		&msgProps,
		NULL,
		NULL,
		NULL,
		MQ_NO_TRANSACTION);
	if (hr == S_OK)
		message.SetReadSize(msgPropVar[2].ulVal);
	return hr;
}
HRESULT MSMQ::SendingQueue(Message& message)
{
	if (_hQueue == NULL)
		throw "Invalid Handle";

	const int NUMBEROFPROPERTIES = 5;
	DWORD propId = 0;

	// Define an MQMSGPROPS structure.  
	MQMSGPROPS msgProps;
	MSGPROPID msgPropId[NUMBEROFPROPERTIES];
	MQPROPVARIANT msgPropVar[NUMBEROFPROPERTIES];
	HRESULT msgStatus[NUMBEROFPROPERTIES];

	// Specify the message properties to be sent.  
	WCHAR labelBuffer[MQ_MAX_MSG_LABEL_LEN];
	msgPropId[propId] = PROPID_M_LABEL;					// Property ID  
	msgPropVar[propId].vt = VT_LPWSTR;					// Type indicator  
	msgPropVar[propId].pwszVal = labelBuffer;			// The message label  
	propId++;

	msgPropId[propId] = PROPID_M_BODY;							// Property ID 
	msgPropVar[propId].vt = VT_VECTOR | VT_UI1;					// Type indicator  
	msgPropVar[propId].caub.pElems = message.GetBuffer().get();	// Body buffer
	msgPropVar[propId].caub.cElems = message.GetBufferSize();	// Buffer size
	propId++;

	// Initialize the MQMSGPROPS structure.  
	msgProps.cProp = propId;
	msgProps.aPropID = msgPropId;
	msgProps.aPropVar = msgPropVar;
	msgProps.aStatus = msgStatus;

	return MQSendMessage(_hQueue, &msgProps, MQ_NO_TRANSACTION);
}
NS_MESSAGE_MSMQ_END