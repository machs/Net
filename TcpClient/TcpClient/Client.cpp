#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

#define SERVER_ADDRESS "127.0.0.1"
#define	PORT	10001

#define SOCKET_NUM 1

struct Headers
{
	int main;
	int sub;
	int bobysize;
};

struct checkVersion
{
	int mainVer;
	int subVer;
	
};

int main()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) return 1;

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	SOCKET clientSocket;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	SOCKADDR_IN server;
	memset(&server, 0, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_port = htons(10001);
	inet_pton(server.sin_family, "127.0.0.1", &server.sin_addr);


	if (clientSocket == INVALID_SOCKET)
	{
		return -1;
	}
	
	checkVersion ver;
	ver.mainVer = 1;
	ver.subVer = 1;

	Headers headers;
	headers.main = 100;
	headers.sub = 101;
	headers.bobysize = sizeof(checkVersion);


	int sizes = sizeof(headers) + headers.bobysize;
	char data[1024];
	memset(data, 0, 1024);

	memcpy(data,&headers, sizeof(headers));
	memcpy(data, sizeof(headers) + &ver, sizeof(ver));

	send(clientSocket, data, sizes,0);


	do
	{

	} while (getchar() != 'q');

	WSACleanup();

	return 0;
}