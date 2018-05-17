#include "stdio.h"
#include "winsock2.h"


#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
#define SOCKET_SIZE 1024

void ErrorHandler(const char* message);

int main(int arge, char* argv[])
{

	SOCKET servSock, clntSock;
	SOCKADDR_IN	serverAddr, clntAddr;

	int clntAddrSz;
	int strlen;
	char buf[BUF_SIZE];

	fd_set fds, copyRead;
	SOCKET sockets[SOCKET_SIZE];
	TIMEVAL tm;
	int sockNum = 0;
	unsigned long u1 = 1;

	WSADATA	wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandler("WASStatup Error");
	}

	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(servSock == INVALID_SOCKET)
		ErrorHandler("servSock == INVALID_SOCKET");

	//���óɷ�����ģʽ
	//ioctlsocket(servSock, FIONBIO, &u1);

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8888);

	if (bind(servSock, (const sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		ErrorHandler("bind is error");
		return 0;
	}

	if (listen(servSock, SOCKET_SIZE) == SOCKET_ERROR)
	{
		ErrorHandler("lister is error");
		return 0;
	}

	//����socket,�����е�Ԫ��������Ч��socket
	for (size_t i = 0; i < SOCKET_SIZE; i++)
	{
		sockets[i] = INVALID_SOCKET;
	}
	sockNum += 1;

	sockets[0] = servSock;
	FD_ZERO(&fds);
	FD_SET(servSock, &fds);

	tm.tv_usec = 100000;
	tm.tv_sec = 0;

	while (true)
	{
		copyRead = fds;
		int selResult = select(sockNum, &copyRead, 0, 0, &tm);
		//printf("==========select return...\n");
		if (selResult == -1)
			puts("==========select error");
	/*	else if (selResult == 0)
			puts("==========timeout");*/
		else
		{
			//���ж��Ƿ����µĿͷ�������
			if (FD_ISSET(sockets[0], &copyRead))
			{
				clntAddrSz = sizeof(clntAddr);
				clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSz);
				//��socket���óɷ�����ģʽ
				ioctlsocket(servSock, FIONBIO, &u1);
				for (size_t i = 0; i < SOCKET_SIZE; i++)
				{
					//����socket������Ч��socket������ͷ���socket
					if (sockets[i] == INVALID_SOCKET)
					{
						FD_SET(clntSock, &fds);
						sockets[i] = clntSock;
						sockNum++;
						break;
					}
				}
			}

			//�������пͷ���socket,0��λ��Ϊ��������socket�����Դ�1��ʼ
			for (int i = 1; i < SOCKET_SIZE; i++)
			{
				//�����Чsocket ���ش���
				if (sockets[i] == INVALID_SOCKET) continue;
				if (FD_ISSET(sockets[i], &fds))
				{
					strlen = recv(sockets[i], buf, BUF_SIZE, 0);
					if (strlen <= 0) //�ͷ��˶Ͽ�������
					{
						closesocket(sockets[i]);
						//��fds��ɾ���ͷ���scoket
						FD_CLR(sockets[i], &fds);
						//����Ӧ��λ���ٴ���Ϊ��Ч
						sockets[i] = INVALID_SOCKET;
						sockNum--;
					}
					else
					{
						send(sockets[i], buf, strlen, 0);
					}
				}
			}
		}
	}
	closesocket(servSock);
	return 0;
}

void ErrorHandler(const char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

