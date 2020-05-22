#include<Winsock2.h>
#include<stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#define DATA_BUFFER 1024

int RegisterLog(SOCKET sClient1, struct sockaddr_in ser1,int iLen1) {
    printf("请输入1 2 3:\n");
    char tmp[10];
    scanf("%s", &tmp);
    if (strcmp(tmp, "1") == 0) {
        char AccPass[20];
        char pack[50];
        strcpy(pack, tmp);
        strcat(pack, " ('");
        printf("请输入账号\n");
        scanf("%s", &AccPass);
        strcat(pack, AccPass);
        strcat(pack, "',");
        printf("请输入密码\n");
        scanf("%s", &AccPass);
        strcat(pack, AccPass);
        strcat(pack, ",");
        printf("请输入昵称\n");
        scanf("%s", &AccPass);
        strcat(pack, "'");
        strcat(pack, AccPass);
        strcat(pack, "')");
        int iSend1 = sendto(sClient1, pack, sizeof(pack), 0, (struct sockaddr*) & ser1, iLen1);
        return 1;
    }
    else if (strcmp(tmp, "2") == 0) {
        char AccPass[20];
        char pack[50];
        strcpy(pack, tmp);
        printf("请输入账号\n");
        scanf("%s", &AccPass);
        strcat(pack, AccPass);
        strcat(pack, " ");
        printf("请输入密码\n");
        scanf("%s", &AccPass);
        strcat(pack, AccPass);
        int iSend1 = sendto(sClient1, pack, sizeof(pack), 0, (struct sockaddr*) & ser1, iLen1);
        return 2;
    }
    else if (strcmp(tmp, "3") == 0){
        return 3;
    }
    else {
        printf("请重新输入\n");
    }
}

void main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET sClient1, sClient2;
    int iPort1 = 5051, iPort2 = 5053;
    int   iLen1, iLen2;
    int   iSend1, iSend2;
    int   iRecv1, iRecv2;
    char send_buf[] = "Hello! I am a client.";
    char recv_buf1[DATA_BUFFER], recv_buf2[DATA_BUFFER];
    char messege[DATA_BUFFER];
    struct sockaddr_in ser1, ser2;
    fflush(stdin);
    memset(recv_buf1, 0, sizeof(recv_buf1));
    memset(recv_buf2, 0, sizeof(recv_buf2));
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock.\n");
        return;
    }
    ser1.sin_family = AF_INET;
    ser1.sin_port = htons(iPort1);
    ser1.sin_addr.s_addr = inet_addr("192.168.0.103");
    sClient1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (sClient1 == INVALID_SOCKET)
    {
        printf("socket( ) Failed:%d\n", WSAGetLastError());
        return;
    }
    iLen1 = sizeof(ser1);
    if(RegisterLog(sClient1, ser1, iLen1) == 3){
        closesocket(sClient1);
        WSACleanup();
        return;
    }
    while (1) {
        printf("请输入:\n");
        scanf("%s", messege);
        if (strcmp(messege, "4") == 0) {
            if (RegisterLog(sClient1, ser1, iLen1) == 3) {
                closesocket(sClient1);
                WSACleanup();
                return;
            }
        }
        else {
            iSend1 = sendto(sClient1, messege, sizeof(messege), 0, (struct sockaddr*) & ser1, iLen1);
            if (iSend1 == SOCKET_ERROR)
            {
                printf("sendto( ) Failed:%d\n", WSAGetLastError());
                return;
            }
            else if (iSend1 == 0)
                return;
            else
                printf("----------\n");
            if (messege[0] == 'E' && messege[1] == 'N' && messege[2] == 'D') {
                closesocket(sClient1);
                WSACleanup();
                return;
            }
        }
    }
}