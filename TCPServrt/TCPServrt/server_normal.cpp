#include <stdio.h>    
#include <winsock2.h>    

#pragma comment(lib,"ws2_32.lib")    

static char revData[1024];



struct msg {
	int   msgleng;
	char msghandler[512];
};

struct msgheader {
	int  main;
	int  sub;
	int  length;
};




//���ǵ�Ч�� ��������ǲ��е�
void moveBbuffer(int length)
{
	char itemcopy[1024];
	memset(&itemcopy, 0, sizeof(itemcopy));
	if (sizeof(revData) > length)
	{
		memcpy(itemcopy, length + revData, length);
		memset(revData, 0, sizeof(revData));
		memcpy(revData, itemcopy, sizeof(itemcopy));
	}
}

char* getbufferData(int length)
{
	char data[1024];
	memset(data, 0, 1024);
	memcpy(data, revData, length);
	moveBbuffer(length);
	return data;
}


void initsizele(int sizes)
{
	int headerint = sizeof(msgheader);
	if (headerint >= strlen(revData))
	{
		msgheader* header = (msgheader*)getbufferData(headerint);
		msg*	message = (msg*)getbufferData(header->length);
		printf("");
	}
}

int main_socket(int argc, char* argv[])
{
	//��ʼ��WSA    
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���    
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�    
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����    
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������    
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		memset(revData, 0, sizeof(revData));

		//��������    
		int ret = recv(sClient, revData, 1024, 0);
		if (ret > 0)
		{
			initsizele(ret);
			printf("===============%d",sizeof(ret));
		}

		//��������    
		const char * sendData = "��ã�TCP�ͻ��ˣ�\n";
		send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}

