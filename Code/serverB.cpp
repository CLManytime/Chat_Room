// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Winsock2.h>
#include<stdio.h>
#include <time.h>
#include<stdlib.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUFFER_LENGTH 1024

void main() {
	int iPort1 = 5054;
	WSADATA wsaData;
	SOCKET sSocket1, sSocket2;
	int iLen1, iLen2;
	int iSend1, iSend2;
	int iRecv1, iRecv2;
	char send_buf[] = "Hello!I am a server";
	char recv_buf1[BUFFER_LENGTH], recv_buf2[BUFFER_LENGTH];
	struct sockaddr_in ser1, ser2, cli1, cli2;
	printf("-------------------------------\n");
	printf("服务器运行中\n");
	printf("-------------------------------\n");
	printf("1 输入1进行注册,输入2进行登录,输入3进行离开,输入4重新进入登录注册界面\n");
	printf("2 账号小于20字节,密码小于15字节,昵称小于10字节\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock.\n ");
		return;
	}
	sSocket1 = socket(AF_INET, SOCK_DGRAM, 0);
	if (sSocket1 == INVALID_SOCKET)
	{
		printf("socket( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	ser1.sin_family = AF_INET;
	ser1.sin_port = htons(iPort1);
	ser1.sin_addr.s_addr = inet_addr("192.168.0.103");
	if (bind(sSocket1, (LPSOCKADDR)&ser1, sizeof(ser1)) == SOCKET_ERROR)
	{
		printf("bind( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	iLen1 = sizeof(cli1);
	memset(recv_buf1, 0, sizeof(recv_buf1));
	timeval tv = { 10, 0 };
	int a = setsockopt(sSocket1, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));
	while (1)
	{
		iRecv1 = recvfrom(sSocket1, recv_buf1, BUFFER_LENGTH, 0,
			(SOCKADDR*)&cli1, &iLen1);
		if (iRecv1 == SOCKET_ERROR){}
		else
		{
			printf("%s\n", recv_buf1);
		}
	}
	closesocket(sSocket1);
	WSACleanup();
}

