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




//考虑到效率 这个方法是不行的
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
	//初始化WSA    
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字    
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口    
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听    
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据    
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		memset(revData, 0, sizeof(revData));

		//接收数据    
		int ret = recv(sClient, revData, 1024, 0);
		if (ret > 0)
		{
			initsizele(ret);
			printf("===============%d",sizeof(ret));
		}

		//发送数据    
		const char * sendData = "你好，TCP客户端！\n";
		send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}

