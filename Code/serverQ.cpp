#include <iostream>
#include<Winsock2.h>
#include<stdio.h>
#include <time.h>
#include<stdlib.h>
#include<windows.h> 
#include<sql.h> 
#include<sqlext.h> 
#include<sqltypes.h> 
#pragma comment(lib, "Ws2_32.lib")
#define BUFFER_LENGTH 1024

int iPort1 = 5051, iPort2 = 5052, iPort3 = 5053, iPort4 = 5054;
WSADATA wsaData;
SOCKET sSocket1, sSocket2, sClient1, sClient2;
int iLen1, iLen2, iLen3, iLen4;
int iSend1, iSend2;
int iRecv1, iRecv2;
char send_buf1[BUFFER_LENGTH], send_buf2[BUFFER_LENGTH];
char recv_buf1[BUFFER_LENGTH], recv_buf2[BUFFER_LENGTH];
struct sockaddr_in ser1, ser2, cli1, cli2, ser3, ser4;

void main() {
	char NameA[10], NameB[10];
	SQLRETURN ret;
	SQLHENV henv;//SQLHANDLE henv 
	SQLHDBC hdbc;//SQLHANDLE hdbc 
	SQLHSTMT hstmt;//SQLHANDLE hstmt 
	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境句柄 
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境属性 
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接句柄 
	ret = SQLConnect(hdbc, (SQLCHAR*)"account&password", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"939210531", SQL_NTS);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		printf("数据库连接成功\n");
	}
	else {
		printf("数据库连接失败\n");
		return;
	}
	printf("-------------------------------\n");
	printf("服务器运行中\n");
	printf("-------------------------------\n");
	fflush(stdin);
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock.\n ");
		return;
	}
	sSocket1 = socket(AF_INET, SOCK_DGRAM, 0);
	sSocket2 = socket(AF_INET, SOCK_DGRAM, 0);
	if (sSocket1 == INVALID_SOCKET)
	{
		printf("socket( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	if (sSocket2 == INVALID_SOCKET)
	{
		printf("socket( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	ser1.sin_family = AF_INET;
	ser1.sin_port = htons(iPort1);
	ser1.sin_addr.s_addr = inet_addr("192.168.0.103");
	ser2.sin_family = AF_INET;
	ser2.sin_port = htons(iPort2);
	ser2.sin_addr.s_addr = inet_addr("192.168.0.103");
	ser3.sin_family = AF_INET;
	ser3.sin_port = htons(iPort3);
	ser3.sin_addr.s_addr = inet_addr("192.168.0.103");
	ser4.sin_family = AF_INET;
	ser4.sin_port = htons(iPort4);
	ser4.sin_addr.s_addr = inet_addr("192.168.0.103");
	sClient1 = socket(AF_INET, SOCK_DGRAM, 0);
	sClient2 = socket(AF_INET, SOCK_DGRAM, 0);
	if (sClient1 == INVALID_SOCKET)
	{
		printf("socket( ) Failed:%d\n", WSAGetLastError());
		return;
	}
	if (sClient2 == INVALID_SOCKET)
	{
		printf("socket( ) Failed:%d\n", WSAGetLastError());
		return;
	}
	if (bind(sSocket1, (LPSOCKADDR)&ser1, sizeof(ser1)) == SOCKET_ERROR)
	{
		printf("bind( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	if (bind(sSocket2, (LPSOCKADDR)&ser2, sizeof(ser2)) == SOCKET_ERROR)
	{
		printf("bind( ) Failed: %d\n", WSAGetLastError());
		return;
	}
	iLen1 = sizeof(cli1);
	iLen2 = sizeof(cli2);
	iLen3 = sizeof(ser3);
	iLen4 = sizeof(ser4);
	memset(recv_buf1, 0, sizeof(recv_buf1));
	memset(recv_buf2, 0, sizeof(recv_buf2));
	timeval tv = { 10, 0 };
	bool flaga = false, flagb = false;
	bool AisOK = false, BisOK = false;
	bool OutA = false, OutB = false;
	int a = setsockopt(sSocket1, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));
	int b = setsockopt(sSocket2, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));
	while (1)
	{
		char option[20];
		iRecv1 = recvfrom(sSocket1, recv_buf1, BUFFER_LENGTH, 0,
			(SOCKADDR*)&cli1, &iLen1);
		if (iRecv1 == SOCKET_ERROR) {}
		else {
			strncpy(option, recv_buf1, 1);//接受A的消息
			option[1] = '\0';
			if (strcmp(option, "1") == 0) {//注册
				char sql[BUFFER_LENGTH] = "INSERT INTO AccountPassword VALUES";
				strcat(sql, recv_buf1 + 1);
				SQLCHAR SQL;
				ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
				ret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					printf("注册成功\n");
					printf("IP地址:[%s],端口:[%d]\n",
						inet_ntoa(cli1.sin_addr),
						ntohs(cli1.sin_port));
					printf("----------\n");
					printf("----------\n");
					strcpy(send_buf1, "注册成功");
					iSend1 = sendto(sClient1, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser3, iLen3);
					int begin, end, cnt = 0;
					for (int i = 0; i < strlen(sql); i++) {
						if (sql[i] == '\'') {
							cnt++;
							if (cnt == 1) {
								begin = i + 1;
							}
							else if (cnt == 2) {
								end = i - 1;
								break;
							}
						}
					}
					strncpy(NameA, sql + begin, end - begin + 1);
					NameA[end - begin + 1] = '\0';
					recv_buf1[0] = '\0';
				}
				else {
					printf("注册失败,账号已存在或账号太长\n");
					printf("IP地址:[%s],端口:[%d]\n",
						inet_ntoa(cli1.sin_addr),
						ntohs(cli1.sin_port));
					printf("----------\n");
					printf("----------\n");
					strcpy(send_buf1, "注册失败,账号已存在或账号太长");
					iSend1 = sendto(sClient1, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser3, iLen3);
				}
			}
			strncpy(option, recv_buf1, 1);
			option[1] = '\0';
			if (strcmp(option, "2") == 0) {//登录
				char account[20], password[20];
				SQLCHAR  TruePassword[20], Name[20];
				SQLLEN LenTruePassword, LenName;
				int cut;
				for (int i = 1; i < strlen(recv_buf1); i++) {
					if (recv_buf1[i] == ' ') {
						cut = i;
						strncpy(account, recv_buf1 + 1, i - 1);
						account[i - 1] = '\0';
						break;
					}
				}
				for (int i = strlen(account); i >= 0; i--) {
					account[i + 1] = account[i];
					if (i == 0) {
						account[i] = '\'';
						account[strlen(account) + 1] = '\0';
						account[strlen(account)] = '\'';
					}
				}
				strncpy(password, recv_buf1 + cut + 1, strlen(recv_buf1) - cut - 1);
				password[strlen(recv_buf1) - cut - 1] = '\0';
				char sql[BUFFER_LENGTH] = "select password,name from AccountPassword where account = ";
				strcat(sql, account);
				ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
				ret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					while (SQLFetch(hstmt) != SQL_NO_DATA) {//遍历结果集
						SQLGetData(hstmt, 1, SQL_C_CHAR, &TruePassword, 15, &LenTruePassword);
						SQLGetData(hstmt, 2, SQL_C_CHAR, &Name, 10, &LenName);
					}
					for (int i = 0; i < strlen((const char*)TruePassword); i++) {
						if (TruePassword[i] == ' ') {
							TruePassword[i] = '\0';
							break;
						}
					}
					if (strcmp((const char*)TruePassword, password) == 0) {
						printf("登陆成功\n");
						printf("IP地址:[%s],端口:[%d]\n",
							inet_ntoa(cli1.sin_addr),
							ntohs(cli1.sin_port));
						printf("----------\n");
						printf("----------\n");
						strcpy(send_buf1, "登陆成功");
						iSend1 = sendto(sClient1, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser3, iLen3);
						AisOK = true;
						strcpy(NameA, (const char*)Name);
						for (int i = 0; i < strlen(NameA); i++) {
							if (NameA[i] == ' ') {
								NameA[i] = '\0';
								break;
							}
						}
						recv_buf1[0] = '\0';
					}
					else {
						printf("密码错误或账号不存在\n");
						printf("IP地址:[%s],端口:[%d]\n",
							inet_ntoa(cli1.sin_addr),
							ntohs(cli1.sin_port));
						printf("----------\n");
						printf("----------\n");
						strcpy(send_buf1, "密码错误或账号不存在");
						iSend1 = sendto(sClient1, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser3, iLen3);
					}
				}
			}
			if (!flaga && AisOK) {//初次登录成功
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A,", localtime(&nowtime));
				strcat(tmp, NameA);
				printf("%s上线\n", tmp);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli1.sin_addr),
					ntohs(cli1.sin_port));
				flaga = true;
				printf("----------\n");
				printf("----------\n");
			}
			if (recv_buf1[0] == 'E' && recv_buf1[1] == 'N' && recv_buf1[2] == 'D') {//离线
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A,", localtime(&nowtime));
				strcat(tmp, NameA);
				printf("%s离线\n", tmp);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli1.sin_addr),
					ntohs(cli1.sin_port));
				printf("----------\n");
				printf("----------\n");
				OutA = true;
			}
			if (AisOK && recv_buf1[0] != '\0' && !OutA) {//正常对话
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A", localtime(&nowtime));
				printf("%s,%s:%s\n", tmp, NameA, recv_buf1);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli1.sin_addr),
					ntohs(cli1.sin_port));
				printf("----------\n");
				printf("----------\n");
				memset(send_buf1, 0, sizeof(send_buf1));
				strcat(send_buf1, tmp);
				strcat(send_buf1, ",");
				strcat(send_buf1, NameA);
				strcat(send_buf1, ":");
				strcat(send_buf1, recv_buf1);
				iSend1 = sendto(sClient2, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser4, iLen4);
			}
		}
		iRecv2 = recvfrom(sSocket2, recv_buf2, BUFFER_LENGTH, 0,
			(SOCKADDR*)&cli2, &iLen2);
		if (iRecv2 == SOCKET_ERROR) {}
		else {
			strncpy(option, recv_buf2, 1);//接受B的消息
			option[1] = '\0';
			if (strcmp(option, "1") == 0) {//注册
				char sql[BUFFER_LENGTH] = "INSERT INTO AccountPassword VALUES";
				strcat(sql, recv_buf2 + 1);
				SQLCHAR SQL;
				ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
				ret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					printf("注册成功\n");
					printf("IP地址:[%s],端口:[%d]\n",
						inet_ntoa(cli2.sin_addr),
						ntohs(cli2.sin_port));
					printf("----------\n");
					printf("----------\n");
					strcpy(send_buf1, "注册成功");
					iSend2 = sendto(sClient2, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser4, iLen4);
					int begin, end, cnt = 0;
					for (int i = 0; i < strlen(sql); i++) {
						if (sql[i] == '\'') {
							cnt++;
							if (cnt == 1) {
								begin = i + 1;
							}
							else if (cnt == 2) {
								end = i - 1;
								break;
							}
						}
					}
					strncpy(NameB, sql + begin, end - begin + 1);
					NameB[end - begin + 1] = '\0';
					recv_buf2[0] = '\0';
				}
				else {
					printf("注册失败,账号已存在或账号太长\n");
					printf("IP地址:[%s],端口:[%d]\n",
						inet_ntoa(cli2.sin_addr),
						ntohs(cli2.sin_port));
					printf("----------\n");
					printf("----------\n");
					strcpy(send_buf1, "注册失败,账号已存在或账号太长");
					iSend2 = sendto(sClient2, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser4, iLen4);
				}
			}
			strncpy(option, recv_buf2, 1);
			option[1] = '\0';
			if (strcmp(option, "2") == 0) {//登录
				char account[20], password[20];
				SQLCHAR  TruePassword[20], Name[20];
				SQLLEN LenTruePassword, LenName;
				int cut;
				for (int i = 1; i < strlen(recv_buf2); i++) {
					if (recv_buf2[i] == ' ') {
						cut = i;
						strncpy(account, recv_buf2 + 1, i - 1);
						account[i - 1] = '\0';
					}
				}
				for (int i = strlen(account); i >= 0; i--) {
					account[i + 1] = account[i];
					if (i == 0) {
						account[i] = '\'';
						account[strlen(account) + 1] = '\0';
						account[strlen(account)] = '\'';
					}
				}
				strncpy(password, recv_buf2 + cut + 1, strlen(recv_buf2) - cut - 1);
				password[strlen(recv_buf2) - cut - 1] = '\0';
				char sql[BUFFER_LENGTH] = "select password,name from AccountPassword where account = ";
				strcat(sql, account);
				ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
				ret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					while (SQLFetch(hstmt) != SQL_NO_DATA) {//遍历结果集
						SQLGetData(hstmt, 1, SQL_C_CHAR, &TruePassword, 15, &LenTruePassword);
						SQLGetData(hstmt, 2, SQL_C_CHAR, &Name, 10, &LenName);
					}
					for (int i = 0; i < strlen((const char*)TruePassword); i++) {
						if (TruePassword[i] == ' ') {
							TruePassword[i] = '\0';
							break;
						}
					}
					if (strcmp((const char*)TruePassword, password) == 0) {
						printf("登陆成功\n");
						printf("IP地址:[%s],端口:[%d]\n",
							inet_ntoa(cli2.sin_addr),
							ntohs(cli2.sin_port));
						printf("----------\n");
						printf("----------\n");
						strcpy(send_buf1, "登陆成功");
						iSend2 = sendto(sClient2, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser4, iLen4);
						BisOK = true;
						strcpy(NameB, (const char*)Name);
						for (int i = 0; i < strlen(NameB); i++) {
							if (NameB[i] == ' ') {
								NameB[i] = '\0';
								break;
							}
						}
						recv_buf2[0] = '\0';
					}
					else {
						printf("密码错误或账号不存在\n");
						printf("IP地址:[%s],端口:[%d]\n",
							inet_ntoa(cli2.sin_addr),
							ntohs(cli2.sin_port));
						printf("----------\n");
						printf("----------\n");
						strcpy(send_buf1, "密码错误或账号不存在");
						iSend2 = sendto(sClient2, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser4, iLen4);
					}
				}
			}
			if (!flagb && BisOK) {//初次登录
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A,", localtime(&nowtime));
				strcat(tmp, NameB);
				printf("%s上线\n", tmp);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli2.sin_addr),
					ntohs(cli2.sin_port));
				flagb = true;
				printf("----------\n");
				printf("----------\n");
			}
			if (recv_buf2[0] == 'E' && recv_buf2[1] == 'N' && recv_buf2[2] == 'D') {//离线
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A,", localtime(&nowtime));
				strcat(tmp, NameB);
				printf("%s离线\n", tmp);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli2.sin_addr),
					ntohs(cli2.sin_port));
				OutB = true;
			}
			if (BisOK && recv_buf2[0] != '\0' && !OutB) {//正常对话
				time_t nowtime = time(NULL);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A", localtime(&nowtime));
				printf("%s,%s:%s\n", tmp, NameB, recv_buf2);
				printf("IP地址:[%s],端口:[%d]\n",
					inet_ntoa(cli2.sin_addr),
					ntohs(cli2.sin_port));
				printf("----------\n");
				printf("----------\n");
				memset(send_buf1, 0, sizeof(send_buf1));
				strcat(send_buf1, tmp);
				strcat(send_buf1, ",");
				strcat(send_buf1, NameB);
				strcat(send_buf1, ":");
				strcat(send_buf1, recv_buf2);
				iSend1 = sendto(sClient1, send_buf1, sizeof(send_buf1), 0, (struct sockaddr*) & ser3, iLen3);
			}
		}
	}
	closesocket(sSocket1);
	closesocket(sSocket2);
	WSACleanup();
}