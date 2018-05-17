#include "Encode.h"


void EncodeTestMain()
{

	auto TestObj = new Encode();
	Message msg;
	msg.sendAnthorID = 10001;
	msg.sendMsg = "jfhiasdfklghdaskjfghakjsgfHLSG";
	Header headers;
	headers.mianCommon = 1002;
	headers.subCommon = 2002;
	headers.bodyLength = sizeof(msg);

	char sendData[1024];
	memset(sendData, 0, 1024);

	memcpy(sendData, &headers, sizeof(headers));
	memcpy(sendData + sizeof(headers), &msg, sizeof(msg));
	//for (size_t i = 0; i < (sizeof(headers) + sizeof(msg)); i++)
	//{
	//	onRecvData(&sendData[i], 1);
	//}

	int length = sizeof(headers) + sizeof(msg);
	int headerlength = sizeof(headers);

	int index = 0;
	for (int i = 0; i < (sizeof(headers) + sizeof(msg)); i += 11)
	{
		char sendTemp[11];
		memset(sendTemp, 0, 11);
		memcpy(sendTemp, sendData + index++ * 11, 11);
		TestObj->onRecvData(sendTemp, 11);
	}

	getchar();

}